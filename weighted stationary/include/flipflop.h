// ====================================
// flipflop.h - delay element for input skewing
// ====================================

#ifndef FLIPFLOP_H
#define FLIPFLOP_H

#include "systemc.h"
#include <deque>

SC_MODULE(FlipFlop) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<sc_int<8>> data_in;
    sc_out<sc_int<8>> data_out;

    int delay_cycles;
    std::deque<sc_int<8>> pipeline;

    void process() {
        if (reset.read() == true) {
            pipeline.clear();
            for (int i = 0; i < delay_cycles; i++)
                pipeline.push_back(0);
            data_out.write(0);
        } else {
            pipeline.push_back(data_in.read());
            sc_int<8> output_value = pipeline.front();
            pipeline.pop_front();
            data_out.write(output_value);

            cout << "[FlipFlop] Row delay=" << delay_cycles 
                 << " Input=" << data_in.read() 
                 << " Output=" << output_value << endl;
        }
    }

    FlipFlop(sc_module_name name, int delay) : sc_module(name), delay_cycles(delay) {
        SC_METHOD(process);
        sensitive << clk.pos();
        dont_initialize();

        for (int i = 0; i < delay_cycles; i++)
            pipeline.push_back(0);
    }
};

#endif
