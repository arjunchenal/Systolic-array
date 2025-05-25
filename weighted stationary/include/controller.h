#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "systemc.h"
#include "sram.h"
#include "systolicarray_weightedstationary.h"
#include "flipflop.h"
#include <iostream>

using namespace std;

template<int RowsA, int ColsA, int ColsB>
SC_MODULE(Controller) {
    sc_in<bool> clk;
    sc_in<bool> reset;

    // sc_signal<sc_int<8>>* X_in_sig;
    sc_signal<sc_int<8>> (&X_in_sig)[ColsA];

    SRAM<RowsA, ColsA, ColsB>* input_sram;
    SRAM<RowsA, ColsA, ColsB>* weight_sram;   
    SRAM<RowsA, ColsA, ColsB>* output_sram;

    systolicArray_weightedstationary<RowsA, ColsA, ColsB>* systolic_array;

    vector<vector<sc_int<8>>> preload_matA;
    vector<vector<sc_int<8>>> preload_matB;

    enum State { IDLE, PRELOAD_INPUTS, LOAD_WEIGHTS, LOAD_INPUTS, COMPUTE, WAIT_DONE, WRITE_OUTPUTS, DONE };

    State current_state;

    sc_signal<sc_int<8>> input_pipeline_data_in[ColsA];
    sc_signal<sc_int<8>> input_pipeline_data_out[ColsA];

    FlipFlop* input_pipeline[ColsA];
    sc_signal<bool> start;
    int compute_cycle;

    void controller_fsm() {
        current_state = IDLE;

        while (true) {
            wait();
    
            switch (current_state) {
                case IDLE:
                    cout << "[Controller] -> IDLE\n";
                    current_state = PRELOAD_INPUTS;
                    break;
    
                case PRELOAD_INPUTS:
                    cout << "[Controller] -> PRELOAD_INPUTS\n";
                    preload_sram();
                    current_state = LOAD_WEIGHTS;
                    break;
    
                case LOAD_WEIGHTS:
                    cout << "[Controller] -> LOAD_WEIGHTS\n";
                    load_weights_to_array();
                    current_state = LOAD_INPUTS;
                    break;
    
                case LOAD_INPUTS:
                    cout << "[Controller] -> LOAD_INPUTS\n";
                    load_inputs();
                    current_state = COMPUTE;
                    break;
    
                case COMPUTE:
                    cout << "[Controller] -> COMPUTE\n";
                    start.write(true);
                    compute_cycle = 0;
                    current_state = WAIT_DONE;
                    break;
    
                case WAIT_DONE:
                    cout << "[Controller] -> WAIT_DONE\n";
                    compute_cycle++;
                    // if (compute_cycle >= ColsA + ColsB) {
                    if (compute_cycle >= RowsA + ColsB) {
                        current_state = WRITE_OUTPUTS;
                    }
                    break;

                case WRITE_OUTPUTS:
                    cout << "[Controller] -> WRITE_OUTPUTS\n";
                    write_outputs();
                    current_state = DONE;
                    break;

                case DONE:
                    cout << "[Controller] -> DONE. Simulation finished.\n";
                    sc_stop();
                    break;
            }
        }
    }
    
    void preload_sram() {
        cout << "[Controller] Preloading matrices into SRAMs...\n";
        cout << " ROwsA is " << RowsA << " ColsA is " << ColsA << " ColsB is " << ColsB << endl;
        for (int i = 0; i < ColsA;i++) {
            for (int j = 0; j < RowsA; j++) {
                sc_int<8> value = preload_matA[j][i];
                cout << "value is " << value << endl;
                input_sram->write_input(i, j, value);
                cout << "  [Controller] Preloaded A[" << i << "][" << j << "] = " << value << std::endl;
            }
        }

        for (int i = 0; i < ColsA; i++) {
            for (int j = 0; j < ColsB; j++) {
                sc_int<8> value = preload_matB[i][j];
                weight_sram->write_weight(i, j, value);
                cout << "  [Controller] Preloaded B[" << i << "][" << j << "] = " << value << std::endl;
            }
        }
    }

    void load_weights_to_array() {
        for (int i = 0; i < ColsA; i++) {
            for (int j = 0; j < ColsB; j++) {
                sc_int<8> weight_value = weight_sram->read_weight(i, j);
                systolic_array->set_weight(i, j, weight_value);
                cout << "  [Controller] Loaded weight B[" << i << "][" << j << "] = " << weight_value << " to PE\n";
            }
        }
    }

    void load_inputs() {
    cout << "[Controller] Starting fixed skewed input streaming.\n";

    for (int col = 0; col < RowsA; ++col) {
        cout << "[Controller] Injecting A column " << col << "\n";

        for (int row = 0; row < ColsA; ++row) {
            sc_int<8> val = input_sram->read_input(row, col);  
            input_pipeline_data_in[row].write(val);
            cout << "  [Controller] A[" << row << "][" << col << "] = " << val << "\n";
        }

        wait(); 

        for (int row = 0; row < ColsA; ++row) {
            sc_int<8> delayed_val = input_pipeline_data_out[row].read();
            X_in_sig[row].write(delayed_val);
            cout << "  [Controller] Delayed output to PE[" << row << "] = " << delayed_val << "\n";
        }
    }

    for (int f = 0; f < ColsA - 1; ++f) {
        cout << "[Controller] Flush cycle " << RowsA + f << "\n";

        for (int row = 0; row < ColsA; ++row) {
            input_pipeline_data_in[row].write(0);
        }

        wait();

        for (int row = 0; row < ColsA; ++row) {
            sc_int<8> delayed_val = input_pipeline_data_out[row].read();
            X_in_sig[row].write(delayed_val);
            cout << "  [Controller] Delayed output to PE[" << row << "] = " << delayed_val << "\n";
        }
    }
}



    // void write_outputs() {
    //     for (int i = 0; i < RowsA; i++) {
    //         for (int j = 0; j < ColsB; j++) {
    //             sc_int<32> result = systolic_array->get_result(i, j);
    //             output_sram->write_output(i, j, result);
    //             cout << "[Controller] Output C[" << i << "][" << j << "] = " << result << "\n";
    //         }
    //     }
    // }


    void write_outputs() {
        for (int i = 0; i < RowsA; i++) {
            for (int j = 0; j < ColsB; j++) {
                sc_int<32> result_val = systolic_array->output_fifo[j]->read();
                output_sram->write_output(i, j, result_val);
                cout << "[Controller] Output C[" << i << "][" << j << "] = " << result_val << "\n";
            }
        }
    }


    SC_CTOR(Controller, sc_signal<sc_int<8>> (&X_in_ref)[ColsA])
    : X_in_sig(X_in_ref) {
        for (int i = 0; i < ColsA; i++) {
            string name = "flipflop_" + to_string(i);
            input_pipeline[i] = new FlipFlop(name.c_str(), i);
            input_pipeline[i]->clk(clk);
            input_pipeline[i]->reset(reset);
            input_pipeline[i]->data_in(input_pipeline_data_in[i]);
            input_pipeline[i]->data_out(input_pipeline_data_out[i]);
        }

        SC_THREAD(controller_fsm);
        sensitive << clk.pos();
        async_reset_signal_is(reset, true);
    }

    ~Controller() {
        for (int i = 0; i < ColsA; i++) {
            delete input_pipeline[i];
        }
    }
};

#endif

