#include "systemc.h"
#include <iostream>

template<class T>
SC_MODULE(printer) {
    sc_fifo_in<T> in;
    
    SC_HAS_PROCESS(printer);
    
    printer(sc_module_name n, int iterations)
    : sc_module(n), ITERATIONS(iterations) {
        SC_THREAD(print_process);
    }
    
    void print_process() {
        for (int i = 0; i < ITERATIONS; i++) {
            T input = in->read();
            std::cout << name() << " " << input << endl;
        }
        
        sc_stop();
    }
    
    int ITERATIONS;
};
