#include "alu.h"

void ALU::multiply() {
    while(true) {
        wait();
        sc_int<16> product = X.read() * Y.read();
        result.write(product);
        //cout << "ALU: X = " << X.read() << ", Y = " << Y.read() << ", Product = " << product << endl;
    }

}
