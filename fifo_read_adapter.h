template<class T>
class fifo_read_adapter : public sc_module, public sc_fifo_in_if<T> {
public:
    sc_in_clk clock;
    sc_in<T> data;
    sc_in<bool> valid;
    sc_out<bool> ready;
    
    SC_HAS_PROCESS(fifo_read_adapter);
    
    fifo_read_adapter(sc_module_name n) : sc_module(n) {
        ready->initialize(false);
    }
    
    void read(T& bucket) {
        // Caller asserts ready signal
        ready->write(true)
        
        // Caller waits until fifo asserts valid, indicating readable data
        do {
            wait(clock->posedge_event());
        }
        while (valid->read() == false);
        
        // Caller receives data and de-asserts ready signal
        bucket = data->read();
        ready->write(false);
    }
    
    // Dummy functions (not used, but definition required)
    bool nb_read(T&) {}
    const sc_event& data_written_event() const {}
    T read() {}
    int num_available() const {}
};
