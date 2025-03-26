`timescale 1ns/1ns
module Full_Adder( Ai, Bi, ADD_SUB_signal, Last_carrier, Carrier, Si );
input Ai; // 1-bit 
input Bi; // 1-bit 
input ADD_SUB_signal; // 1-bit, signal的第2 bit 
input Last_carrier; // 1-bit 
output Carrier; // 1-bit 
output Si; // sum, 1-bit 

wire xor_Bi_signal; // 1-bit  減法或加法的區分 
wire t0, t1, t2;  // 1-bit 
wire t3, t4, t5;  // 1-bit 
wire t6, t7;  // 1-bit 


//   Signal ( 6-bits)?
//   AND  : 36
//   OR   : 37
//   ADD  : 32
//   SUB  : 34
//   SLT  : 42
//parameter AND = 6'b100100; // 36 
//parameter OR  = 6'b100101; // 37 
//parameter ADD = 6'b100000; // 32 
//parameter SUB = 6'b100010; // 34 只有sub的第2 bit是1 
//parameter SLT = 6'b101010; // 42 


// 若是sub, 則1->0, 0->1 
xor( xor_Bi_signal, Bi, ADD_SUB_signal );

// Sum i bit 
xor( t1, Ai, xor_Bi_signal );
xor( t2, t1, Last_carrier );


// Ci+1 
and( t3, Ai, xor_Bi_signal );
and( t4, xor_Bi_signal, Last_carrier );
and( t5, Ai, Last_carrier );
or( t6, t3, t4 );
or( t7, t6, t5 );

assign Si = t2;
assign Carrier = t7;


endmodule
