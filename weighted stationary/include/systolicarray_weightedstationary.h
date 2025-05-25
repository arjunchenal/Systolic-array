// systolicarray_weightedstationary.h
#ifndef SYSTOLICARRAY_WEIGHTEDSTATIONARY_H
#define SYSTOLICARRAY_WEIGHTEDSTATIONARY_H

#include "systemc.h"
#include "macunit_weightedstationary.h"
#include <vector>
#include <iostream>

using namespace std;

template <int ROWS_A, int COLS_A, int COLS_B>
SC_MODULE(systolicArray_weightedstationary) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<sc_int<8>> X_in[ROWS_A];

    sc_out<sc_int<32>> result[ROWS_A][COLS_B];
    sc_out<sc_int<32>> shifted_out[ROWS_A][COLS_B];

    MacUnit_weightedstationary<COLS_A>* mac_array[COLS_A][COLS_B];

    sc_signal<sc_int<8>> X_in_buffer[ROWS_A];
    sc_signal<sc_int<8>> X_intermodule_sig[ROWS_A][COLS_B + 1];
    sc_signal<sc_int<32>> partial_sum_sig[ROWS_A][COLS_B];

    sc_signal<sc_int<32>> zero_sig[COLS_B];   

    sc_in<bool> start_readout;



    vector<vector<sc_int<8>>> A, B;
    vector<vector<sc_int<32>>> C;
    int matrix_a_rows, matrix_a_cols, matrix_b_rows, matrix_b_cols;

    void set_matrix(const vector<vector<sc_int<8>>> &matA, const vector<vector<sc_int<8>>> &matB) {
        A = matA;
        B = matB;
        matrix_a_rows = A.size();
        matrix_a_cols = A.empty() ? 0 : A[0].size();
        matrix_b_rows = B.size();
        matrix_b_cols = B.empty() ? 0 : B[0].size();

        if (matrix_a_rows != ROWS_A || matrix_a_cols != COLS_A || matrix_b_rows != COLS_A || matrix_b_cols != COLS_B) {
            cerr << "Error: Matrix dimensions do not match template parameters" << endl;
            cerr << "Expected A: " << ROWS_A << "x" << COLS_A << ", Got: " << matrix_a_rows << "x" << matrix_a_cols << endl;
            cerr << "Expected B: " << COLS_A << "x" << COLS_B << ", Got: " << matrix_b_rows << "x" << matrix_b_cols << endl;
            sc_stop();
        }

        C.resize(matrix_a_rows, vector<sc_int<32>>(matrix_b_cols, 0));
        cout << "Matrix A contents:" << endl;
        for (const auto& row : A) {
            for (const auto& val : row) {
                cout << val << "\t";
            }
            cout << endl;
        }

        cout << "Matrix B contents:" << endl;
        for (const auto& row : B) {
            for (const auto& val : row) {
                cout << val << "\t";
            }
            cout << endl;
        }


        for (int i = 0; i < COLS_A; ++i) {
            for (int j = 0; j < COLS_B; ++j) {
                if (i < matrix_a_rows && j < matrix_b_cols) {
                    mac_array[i][j]->set_weight(B[i][j]);
                    cout << "PE[" << i << "][" << j << "]: Weight = " << B[i][j] << endl;
                } else {
                    cerr << "Index out of bounds when setting weights!\n";
                }
            }
        }
    }

    // void shift_results() {
    //     int cycle_count = 0;
    //     cout << "test is "<< ROWS_A + 5 << endl;
    //     const int offset = ROWS_A + 5;
    
    //     while (true) {
    //         wait(clk.posedge_event());
    //         cycle_count++;
    
    //         for (int j = 0; j < COLS_B; ++j) {
    //             int expected_i = cycle_count - j - offset;
    //             if (expected_i >= 0 && expected_i < ROWS_A) {
    //                 sc_int<32> value = partial_sum_sig[ROWS_A - 1][j].read();
    //                 shifted_out[ROWS_A - 1][j].write(value);
    //                 cout << "Output from PE[" << ROWS_A - 1 << "][" << j << "] = " << value
    //                      << " → C[" << expected_i << "][" << j << "] at time " << sc_time_stamp() << endl;
    //                 cout << endl;
    //             }
    //         }
    
    //         if (cycle_count >= offset + ROWS_A + COLS_B - 1) {
    //             break;
    //         }
    //     }
    // }

    void set_weight(int row, int col, sc_int<8> weight) {
        if (row >= 0 && row < ROWS_A && col >= 0 && col < COLS_B) {
            mac_array[row][col]->set_weight(weight);
        }
    }

    sc_int<32> get_result(int row, int col) {
        if (row >= 0 && row < ROWS_A && col >= 0 && col < COLS_B) {
            return partial_sum_sig[row][col].read();
        } else {
            return 0;
        }
    }

    void update_input_buffer() {
        for (int i = 0; i < ROWS_A; i++)
            X_in_buffer[i].write(X_in[i].read());
    }

    SC_CTOR(systolicArray_weightedstationary) {

        for (int j = 0; j < COLS_B; j++) {
            zero_sig[j].write(0);
        }

        for (int i = 0; i < COLS_A; i++) {
            for (int j = 0; j < COLS_B; j++) {
                string name = "mac_" + to_string(i) + "_" + to_string(j);
                mac_array[i][j] = new MacUnit_weightedstationary<COLS_A>(name.c_str());
                mac_array[i][j]->clk(clk);
                mac_array[i][j]->reset(reset);

                if (j == 0)
                    mac_array[i][j]->X_in(X_in_buffer[i]);
                else
                    mac_array[i][j]->X_in(X_intermodule_sig[i][j - 1]);

                if (i == 0) {
                    mac_array[i][j]->partial_sum_in(zero_sig[j]);
                }
                else {
                    mac_array[i][j]->partial_sum_in(partial_sum_sig[i - 1][j]);
                }
                mac_array[i][j]->X_out(X_intermodule_sig[i][j]);
                mac_array[i][j]->partial_sum_out(partial_sum_sig[i][j]);
                mac_array[i][j]->result(result[i][j]);
            }
        }

       //SC_THREAD(shift_results);
        SC_METHOD(update_input_buffer);
        for (int i = 0; i < ROWS_A; i++)
            sensitive << X_in[i];
    }

    ~systolicArray_weightedstationary() {
        for (int i = 0; i < COLS_A; i++)
            for (int j = 0; j < COLS_B; j++)
                delete mac_array[i][j];
    }
};

#endif

