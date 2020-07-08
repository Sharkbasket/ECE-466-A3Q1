#include "systemc.h"
#include "adder.h"
#include "generator.h"
#include "fork_module.h"
#include "printer.h"

int sc_main(int argc, char* argv[]) {
    adder<int> adder_i("adder_i");
    generator<int> generator_i("generator_i", 1);
    fork_module<int> fork_i("fork_i");
    printer<int> printer_i("printer_i", 10);
    
    sc_fifo<int> adder_out("adder_out", 1); // To be replaced by hw_fifo
    sc_fifo<int> generator_out("generator_out", 1);
    sc_fifo<int> feedback("feedback", 1);
    sc_fifo<int> printer_in("printer_in", 1);
    
    feedback.write(40);
    
    adder_i.in1(feedback);
    adder_i.in2(generator_out);
    adder_i.out(adder_out);
    
    generator_i.out(generator_out);
    
    fork_i.in(adder_out);
    fork_i.out1(feedback);
    fork_i.out2(printer_in);
    
    printer_i.in(printer_in);
    
    sc_start();
    
    return 0;
}
