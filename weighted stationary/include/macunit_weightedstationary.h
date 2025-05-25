#ifndef MACUNIT_WEIGHTEDSTATIONARY_H
#define MACUNIT_WEIGHTEDSTATIONARY_H

#include "systemc.h"
#include "alu.h"
#include "accumulator.h"
#include <vector>

template<int COLS_A>
SC_MODULE(MacUnit_weightedstationary) {
    sc_signal<sc_int<8>> X_sig, Y_sig;
    sc_signal<sc_int<16>> alu_out_sig;

    sc_in<sc_int<8>> X_in;
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<sc_int<32>> partial_sum_in;
    sc_out<sc_int<32>> partial_sum_out;
    sc_out<sc_int<8>> X_out;
    sc_out<sc_int<32>> result;

    ALU alu;
    Accumulator acc;

    sc_int<8> weight;

    void set_weight(sc_int<8> w) {
        weight = w;
    }

    void process() {
        while (true) {
            wait();
            sc_int<8> x = X_in.read();
            X_sig.write(x);
            Y_sig.write(weight);
            X_out.write(x);
            result.write(acc.acc_out.read());
            //partial_sum_out.write(acc.acc_out.read());
            cout << "[MacUnit] X: " << x << ", Y: " << weight 
                 << ", Partial Sum In: " << partial_sum_in.read() 
                 << ", Result: " << result.read() << endl;
        }
    }


    SC_CTOR(MacUnit_weightedstationary) : alu("alu"), acc("acc") {
        alu.X(X_sig);
        alu.Y(Y_sig);
        alu.result(alu_out_sig);
        alu.clk(clk);

        acc.alu_result(alu_out_sig);
        acc.acc_in(partial_sum_in);
        acc.acc_out(partial_sum_out);
        acc.reset(reset);
        acc.clk(clk);

        SC_CTHREAD(process, clk.pos());
    }
};

#endif