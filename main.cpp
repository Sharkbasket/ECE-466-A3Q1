#include "systemc.h"
#include "adder.h"
#include "generator.h"
#include "fork_module.h"
#include "printer.h"

int sc_main(int argc, char* argv[]) {
    // Module instantiation
    adder<int> adder_i("adder_i");
    generator<int> generator_i("generator_i", 1);
    fork_module<int> fork_i("fork_i");
    printer<int> printer_i("printer_i", 10);
    hw_fifo<int> hw_fifo_i("hw_fifo_i", 1);
    
    // Heirarchical channels
    fifo_read_adapter<int> fifo_read_adapter_i("fifo_read_adapter_i");
    
    // Primitive channels
    sc_clock clock("clock", 10, SC_NS);
    sc_fifo<int> generator_out("generator_out", 1);
    sc_fifo<int> feedback("feedback", 1);
    sc_fifo<int> printer_in("printer_in", 1);
    
    // These signals are named for the module that drives them
    // e.g. adder_valid is driven by an output of the adder module
    sc_signal<bool> adder_valid;
    sc_signal<bool> fifo_valid;
    sc_signal<bool> fifo_ready;
    sc_signal<bool> adapter_ready;
    sc_signal<int> adder_data;
    sc_signal<int> fifo_data;
    
    feedback.write(40);
    
    // Interconnect modules
    generator_i.out(generator_out);
    
    adder_i.in1(feedback);
    adder_i.in2(generator_out);
    adder_i.ready_in(fifo_ready);
    adder_i.valid_out(adder_valid);
    adder_i.data_out(adder_data);
    
    hw_fifo_i.clock(clock);
    hw_fifo_i.ready_in(adapter_ready);
    hw_fifo_i.valid_in(adder_valid);
    hw_fifo_i.ready_out(fifo_ready);
    hw_fifo_i.valid_out(fifo_valid);
    hw_fifo_i.data_in(adder_data);
    hw_fifo_i.data_out(fifo_data);
    
    fifo_read_adapter_i.clock(clock);
    fifo_read_adapter_i.ready_out(adapter_ready);
    fifo_read_adapter_i.valid_in(fifo_valid);
    fifo_read_adapter_i.data_in(fifo_data);
    
    fork_i.in(fifo_read_adapter_i);
    fork_i.out1(feedback);
    fork_i.out2(printer_in);
    
    printer_i.in(printer_in);
    
    sc_start();
    
    return 0;
}
