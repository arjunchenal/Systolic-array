
// Author : Arjun
// Date: 04.03.2025
// Description: Implementation file for Accumulator module.

#include "accumulator.h"


void Accumulator::update_acc() {
    while (true) {
        wait();
        if (reset.read()) {
            acc_out.write(0);
        } else {
            sc_int<32> sum = acc_in.read() + alu_result.read();
            acc_out.write(sum);
        }
    }
}


