#include "systemc.h"

template<class T>
class hw_fifo : public sc_module {
public:
    sc_in_clk clock;
    sc_in<T> data_in;
    sc_out<T> data_out;
    sc_in<bool> ready_in, valid_in;
    sc_out<bool> ready_out, valid_out;
    
    SC_HAS_PROCESS(hw_fifo);
    
    hw_fifo(sc_module_name n, int fifo_size)
    : sc_module(n), size(fifo_size), first_item(0), item_count(0)
    {
        SC_CTHREAD(fifo_process, clock.pos());
        
        data = new T[size];
        
        ready_out->initialize(true);
        valid_out->initialize(false);
    }
    
    ~hw_fifo()
    {
        delete[] data;
    }
    
protected:
    void fifo_process()
    {
        bool readable, writeable;
        
        while (true)
        {
            readable = (item_count > 0);
            writeable = (item_count < size);
            
            if (valid_in->read() && writeable)
            {
                data[(first_item + item_count) % size] = data_in->read();
                ++item_count;
            }
            
            if (ready_in->read() && readable)
            {
                first_item = (first_item + 1) % size;
                --item_count;
            }
            
            data_out->write(data[first_item]);
            ready_out->write(item_count < size);
            valid_out->write(item_count > 0);
            
            wait();
        }
    }
    
    int size, first_item, item_count;
    T* data;
};
