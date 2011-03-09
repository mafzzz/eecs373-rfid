//////////////////////////////////////////////////////////////////////
// Created by Actel SmartDesign Wed Mar 09 16:51:27 2011
// Testbench Template
// This is a basic testbench that instantiates your design with basic 
// clock and reset pins connected.  If your design has special
// clock/reset or testbench driver requirements then you should 
// copy this file and modify it. 
//////////////////////////////////////////////////////////////////////

`timescale 1ns/100ps

module testbench;

parameter SYSCLK_PERIOD = 100; // 10MHz

reg SYSCLK;
reg NSYSRESET;

initial
begin
    SYSCLK = 1'b0;
    NSYSRESET = 1'b0;
end

//////////////////////////////////////////////////////////////////////
// Reset Pulse
//////////////////////////////////////////////////////////////////////
initial
begin
    #(SYSCLK_PERIOD * 10 )
        NSYSRESET = 1'b1;
end


//////////////////////////////////////////////////////////////////////
// 10MHz Clock Driver
//////////////////////////////////////////////////////////////////////
always @(SYSCLK)
    #(SYSCLK_PERIOD / 2.0) SYSCLK <= !SYSCLK;


//////////////////////////////////////////////////////////////////////
// Instantiate Unit Under Test:  mss_core
//////////////////////////////////////////////////////////////////////
mss_core mss_core_0 (
    // Inputs
    .UART_0_RXD({1{1'b0}}),
    .MSS_RESET_N(NSYSRESET),

    // Outputs
    .UART_0_TXD( ),
    .EMC_AB( ),
    .EMC_BYTEN( ),
    .EMC_CS0_N( ),
    .EMC_CS1_N( ),
    .EMC_OEN0_N( ),
    .EMC_OEN1_N( ),
    .EMC_CLK( ),
    .EMC_RW_N( ),

    // Inouts
    .EMC_DB( )

);

endmodule

