#include "systemc.h"

template<class T>
class fifo_read_adapter : public sc_module, public sc_fifo_in_if<T> {
public:
    sc_in_clk clock;
    sc_out<bool> ready_out;
    sc_in<bool> valid_in;
    sc_in<T> data_in;
    
    SC_HAS_PROCESS(fifo_read_adapter);
    
    fifo_read_adapter(sc_module_name n) : sc_module(n) {
        ready_out.initialize(false);
    }
    
    T read() {
        // Caller asserts ready_out signal
        ready_out->write(true);
        
        // Caller waits until fifo asserts valid, indicating readable data
        do {
            wait(clock->posedge_event());
        } while (valid_in->read() == false);
        
        // Caller receives data and de-asserts ready_out signal
        ready_out->write(false);
        return data_in->read();
    }
    
    // Dummy functions (not used, but definition required)
    bool nb_read(T&) {}
    const sc_event& data_written_event() const {}
    void read(T&) {}
    int num_available() const {}
};
