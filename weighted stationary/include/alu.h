#ifndef ALU_H
#define ALU_H

#include "systemc.h"

SC_MODULE(ALU) {
    sc_in<sc_int<8>> X, Y;
    sc_out<sc_int<16>> result;
    sc_in<bool> clk;

    void multiply();

    SC_CTOR(ALU) {
        SC_CTHREAD(multiply, clk.pos());
        sensitive << X << Y;
    }
};

#endif
