`timescale 1ns/1ns
module AND_32( Ai, Bi, Si );
input Ai; // 1-bit 
input Bi; // 1-bit 
output Si; // sum, 1-bit 

wire t0;

and( t0, Ai, Bi );
assign Si = t0;

endmodule