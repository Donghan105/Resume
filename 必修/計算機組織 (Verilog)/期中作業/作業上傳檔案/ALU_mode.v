`timescale 1ns/1ns
module ALU_mode( Signal, data, andOut, orOut, Out );
input [5:0] Signal;
input [31:0] data;
input [31:0] andOut;
input [31:0] orOut;
output [31:0] Out;

// 是SLT的話檢查dataout[31],若是1則為true, else false 
// 不是SLT照常輸出 
assign Out = Signal[2] ? ( Signal[0] ? orOut : andOut ) : ( Signal[3] ? ( data[31] ? 1 : 0 ) : data );

endmodule