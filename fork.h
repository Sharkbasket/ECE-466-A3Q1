#include "systemc.h"

template<class T>
SC_MODULE(fork)
{
    sc_fifo_in<T> in;
    sc_fifo_out<T> out1, out2;
    
    SC_HAS_PROCESS(fork);
    fork(sc_module_name n) : sc_module(n)
    {
        SC_THREAD(fork_process);
    }
    
    void fork_process()
    {
        while (true)
        {
            T input = in->read();
            out1->write(input);
            out2->write(input);
        }
    }
};
