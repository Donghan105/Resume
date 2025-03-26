`timescale 1ns/1ns
module MUX_2_to_1( Shift_or_not, original, shift_data, temp );
input Shift_or_not; // 1-bit 
input original; // 1-bit 
input shift_data; // 1-bit 
output temp; // 1-bit 

assign temp = Shift_or_not ? shift_data : original;


endmodule