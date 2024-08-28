#include "Vcode.h"        // Include the Verilog module's C++ header
#include "verilated.h"    // Include Verilator header
#include "verilated_vcd_c.h"  // Include Verilator VCD header for waveform dumping

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);  // Initialize Verilator
    Vcode* dut = new Vcode;  // Instantiate the DUT (Device Under Test)

    // Trace dump for waveform generation
    VerilatedVcdC* tfp = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    dut->trace(tfp, 99);
    
    tfp->open("cir_fifo_wave.vcd");

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
    dut->reset = 1;
    for (int i = 0; i < 2; i++) { // Apply reset for two clock cycles
        dut->clk = !dut->clk;
        dut->eval();
        tfp->dump(sim_time);
        sim_time += clk_period / 2;
    }
    dut->reset = 0;

    // Push data to FIFO
    dut->push = 1;
    dut->data_in = 18;
    for (int i = 0; i < 4; i++) {  // Push 4 data values
        dut->clk = !dut->clk;
        dut->eval();
        tfp->dump(sim_time);
        sim_time += clk_period / 2;
        if (i == 1) dut->data_in = 19;
        else if (i == 2) dut->data_in = 11;
        else if (i == 3) dut->push = 0;
    }

    // Pop data from FIFO
    dut->pop = 1;
    for (int i = 0; i < 4; i++) {  // Pop 4 data values
        dut->clk = !dut->clk;
        dut->eval();
        tfp->dump(sim_time);
        sim_time += clk_period / 2;
    }
    dut->pop = 0;

    // Repeat similar steps as the Verilog testbench
    // Push more data and pop again
    dut->push = 1;
    dut->data_in = 18;
    for (int i = 0; i < 5; i++) {
        dut->clk = !dut->clk;
        dut->eval();
        tfp->dump(sim_time);
        sim_time += clk_period / 2;
        if (i == 1) dut->data_in = 19;
        else if (i == 2) dut->data_in = 11;
        else if (i == 3) dut->data_in = 21;
        else if (i == 4) dut->push = 0;
    }

    dut->pop = 1;
    dut->clk = !dut->clk;
    dut->eval();
    tfp->dump(sim_time);
    sim_time += clk_period / 2;
    dut->pop = 0;

    // Push data again
    dut->push = 1;
    dut->data_in = 10;
    dut->clk = !dut->clk;
    dut->eval();
    tfp->dump(sim_time);
    sim_time += clk_period / 2;
    dut->push = 0;

    // Run simulation for additional cycles
    for (int i = 0; i < 20; i++) {
        dut->clk = !dut->clk;
        dut->eval();
        tfp->dump(sim_time);
        sim_time += clk_period / 2;
    }

    // End simulation
    tfp->close();
    delete dut;
    delete tfp;
    return 0;
}
