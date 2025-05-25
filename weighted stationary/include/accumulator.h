#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include "systemc.h"

SC_MODULE(Accumulator) {
    sc_in<sc_int<16>> alu_result;
    sc_in<sc_int<32>> acc_in; // NEW: partial sum input
    sc_in<bool> clk;
    sc_out<sc_int<32>> acc_out;
    sc_in<bool> reset;
    sc_int<32> accumulator; 
    

    void update_acc(); 

    SC_CTOR(Accumulator) {
        accumulator = 0;
        SC_CTHREAD(update_acc, clk.pos());

    }
};

#endif 