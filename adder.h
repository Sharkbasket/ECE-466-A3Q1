#include "systemc.h"

template<class T>
SC_MODULE(adder) {
    sc_fifo_in<T> in1, in2;
    sc_in<bool> ready_in;
    sc_out<bool> valid_out;
    sc_out<T> data_out;
    
    SC_HAS_PROCESS(adder);
    
    adder(sc_module_name n) : sc_module(n) {
        SC_THREAD(add_process);
    }
    
    void add_process() {
        while (true) {
            data_out->write(in1->read() + in2->read());
            valid_out->write(true);
            
            // Wait for ready_in signal (indicates data was read and fifo has
            // available space for next write)
            do {
                wait(clock->posedge_event());
            } while (ready_in->read() == false);
            
            valid_out->write(false);
        }
    }
};
