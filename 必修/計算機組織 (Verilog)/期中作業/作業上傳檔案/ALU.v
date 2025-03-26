`timescale 1ns/1ns
module ALU( dataA, dataB, Signal, dataOut, reset );
input reset ;
input [31:0] dataA ;
input [31:0] dataB ;
input [5:0] Signal ;

wire [31:0] data ;
wire [31:0] andOut ;
wire [31:0] orOut ;
wire [31:0] result;
output [31:0] dataOut ;

//   Signal ( 6-bits)?
//   AND  : 36
//   OR   : 37
//   ADD  : 32
//   SUB  : 34
//   SLT  : 42

wire [31:0] temp ;

parameter AND = 6'b100100; // 36 
parameter OR  = 6'b100101; // 37 
parameter ADD = 6'b100000; // 32 
parameter SUB = 6'b100010; // 34 只有sub的第2 bit是1 
parameter SLT = 6'b101010; // 42 
/*
定義各種訊號
*/

Full_Adder bit_1( .Ai( dataA[0] ), .Bi( dataB[0] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( Signal[1] ), .Carrier( temp[0] ), .Si( data[0]) );
Full_Adder bit_2( .Ai( dataA[1] ), .Bi( dataB[1] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[0] ), .Carrier( temp[1] ), .Si( data[1] ) );
Full_Adder bit_3( .Ai( dataA[2] ), .Bi( dataB[2] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[1] ), .Carrier( temp[2] ), .Si( data[2] ) );
Full_Adder bit_4( .Ai( dataA[3] ), .Bi( dataB[3] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[2] ), .Carrier( temp[3] ), .Si( data[3] ) );
Full_Adder bit_5( .Ai( dataA[4] ), .Bi( dataB[4] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[3] ), .Carrier( temp[4] ), .Si( data[4] ) );
Full_Adder bit_6( .Ai( dataA[5] ), .Bi( dataB[5] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[4] ), .Carrier( temp[5] ), .Si( data[5] ) );
Full_Adder bit_7( .Ai( dataA[6] ), .Bi( dataB[6] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[5] ), .Carrier( temp[6] ), .Si( data[6] ) );
Full_Adder bit_8( .Ai( dataA[7] ), .Bi( dataB[7] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[6] ), .Carrier( temp[7] ), .Si( data[7] ) );
Full_Adder bit_9( .Ai( dataA[8] ), .Bi( dataB[8] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[7] ), .Carrier( temp[8] ), .Si( data[8] ) );
Full_Adder bit_10( .Ai( dataA[9] ), .Bi( dataB[9] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[8] ), .Carrier( temp[9] ), .Si( data[9] ) );
Full_Adder bit_11( .Ai( dataA[10] ), .Bi( dataB[10] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[9] ), .Carrier( temp[10] ), .Si( data[10] ) );
Full_Adder bit_12( .Ai( dataA[11] ), .Bi( dataB[11] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[10] ), .Carrier( temp[11] ), .Si( data[11] ) );
Full_Adder bit_13( .Ai( dataA[12] ), .Bi( dataB[12] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[11] ), .Carrier( temp[12] ), .Si( data[12] ) );
Full_Adder bit_14( .Ai( dataA[13] ), .Bi( dataB[13] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[12] ), .Carrier( temp[13] ), .Si( data[13] ) );
Full_Adder bit_15( .Ai( dataA[14] ), .Bi( dataB[14] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[13] ), .Carrier( temp[14] ), .Si( data[14] ) );
Full_Adder bit_16( .Ai( dataA[15] ), .Bi( dataB[15] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[14] ), .Carrier( temp[15] ), .Si( data[15] ) );
Full_Adder bit_17( .Ai( dataA[16] ), .Bi( dataB[16] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[15] ), .Carrier( temp[16] ), .Si( data[16] ) );
Full_Adder bit_18( .Ai( dataA[17] ), .Bi( dataB[17] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[16] ), .Carrier( temp[17] ), .Si( data[17] ) );
Full_Adder bit_19( .Ai( dataA[18] ), .Bi( dataB[18] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[17] ), .Carrier( temp[18] ), .Si( data[18] ) );
Full_Adder bit_20( .Ai( dataA[19] ), .Bi( dataB[19] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[18] ), .Carrier( temp[19] ), .Si( data[19] ) );
Full_Adder bit_21( .Ai( dataA[20] ), .Bi( dataB[20] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[19] ), .Carrier( temp[20] ), .Si( data[20] ) );
Full_Adder bit_22( .Ai( dataA[21] ), .Bi( dataB[21] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[20] ), .Carrier( temp[21] ), .Si( data[21] ) );
Full_Adder bit_23( .Ai( dataA[22] ), .Bi( dataB[22] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[21] ), .Carrier( temp[22] ), .Si( data[22] ) );
Full_Adder bit_24( .Ai( dataA[23] ), .Bi( dataB[23] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[22] ), .Carrier( temp[23] ), .Si( data[23] ) );
Full_Adder bit_25( .Ai( dataA[24] ), .Bi( dataB[24] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[23] ), .Carrier( temp[24] ), .Si( data[24] ) );
Full_Adder bit_26( .Ai( dataA[25] ), .Bi( dataB[25] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[24] ), .Carrier( temp[25] ), .Si( data[25] ) );
Full_Adder bit_27( .Ai( dataA[26] ), .Bi( dataB[26] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[25] ), .Carrier( temp[26] ), .Si( data[26] ) );
Full_Adder bit_28( .Ai( dataA[27] ), .Bi( dataB[27] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[26] ), .Carrier( temp[27] ), .Si( data[27] ) );
Full_Adder bit_29( .Ai( dataA[28] ), .Bi( dataB[28] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[27] ), .Carrier( temp[28] ), .Si( data[28] ) );
Full_Adder bit_30( .Ai( dataA[29] ), .Bi( dataB[29] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[28] ), .Carrier( temp[29] ), .Si( data[29] ) );
Full_Adder bit_31( .Ai( dataA[30] ), .Bi( dataB[30] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[29] ), .Carrier( temp[30] ), .Si( data[30] ) );
Full_Adder bit_32( .Ai( dataA[31] ), .Bi( dataB[31] ), .ADD_SUB_signal( Signal[1] ), .Last_carrier( temp[30] ), .Carrier( temp[31] ), .Si( data[31] ) );

AND_32 and_bit_1( .Ai( dataA[0] ), .Bi( dataB[0] ), .Si( andOut[0]) );
AND_32 and_bit_2( .Ai( dataA[1] ), .Bi( dataB[1] ), .Si( andOut[1]) );
AND_32 and_bit_3( .Ai( dataA[2] ), .Bi( dataB[2] ), .Si( andOut[2]) );
AND_32 and_bit_4( .Ai( dataA[3] ), .Bi( dataB[3] ), .Si( andOut[3]) );
AND_32 and_bit_5( .Ai( dataA[4] ), .Bi( dataB[4] ), .Si( andOut[4]) );
AND_32 and_bit_6( .Ai( dataA[5] ), .Bi( dataB[5] ), .Si( andOut[5]) );
AND_32 and_bit_7( .Ai( dataA[6] ), .Bi( dataB[6] ), .Si( andOut[6]) );
AND_32 and_bit_8( .Ai( dataA[7] ), .Bi( dataB[7] ), .Si( andOut[7]) );
AND_32 and_bit_9( .Ai( dataA[8] ), .Bi( dataB[8] ), .Si( andOut[8]) );
AND_32 and_bit_10( .Ai( dataA[9] ), .Bi( dataB[9] ), .Si( andOut[9]) );
AND_32 and_bit_11( .Ai( dataA[10] ), .Bi( dataB[10] ), .Si( andOut[10]) );
AND_32 and_bit_12( .Ai( dataA[11] ), .Bi( dataB[11] ), .Si( andOut[11]) );
AND_32 and_bit_13( .Ai( dataA[12] ), .Bi( dataB[12] ), .Si( andOut[12]) );
AND_32 and_bit_14( .Ai( dataA[13] ), .Bi( dataB[13] ), .Si( andOut[13]) );
AND_32 and_bit_15( .Ai( dataA[14] ), .Bi( dataB[14] ), .Si( andOut[14]) );
AND_32 and_bit_16( .Ai( dataA[15] ), .Bi( dataB[15] ), .Si( andOut[15]) );
AND_32 and_bit_17( .Ai( dataA[16] ), .Bi( dataB[16] ), .Si( andOut[16]) );
AND_32 and_bit_18( .Ai( dataA[17] ), .Bi( dataB[17] ), .Si( andOut[17]) );
AND_32 and_bit_19( .Ai( dataA[18] ), .Bi( dataB[18] ), .Si( andOut[18]) );
AND_32 and_bit_20( .Ai( dataA[19] ), .Bi( dataB[19] ), .Si( andOut[19]) );
AND_32 and_bit_21( .Ai( dataA[20] ), .Bi( dataB[20] ), .Si( andOut[20]) );
AND_32 and_bit_22( .Ai( dataA[21] ), .Bi( dataB[21] ), .Si( andOut[21]) );
AND_32 and_bit_23( .Ai( dataA[22] ), .Bi( dataB[22] ), .Si( andOut[22]) );
AND_32 and_bit_24( .Ai( dataA[23] ), .Bi( dataB[23] ), .Si( andOut[23]) );
AND_32 and_bit_25( .Ai( dataA[24] ), .Bi( dataB[24] ), .Si( andOut[24]) );
AND_32 and_bit_26( .Ai( dataA[25] ), .Bi( dataB[25] ), .Si( andOut[25]) );
AND_32 and_bit_27( .Ai( dataA[26] ), .Bi( dataB[26] ), .Si( andOut[26]) );
AND_32 and_bit_28( .Ai( dataA[27] ), .Bi( dataB[27] ), .Si( andOut[27]) );
AND_32 and_bit_29( .Ai( dataA[28] ), .Bi( dataB[28] ), .Si( andOut[28]) );
AND_32 and_bit_30( .Ai( dataA[29] ), .Bi( dataB[29] ), .Si( andOut[29]) );
AND_32 and_bit_31( .Ai( dataA[30] ), .Bi( dataB[30] ), .Si( andOut[30]) );
AND_32 and_bit_32( .Ai( dataA[21] ), .Bi( dataB[31] ), .Si( andOut[31]) );

OR_32 or_bit_1( .Ai( dataA[0] ), .Bi( dataB[0] ), .Si( orOut[0]) );
OR_32 or_bit_2( .Ai( dataA[1] ), .Bi( dataB[1] ), .Si( orOut[1]) );
OR_32 or_bit_3( .Ai( dataA[2] ), .Bi( dataB[2] ), .Si( orOut[2]) );
OR_32 or_bit_4( .Ai( dataA[3] ), .Bi( dataB[3] ), .Si( orOut[3]) );
OR_32 or_bit_5( .Ai( dataA[4] ), .Bi( dataB[4] ), .Si( orOut[4]) );
OR_32 or_bit_6( .Ai( dataA[5] ), .Bi( dataB[5] ), .Si( orOut[5]) );
OR_32 or_bit_7( .Ai( dataA[6] ), .Bi( dataB[6] ), .Si( orOut[6]) );
OR_32 or_bit_8( .Ai( dataA[7] ), .Bi( dataB[7] ), .Si( orOut[7]) );
OR_32 or_bit_9( .Ai( dataA[8] ), .Bi( dataB[8] ), .Si( orOut[8]) );
OR_32 or_bit_10( .Ai( dataA[9] ), .Bi( dataB[9] ), .Si( orOut[9]) );
OR_32 or_bit_11( .Ai( dataA[10] ), .Bi( dataB[10] ), .Si( orOut[10]) );
OR_32 or_bit_12( .Ai( dataA[11] ), .Bi( dataB[11] ), .Si( orOut[11]) );
OR_32 or_bit_13( .Ai( dataA[12] ), .Bi( dataB[12] ), .Si( orOut[12]) );
OR_32 or_bit_14( .Ai( dataA[13] ), .Bi( dataB[13] ), .Si( orOut[13]) );
OR_32 or_bit_15( .Ai( dataA[14] ), .Bi( dataB[14] ), .Si( orOut[14]) );
OR_32 or_bit_16( .Ai( dataA[15] ), .Bi( dataB[15] ), .Si( orOut[15]) );
OR_32 or_bit_17( .Ai( dataA[16] ), .Bi( dataB[16] ), .Si( orOut[16]) );
OR_32 or_bit_18( .Ai( dataA[17] ), .Bi( dataB[17] ), .Si( orOut[17]) );
OR_32 or_bit_19( .Ai( dataA[18] ), .Bi( dataB[18] ), .Si( orOut[18]) );
OR_32 or_bit_20( .Ai( dataA[19] ), .Bi( dataB[19] ), .Si( orOut[19]) );
OR_32 or_bit_21( .Ai( dataA[20] ), .Bi( dataB[20] ), .Si( orOut[20]) );
OR_32 or_bit_22( .Ai( dataA[21] ), .Bi( dataB[21] ), .Si( orOut[21]) );
OR_32 or_bit_23( .Ai( dataA[22] ), .Bi( dataB[22] ), .Si( orOut[22]) );
OR_32 or_bit_24( .Ai( dataA[23] ), .Bi( dataB[23] ), .Si( orOut[23]) );
OR_32 or_bit_25( .Ai( dataA[24] ), .Bi( dataB[24] ), .Si( orOut[24]) );
OR_32 or_bit_26( .Ai( dataA[25] ), .Bi( dataB[25] ), .Si( orOut[25]) );
OR_32 or_bit_27( .Ai( dataA[26] ), .Bi( dataB[26] ), .Si( orOut[26]) );
OR_32 or_bit_28( .Ai( dataA[27] ), .Bi( dataB[27] ), .Si( orOut[27]) );
OR_32 or_bit_29( .Ai( dataA[28] ), .Bi( dataB[28] ), .Si( orOut[28]) );
OR_32 or_bit_30( .Ai( dataA[29] ), .Bi( dataB[29] ), .Si( orOut[29]) );
OR_32 or_bit_31( .Ai( dataA[30] ), .Bi( dataB[30] ), .Si( orOut[30]) );
OR_32 or_bit_32( .Ai( dataA[21] ), .Bi( dataB[31] ), .Si( orOut[31]) );

ALU_mode am( .Signal( Signal ), .data( data ), .andOut( andOut ), .orOut( orOut ), .Out( dataOut ) );

endmodule