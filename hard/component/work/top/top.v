`timescale 1 ns/100 ps
// Version: 9.0 SP3A 9.0.3.7


module top(
       UART_0_TXD,
       UART_0_RXD,
       MSS_RESET_N
    );
output UART_0_TXD;
input  UART_0_RXD;
input  MSS_RESET_N;

    wire GND_net, VCC_net;
    
    VCC VCC (.Y(VCC_net));
    mss_core mss_core_0 (.UART_0_TXD(UART_0_TXD), .UART_0_RXD(
        UART_0_RXD), .MSS_RESET_N(MSS_RESET_N));
    GND GND (.Y(GND_net));
    
endmodule
