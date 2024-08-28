#include "Vcode.h"        // Include the Verilog module's C++ header
#include "verilated.h"    // Include Verilator header
#include "verilated_vcd_c.h"  // Include Verilator VCD header for waveform dumping
#include <iostream>       // Include iostream for logging

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);  // Initialize Verilator
    Vcode* dut = new Vcode;  // Instantiate the DUT (Device Under Test)

    // Initialize inputs
    dut->clk = 0;
    dut->reset = 0;
    dut->push = 0;
    dut->pop = 0;
    dut->data_in = 0;

    // Clock generation
    int clk_period = 10;  // Clock period in time steps
    int sim_time = 0;     // Simulation time tracker

    // Reset DUT
    std::cout << "Applying reset" << std::endl;
    dut->reset = 1;
    for (int i = 0; i < 2; i++) { // Apply reset for two clock cycles
        dut->clk = !dut->clk;
        dut->eval();
        std::cout << "Cycle " << sim_time++ << ": reset = " << dut->reset << ", clk = " << dut->clk << std::endl;
    }
    dut->reset = 0;

    // Push data to FIFO
    std::cout << "Pushing data to FIFO" << std::endl;
    dut->push = 1;
    dut->data_in = 18;
    for (int i = 0; i < 4; i++) {  // Push 4 data values
        dut->clk = !dut->clk;
        dut->eval();
        if (i == 1) dut->data_in = 19;
        else if (i == 2) dut->data_in = 11;
        else if (i == 3) dut->push = 0;
        std::cout << "Cycle " << sim_time++ << ": push = " << dut->push << ", data_in = " << (int)dut->data_in << ", clk = " << dut->clk << std::endl;
    }

    // Pop data from FIFO
    std::cout << "Popping data from FIFO" << std::endl;
    dut->pop = 1;
    for (int i = 0; i < 4; i++) {  // Pop 4 data values
        dut->clk = !dut->clk;
        dut->eval();
        std::cout << "Cycle " << sim_time++ << ": pop = " << dut->pop << ", clk = " << dut->clk << std::endl;
    }
    dut->pop = 0;

    // Push more data and pop again
    std::cout << "Pushing more data to FIFO" << std::endl;
    dut->push = 1;
    dut->data_in = 18;
    for (int i = 0; i < 5; i++) {
        dut->clk = !dut->clk;
        dut->eval();
        if (i == 1) dut->data_in = 19;
        else if (i == 2) dut->data_in = 11;
        else if (i == 3) dut->data_in = 21;
        else if (i == 4) dut->push = 0;
        std::cout << "Cycle " << sim_time++ << ": push = " << dut->push << ", data_in = " << (int)dut->data_in << ", clk = " << dut->clk << std::endl;
    }

    dut->pop = 1;
    dut->clk = !dut->clk;
    dut->eval();
    std::cout << "Cycle " << sim_time++ << ": pop = " << dut->pop << ", clk = " << dut->clk << std::endl;

    dut->pop = 0;

    // Push data again
    std::cout << "Pushing data again" << std::endl;
    dut->push = 1;
    dut->data_in = 10;
    dut->clk = !dut->clk;
    dut->eval();
    std::cout << "Cycle " << sim_time++ << ": push = " << dut->push << ", data_in = " << (int)dut->data_in << ", clk = " << dut->clk << std::endl;
    dut->push = 0;

    // Run simulation for additional cycles
    std::cout << "Running simulation for additional cycles" << std::endl;
    for (int i = 0; i < 20; i++) {
        dut->clk = !dut->clk;
        dut->eval();
        std::cout << "Cycle " << sim_time++ << ": clk = " << dut->clk << std::endl ;
    }

    // End simulation
    std::cout << "Ending simulation" << std::endl;

    delete dut;
    
    return 0;
}

