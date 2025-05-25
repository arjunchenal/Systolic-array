// // testbench_weighted_stationary.cpp
// #include "systemc.h"
// #include "systolicarray_weightedstationary.h"
// #include "sram.h"
// #include "controller.h"
// #include <vector>
// #include <iostream>
// #include <cstdlib>
// #include <ctime>
// #include <random>

// const int ROWS_A = 4;
// const int COLS_A = 2;
// const int ROWS_B = 2;
// const int COLS_B = 2;

// SC_MODULE(TestbenchWeightStationary) {
//     sc_signal<bool> clk;
//     sc_signal<bool> reset;

//     sc_signal<sc_int<8>> X_in[ROWS_A];
//     sc_signal<sc_int<32>> result[ROWS_A][COLS_B];
//     sc_signal<sc_int<32>> shifted_out[ROWS_A][COLS_B];
//     sc_signal<bool> start_readout;

//     SRAM<ROWS_A, COLS_A, COLS_B>* input_sram;
//     SRAM<ROWS_A, COLS_A, COLS_B>* weight_sram;
//     SRAM<ROWS_A, COLS_A, COLS_B>* output_sram;

//     systolicArray_weightedstationary<ROWS_A, COLS_A, COLS_B>* systolic_array;
//     Controller<ROWS_A, COLS_A, COLS_B>* controller;

//     void generate_clock() {
//         while (true) {
//             clk.write(false);
//             wait(5, SC_NS);
//             clk.write(true);
//             wait(5, SC_NS);
//         }
//     }

//     vector<vector<sc_int<8>>> generate_random_matrix(int rows, int cols) {
//         int total = rows * cols;
//         vector<sc_int<8>> pool(total);
//         for (int i = 0; i < total; ++i) pool[i] = i + 1;
//         std::random_shuffle(pool.begin(), pool.end());

//         vector<vector<sc_int<8>>> mat(rows, vector<sc_int<8>>(cols));
//         int index = 0;
//         for (int i = 0; i < rows; ++i)
//             for (int j = 0; j < cols; ++j)
//                 mat[i][j] = pool[index++];

//         return mat;
//     }

//     vector<vector<sc_int<8>>> transpose(const vector<vector<sc_int<8>>> &mat) {
//         if (mat.empty() || mat[0].empty()) return {};
//         size_t rows = mat.size(), cols = mat[0].size();
//         vector<vector<sc_int<8>>> transposed(cols, vector<sc_int<8>>(rows));
//         for (size_t i = 0; i < rows; ++i)
//             for (size_t j = 0; j < cols; ++j)
//                 transposed[j][i] = mat[i][j];
//         return transposed;
//     }

//     void stimulate() {
//         vector<vector<sc_int<8>>> matA = generate_random_matrix(ROWS_A, COLS_A);
//         vector<vector<sc_int<8>>> matB = generate_random_matrix(COLS_A, COLS_B);

//         cout << "Generated random matrices:" << endl;
//         cout << "Matrix A:" << endl;
//         for (const auto& row : matA) {
//             for (const auto& elem : row) cout << elem << " ";
//             cout << endl;
//         }
//         cout << "Matrix B:" << endl;
//         for (const auto& row : matB) {
//             for (const auto& elem : row) cout << elem << " ";
//             cout << endl;
//         }

//         controller->preload_matA = matA;  //transpose(matA);
//         controller->preload_matB = matB;

//         reset.write(true);
//         wait(10, SC_NS);
//         reset.write(false);
//         wait(10, SC_NS);

//         wait(500, SC_NS);
//         sc_stop();
//     }

//     SC_CTOR(TestbenchWeightStationary) {
//         input_sram = new SRAM<ROWS_A, COLS_A, COLS_B>("input_sram");
//         weight_sram = new SRAM<ROWS_A, COLS_A, COLS_B>("weight_sram");
//         output_sram = new SRAM<ROWS_A, COLS_A, COLS_B>("output_sram");

//         systolic_array = new systolicArray_weightedstationary<ROWS_A, COLS_A, COLS_B>("systolic_array");
//         systolic_array->clk(clk);
//         systolic_array->reset(reset);

//         for (int i = 0; i < ROWS_A; i++)
//             systolic_array->X_in[i](X_in[i]);

//         for (int i = 0; i < ROWS_A; i++)
//             for (int j = 0; j < COLS_B; j++) {
//                 systolic_array->result[i][j](result[i][j]);
//                 systolic_array->shifted_out[i][j](shifted_out[i][j]);
//             }

//         controller = new Controller<ROWS_A, COLS_A, COLS_B>("controller", X_in);
//         controller->clk(clk);
//         controller->reset(reset);
//         controller->input_sram = input_sram;
//         controller->weight_sram = weight_sram;
//         controller->output_sram = output_sram;
//         controller->systolic_array = systolic_array;

//         systolic_array->start_readout(start_readout);

//         SC_THREAD(generate_clock);
//         SC_THREAD(stimulate);
//     }

//     ~TestbenchWeightStationary() {
//         delete systolic_array;
//         delete controller;
//         delete input_sram;
//         delete weight_sram;
//         delete output_sram;
//     }
// };

// int sc_main(int argc, char* argv[]) {
//     srand(time(NULL));
//     TestbenchWeightStationary tb("tb_weight");

//     sc_trace_file* tf = sc_create_vcd_trace_file("trace_weightstationary");
//     tf->set_time_unit(1, SC_NS);

//     sc_trace(tf, tb.clk, "clk");
//     for (int i = 0; i < ROWS_A; i++)
//         sc_trace(tf, tb.X_in[i], "X_in_" + std::to_string(i));
//     for (int i = 0; i < ROWS_A; i++)
//         for (int j = 0; j < COLS_B; j++)
//             sc_trace(tf, tb.shifted_out[i][j], "shifted_out_" + std::to_string(i) + "_" + std::to_string(j));

//     sc_start(1000, SC_NS);
//     sc_close_vcd_trace_file(tf);
//     return 0;
// }






















// testbench_weighted_stationary.cpp
#include "systemc.h"
#include "systolicarray_weightedstationary.h"
#include "sram.h"
#include "controller.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

const int ROWS_A = 2;
const int COLS_A = 2;
const int ROWS_B = 2;
const int COLS_B = 2;

SC_MODULE(TestbenchWeightStationary) {
    sc_signal<bool> clk;
    sc_signal<bool> reset;

    sc_signal<sc_int<8>> X_in[COLS_A];
    sc_signal<sc_int<32>> result[ROWS_A][COLS_B];
    sc_signal<sc_int<32>> shifted_out[ROWS_A][COLS_B];
    sc_signal<bool> start_readout;

    SRAM<ROWS_A, COLS_A, COLS_B>* input_sram;
    SRAM<ROWS_A, COLS_A, COLS_B>* weight_sram;
    SRAM<ROWS_A, COLS_A, COLS_B>* output_sram;

    systolicArray_weightedstationary<ROWS_A, COLS_A, COLS_B>* systolic_array;
    Controller<ROWS_A, COLS_A, COLS_B>* controller;

    void generate_clock() {
        while (true) {
            clk.write(false);
            wait(5, SC_NS);
            clk.write(true);
            wait(5, SC_NS);
        }
    }

    vector<vector<sc_int<8>>> generate_random_matrix(int rows, int cols) {
        int total = rows * cols;
        vector<sc_int<8>> pool(total);
        for (int i = 0; i < total; ++i) pool[i] = i + 1;
        std::random_shuffle(pool.begin(), pool.end());

        vector<vector<sc_int<8>>> mat(rows, vector<sc_int<8>>(cols));
        int index = 0;
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                mat[i][j] = pool[index++];

        return mat;
    }

    vector<vector<sc_int<8>>> transpose(const vector<vector<sc_int<8>>> &mat) {
        if (mat.empty() || mat[0].empty()) return {};
        size_t rows = mat.size(), cols = mat[0].size();
        vector<vector<sc_int<8>>> transposed(cols, vector<sc_int<8>>(rows));
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                transposed[j][i] = mat[i][j];
        return transposed;
    }

    void stimulate() {
        vector<vector<sc_int<8>>> matA = generate_random_matrix(ROWS_A, COLS_A);
        vector<vector<sc_int<8>>> matB = generate_random_matrix(COLS_A, COLS_B);

        cout << "Generated random matrices:" << endl;
        cout << "Matrix A:" << endl;
        for (const auto& row : matA) {
            for (const auto& elem : row) cout << elem << " ";
            cout << endl;
        }
        cout << "Matrix B:" << endl;
        for (const auto& row : matB) {
            for (const auto& elem : row) cout << elem << " ";
            cout << endl;
        }

        controller->preload_matA = matA; 
        controller->preload_matB = matB;

        reset.write(true);
        wait(10, SC_NS);
        reset.write(false);
        wait(10, SC_NS);

        wait(500, SC_NS);
        sc_stop();
    }

    SC_CTOR(TestbenchWeightStationary) {
        input_sram = new SRAM<ROWS_A, COLS_A, COLS_B>("input_sram");
        weight_sram = new SRAM<ROWS_A, COLS_A, COLS_B>("weight_sram");
        output_sram = new SRAM<ROWS_A, COLS_A, COLS_B>("output_sram");

        systolic_array = new systolicArray_weightedstationary<ROWS_A, COLS_A, COLS_B>("systolic_array");
        systolic_array->clk(clk);
        systolic_array->reset(reset);

        for (int i = 0; i < COLS_A; i++) {
            X_in[i].write(0);
            systolic_array->X_in[i](X_in[i]);
        }

        for (int i = 0; i < ROWS_A; i++)
            for (int j = 0; j < COLS_B; j++) {
                systolic_array->result[i][j](result[i][j]);
                systolic_array->shifted_out[i][j](shifted_out[i][j]);
            }

        controller = new Controller<ROWS_A, COLS_A, COLS_B>("controller", X_in);
        controller->clk(clk);
        controller->reset(reset);
        controller->input_sram = input_sram;
        controller->weight_sram = weight_sram;
        controller->output_sram = output_sram;
        controller->systolic_array = systolic_array;

        systolic_array->start_readout(start_readout);

        SC_THREAD(generate_clock);
        SC_THREAD(stimulate);
    }

    ~TestbenchWeightStationary() {
        delete systolic_array;
        delete controller;
        delete input_sram;
        delete weight_sram;
        delete output_sram;
    }
};

int sc_main(int argc, char* argv[]) {
    srand(time(NULL));
    TestbenchWeightStationary tb("tb_weight");

    sc_trace_file* tf = sc_create_vcd_trace_file("trace_weightstationary");
    tf->set_time_unit(1, SC_NS);

    sc_trace(tf, tb.clk, "clk");
    for (int i = 0; i < COLS_A; i++)
        sc_trace(tf, tb.X_in[i], "X_in_" + std::to_string(i));
    for (int i = 0; i < ROWS_A; i++)
        for (int j = 0; j < COLS_B; j++)
            sc_trace(tf, tb.shifted_out[i][j], "shifted_out_" + std::to_string(i) + "_" + std::to_string(j));

    sc_start(1500, SC_NS);
    sc_close_vcd_trace_file(tf);
    return 0;
}



















