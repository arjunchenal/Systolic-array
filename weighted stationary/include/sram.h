
#ifndef SRAM_H
#define SRAM_H

#include "systemc.h"

template<int ROWS_A, int COLS_A, int COLS_B>
SC_MODULE(SRAM) {

    sc_int<32> input_memory[COLS_A][ROWS_A];
    sc_int<32> weight_memory[COLS_A][COLS_B];
    sc_int<32> output_memory[ROWS_A][COLS_B];

    void write_input(int row, int col, sc_int<32> data) {
        if (col >= 0 && row < COLS_A && row >= 0 && col < ROWS_A) {
            input_memory[row][col] = data;
            cout << "[SRAM] Input write: A[" << row << "][" << col << "] = " << data << endl;
        } else {
            cerr << "[SRAM] Input write error: Index out of bounds" << endl;
        }
    }

    sc_int<32> read_input(int row, int col) {
        if (col >= 0 && row < COLS_A && row >= 0 && col < ROWS_A) {
            cout << "[SRAM] Input read: A[" << row << "][" << col << "] = " << input_memory[row][col] << endl;
            return input_memory[row][col];
        } else {
            cerr << "[SRAM] Input read error: Index out of bounds" << endl;
            return 0;
        }
    }

    void write_weight(int row, int col, sc_int<32> data) {
        if (row >= 0 && row < ROWS_A && col >= 0 && col < COLS_B) {
            weight_memory[col][row] = data;
            cout << "[SRAM] Weight write: B[" << row << "][" << col << "] = " << data << endl;
        } else {
            cerr << "[SRAM] Weight write error: Index out of bounds" << endl;
        }
    }

    sc_int<32> read_weight(int row, int col) {
        if (row >= 0 && row < ROWS_A && col >= 0 && col < COLS_B) {
            cout << "[SRAM] Weight read: B[" << row << "][" << col << "] = " << weight_memory[col][row] << endl;
            return weight_memory[col][row];
        } else {
            cerr << "[SRAM] Weight read error: Index out of bounds" << endl;
            return 0;
        }
    }

    void write_output(int row, int col, sc_int<32> data) {
        if (row >= 0 && row < ROWS_A && col >= 0 && col < COLS_B) {
            output_memory[row][col] = data;
            cout << "[SRAM] Output write: C[" << row << "][" << col << "] = " << data << endl;
        } else {
            cerr << "[SRAM] Output write error: Index out of bounds" << endl;
        }
    }

    sc_int<32> read_output(int row, int col) {
        if (row >= 0 && row < ROWS_A && col >= 0 && col < COLS_B) {
            cout << "[SRAM] Output read: C[" << row << "][" << col << "] = " << output_memory[row][col] << endl;
            return output_memory[row][col];
        } else {
            cerr << "[SRAM] Output read error: Index out of bounds" << endl;
            return 0;
        }
    }

    SC_CTOR(SRAM) {
        for (int i = 0; i < COLS_A; ++i) {
            for (int j = 0; j < ROWS_A; ++j) {
                input_memory[i][j] = 0;
            }
        }
        for (int i = 0; i < COLS_A; ++i) {
            for (int j = 0; j < COLS_B; ++j) {
                weight_memory[i][j] = 0;
            }
        }
        for (int i = 0; i < ROWS_A; ++i) {
            for (int j = 0; j < COLS_B; ++j) {
                output_memory[i][j] = 0;
            }
        }
    }
};

#endif






