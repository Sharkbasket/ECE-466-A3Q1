#include "systemc.h"

template<class T>
SC_MODULE(adder) {
    sc_fifo_in<T> in1, in2;
    sc_fifo_out<T> out;
    
    SC_HAS_PROCESS(adder);
    
    adder(sc_module_name n) : sc_module(n) {
        SC_THREAD(add_process);
    }
    
    void add_process() {
        while (true) {
            out->write(in1->read() + in2->read());
        }
    }
};
