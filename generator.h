#include "systemc.h"

template<class T>
SC_MODULE(generator) {
    sc_fifo_out<T> out;
    
    SC_HAS_PROCESS(generator);
    
    generator(sc_module_name n, T constant)
    : sc_module(n), CONSTANT(constant) {
        SC_THREAD(gen_process);
    }
    
    void gen_process() {
        while (true) {
            out->write(CONSTANT);
        }
    }
    
    T CONSTANT;
};
