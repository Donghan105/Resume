`timescale 1ns/1ns
module Shifter( dataA, dataB, Signal, dataOut, reset );
input reset ;
input [31:0] dataA ;
input [31:0] dataB ;
input [5:0] Signal ;
output [31:0] dataOut ;


wire [31:0] temp [4:0] ;

parameter SLL = 6'b000000;

/*
=====================================================
下面為模擬範例，程式撰寫請遵照老師上課說明的方法來寫
=====================================================
*/
// 第一層 
MUX_2_to_1 level1_1( .Shift_or_not( dataB[0] ), .original( dataA[0] ), .shift_data( 1'b0 ), .temp( temp[0][0] ) );
MUX_2_to_1 level1_2( .Shift_or_not( dataB[0] ), .original( dataA[1] ), .shift_data( dataA[0] ), .temp( temp[0][1] ) );
MUX_2_to_1 level1_3( .Shift_or_not( dataB[0] ), .original( dataA[2] ), .shift_data( dataA[1] ), .temp( temp[0][2] ) );
MUX_2_to_1 level1_4( .Shift_or_not( dataB[0] ), .original( dataA[3] ), .shift_data( dataA[2] ), .temp( temp[0][3] ) );
MUX_2_to_1 level1_5( .Shift_or_not( dataB[0] ), .original( dataA[4] ), .shift_data( dataA[3] ), .temp( temp[0][4] ) );
MUX_2_to_1 level1_6( .Shift_or_not( dataB[0] ), .original( dataA[5] ), .shift_data( dataA[4] ), .temp( temp[0][5] ) );
MUX_2_to_1 level1_7( .Shift_or_not( dataB[0] ), .original( dataA[6] ), .shift_data( dataA[5] ), .temp( temp[0][6] ) );
MUX_2_to_1 level1_8( .Shift_or_not( dataB[0] ), .original( dataA[7] ), .shift_data( dataA[6] ), .temp( temp[0][7] ) );
MUX_2_to_1 level1_9( .Shift_or_not( dataB[0] ), .original( dataA[8] ), .shift_data( dataA[7] ), .temp( temp[0][8] ) );
MUX_2_to_1 level1_10( .Shift_or_not( dataB[0] ), .original( dataA[9] ), .shift_data( dataA[8] ), .temp( temp[0][9] ) );
MUX_2_to_1 level1_11( .Shift_or_not( dataB[0] ), .original( dataA[10] ), .shift_data( dataA[9] ), .temp( temp[0][10] ) );
MUX_2_to_1 level1_12( .Shift_or_not( dataB[0] ), .original( dataA[11] ), .shift_data( dataA[10] ), .temp( temp[0][11] ) );
MUX_2_to_1 level1_13( .Shift_or_not( dataB[0] ), .original( dataA[12] ), .shift_data( dataA[11] ), .temp( temp[0][12] ) );
MUX_2_to_1 level1_14( .Shift_or_not( dataB[0] ), .original( dataA[13] ), .shift_data( dataA[12] ), .temp( temp[0][13] ) );
MUX_2_to_1 level1_15( .Shift_or_not( dataB[0] ), .original( dataA[14] ), .shift_data( dataA[13] ), .temp( temp[0][14] ) );
MUX_2_to_1 level1_16( .Shift_or_not( dataB[0] ), .original( dataA[15] ), .shift_data( dataA[14] ), .temp( temp[0][15] ) );
MUX_2_to_1 level1_17( .Shift_or_not( dataB[0] ), .original( dataA[16] ), .shift_data( dataA[15] ), .temp( temp[0][16] ) );
MUX_2_to_1 level1_18( .Shift_or_not( dataB[0] ), .original( dataA[17] ), .shift_data( dataA[16] ), .temp( temp[0][17] ) );
MUX_2_to_1 level1_19( .Shift_or_not( dataB[0] ), .original( dataA[18] ), .shift_data( dataA[17] ), .temp( temp[0][18] ) );
MUX_2_to_1 level1_20( .Shift_or_not( dataB[0] ), .original( dataA[19] ), .shift_data( dataA[18] ), .temp( temp[0][19] ) );
MUX_2_to_1 level1_21( .Shift_or_not( dataB[0] ), .original( dataA[20] ), .shift_data( dataA[19] ), .temp( temp[0][20] ) );
MUX_2_to_1 level1_22( .Shift_or_not( dataB[0] ), .original( dataA[21] ), .shift_data( dataA[20] ), .temp( temp[0][21] ) );
MUX_2_to_1 level1_23( .Shift_or_not( dataB[0] ), .original( dataA[22] ), .shift_data( dataA[21] ), .temp( temp[0][22] ) );
MUX_2_to_1 level1_24( .Shift_or_not( dataB[0] ), .original( dataA[23] ), .shift_data( dataA[22] ), .temp( temp[0][23] ) );
MUX_2_to_1 level1_25( .Shift_or_not( dataB[0] ), .original( dataA[24] ), .shift_data( dataA[23] ), .temp( temp[0][24] ) );
MUX_2_to_1 level1_26( .Shift_or_not( dataB[0] ), .original( dataA[25] ), .shift_data( dataA[24] ), .temp( temp[0][25] ) );
MUX_2_to_1 level1_27( .Shift_or_not( dataB[0] ), .original( dataA[26] ), .shift_data( dataA[25] ), .temp( temp[0][26] ) );
MUX_2_to_1 level1_28( .Shift_or_not( dataB[0] ), .original( dataA[27] ), .shift_data( dataA[26] ), .temp( temp[0][27] ) );
MUX_2_to_1 level1_29( .Shift_or_not( dataB[0] ), .original( dataA[28] ), .shift_data( dataA[27] ), .temp( temp[0][28] ) );
MUX_2_to_1 level1_30( .Shift_or_not( dataB[0] ), .original( dataA[29] ), .shift_data( dataA[28] ), .temp( temp[0][29] ) );
MUX_2_to_1 level1_31( .Shift_or_not( dataB[0] ), .original( dataA[30] ), .shift_data( dataA[29] ), .temp( temp[0][30] ) );
MUX_2_to_1 level1_32( .Shift_or_not( dataB[0] ), .original( dataA[31] ), .shift_data( dataA[30] ), .temp( temp[0][31] ) );

// 第二層 
MUX_2_to_1 level2_1( .Shift_or_not( dataB[1] ), .original( temp[0][0] ), .shift_data( 1'b0 ), .temp( temp[1][0] ) );
MUX_2_to_1 level2_2( .Shift_or_not( dataB[1] ), .original( temp[0][1] ), .shift_data( 1'b0 ), .temp( temp[1][1] ) );
MUX_2_to_1 level2_3( .Shift_or_not( dataB[1] ), .original( temp[0][2] ), .shift_data( temp[0][0] ), .temp( temp[1][2] ) );
MUX_2_to_1 level2_4( .Shift_or_not( dataB[1] ), .original( temp[0][3] ), .shift_data( temp[0][1] ), .temp( temp[1][3] ) );
MUX_2_to_1 level2_5( .Shift_or_not( dataB[1] ), .original( temp[0][4] ), .shift_data( temp[0][2] ), .temp( temp[1][4] ) );
MUX_2_to_1 level2_6( .Shift_or_not( dataB[1] ), .original( temp[0][5] ), .shift_data( temp[0][3] ), .temp( temp[1][5] ) );
MUX_2_to_1 level2_7( .Shift_or_not( dataB[1] ), .original( temp[0][6] ), .shift_data( temp[0][4] ), .temp( temp[1][6] ) );
MUX_2_to_1 level2_8( .Shift_or_not( dataB[1] ), .original( temp[0][7] ), .shift_data( temp[0][5] ), .temp( temp[1][7] ) );
MUX_2_to_1 level2_9( .Shift_or_not( dataB[1] ), .original( temp[0][8] ), .shift_data( temp[0][6] ), .temp( temp[1][8] ) );
MUX_2_to_1 level2_10( .Shift_or_not( dataB[1] ), .original( temp[0][9] ), .shift_data( temp[0][7] ), .temp( temp[1][9] ) );
MUX_2_to_1 level2_11( .Shift_or_not( dataB[1] ), .original( temp[0][10] ), .shift_data( temp[0][8] ), .temp( temp[1][10] ) );
MUX_2_to_1 level2_12( .Shift_or_not( dataB[1] ), .original( temp[0][11] ), .shift_data( temp[0][9] ), .temp( temp[1][11] ) );
MUX_2_to_1 level2_13( .Shift_or_not( dataB[1] ), .original( temp[0][12] ), .shift_data( temp[0][10] ), .temp( temp[1][12] ) );
MUX_2_to_1 level2_14( .Shift_or_not( dataB[1] ), .original( temp[0][13] ), .shift_data( temp[0][11] ), .temp( temp[1][13] ) );
MUX_2_to_1 level2_15( .Shift_or_not( dataB[1] ), .original( temp[0][14] ), .shift_data( temp[0][12] ), .temp( temp[1][14] ) );
MUX_2_to_1 level2_16( .Shift_or_not( dataB[1] ), .original( temp[0][15] ), .shift_data( temp[0][13] ), .temp( temp[1][15] ) );
MUX_2_to_1 level2_17( .Shift_or_not( dataB[1] ), .original( temp[0][16] ), .shift_data( temp[0][14] ), .temp( temp[1][16] ) );
MUX_2_to_1 level2_18( .Shift_or_not( dataB[1] ), .original( temp[0][17] ), .shift_data( temp[0][15] ), .temp( temp[1][17] ) );
MUX_2_to_1 level2_19( .Shift_or_not( dataB[1] ), .original( temp[0][18] ), .shift_data( temp[0][16] ), .temp( temp[1][18] ) );
MUX_2_to_1 level2_20( .Shift_or_not( dataB[1] ), .original( temp[0][19] ), .shift_data( temp[0][17] ), .temp( temp[1][19] ) );
MUX_2_to_1 level2_21( .Shift_or_not( dataB[1] ), .original( temp[0][20] ), .shift_data( temp[0][18] ), .temp( temp[1][20] ) );
MUX_2_to_1 level2_22( .Shift_or_not( dataB[1] ), .original( temp[0][21] ), .shift_data( temp[0][19] ), .temp( temp[1][21] ) );
MUX_2_to_1 level2_23( .Shift_or_not( dataB[1] ), .original( temp[0][22] ), .shift_data( temp[0][20] ), .temp( temp[1][22] ) );
MUX_2_to_1 level2_24( .Shift_or_not( dataB[1] ), .original( temp[0][23] ), .shift_data( temp[0][21] ), .temp( temp[1][23] ) );
MUX_2_to_1 level2_25( .Shift_or_not( dataB[1] ), .original( temp[0][24] ), .shift_data( temp[0][22] ), .temp( temp[1][24] ) );
MUX_2_to_1 level2_26( .Shift_or_not( dataB[1] ), .original( temp[0][25] ), .shift_data( temp[0][23] ), .temp( temp[1][25] ) );
MUX_2_to_1 level2_27( .Shift_or_not( dataB[1] ), .original( temp[0][26] ), .shift_data( temp[0][24] ), .temp( temp[1][26] ) );
MUX_2_to_1 level2_28( .Shift_or_not( dataB[1] ), .original( temp[0][27] ), .shift_data( temp[0][25] ), .temp( temp[1][27] ) );
MUX_2_to_1 level2_29( .Shift_or_not( dataB[1] ), .original( temp[0][28] ), .shift_data( temp[0][26] ), .temp( temp[1][28] ) );
MUX_2_to_1 level2_30( .Shift_or_not( dataB[1] ), .original( temp[0][29] ), .shift_data( temp[0][27] ), .temp( temp[1][29] ) );
MUX_2_to_1 level2_31( .Shift_or_not( dataB[1] ), .original( temp[0][30] ), .shift_data( temp[0][28] ), .temp( temp[1][30] ) );
MUX_2_to_1 level2_32( .Shift_or_not( dataB[1] ), .original( temp[0][31] ), .shift_data( temp[0][29] ), .temp( temp[1][31] ) );

// 第三層 
MUX_2_to_1 level3_1( .Shift_or_not( dataB[2] ), .original( temp[1][0] ), .shift_data( 1'b0 ), .temp( temp[2][0] ) );
MUX_2_to_1 level3_2( .Shift_or_not( dataB[2] ), .original( temp[1][1] ), .shift_data( 1'b0 ), .temp( temp[2][1] ) );
MUX_2_to_1 level3_3( .Shift_or_not( dataB[2] ), .original( temp[1][2] ), .shift_data( 1'b0 ), .temp( temp[2][2] ) );
MUX_2_to_1 level3_4( .Shift_or_not( dataB[2] ), .original( temp[1][3] ), .shift_data( 1'b0 ), .temp( temp[2][3] ) );
MUX_2_to_1 level3_5( .Shift_or_not( dataB[2] ), .original( temp[1][4] ), .shift_data( temp[1][0] ), .temp( temp[2][4] ) );
MUX_2_to_1 level3_6( .Shift_or_not( dataB[2] ), .original( temp[1][5] ), .shift_data( temp[1][1] ), .temp( temp[2][5] ) );
MUX_2_to_1 level3_7( .Shift_or_not( dataB[2] ), .original( temp[1][6] ), .shift_data( temp[1][2] ), .temp( temp[2][6] ) );
MUX_2_to_1 level3_8( .Shift_or_not( dataB[2] ), .original( temp[1][7] ), .shift_data( temp[1][3] ), .temp( temp[2][7] ) );
MUX_2_to_1 level3_9( .Shift_or_not( dataB[2] ), .original( temp[1][8] ), .shift_data( temp[1][4] ), .temp( temp[2][8] ) );
MUX_2_to_1 level3_10( .Shift_or_not( dataB[2] ), .original( temp[1][9] ), .shift_data( temp[1][5] ), .temp( temp[2][9] ) );
MUX_2_to_1 level3_11( .Shift_or_not( dataB[2] ), .original( temp[1][10] ), .shift_data( temp[1][6] ), .temp( temp[2][10] ) );
MUX_2_to_1 level3_12( .Shift_or_not( dataB[2] ), .original( temp[1][11] ), .shift_data( temp[1][7] ), .temp( temp[2][11] ) );
MUX_2_to_1 level3_13( .Shift_or_not( dataB[2] ), .original( temp[1][12] ), .shift_data( temp[1][8] ), .temp( temp[2][12] ) );
MUX_2_to_1 level3_14( .Shift_or_not( dataB[2] ), .original( temp[1][13] ), .shift_data( temp[1][9] ), .temp( temp[2][13] ) );
MUX_2_to_1 level3_15( .Shift_or_not( dataB[2] ), .original( temp[1][14] ), .shift_data( temp[1][10] ), .temp( temp[2][14] ) );
MUX_2_to_1 level3_16( .Shift_or_not( dataB[2] ), .original( temp[1][15] ), .shift_data( temp[1][11] ), .temp( temp[2][15] ) );
MUX_2_to_1 level3_17( .Shift_or_not( dataB[2] ), .original( temp[1][16] ), .shift_data( temp[1][12] ), .temp( temp[2][16] ) );
MUX_2_to_1 level3_18( .Shift_or_not( dataB[2] ), .original( temp[1][17] ), .shift_data( temp[1][13] ), .temp( temp[2][17] ) );
MUX_2_to_1 level3_19( .Shift_or_not( dataB[2] ), .original( temp[1][18] ), .shift_data( temp[1][14] ), .temp( temp[2][18] ) );
MUX_2_to_1 level3_20( .Shift_or_not( dataB[2] ), .original( temp[1][19] ), .shift_data( temp[1][15] ), .temp( temp[2][19] ) );
MUX_2_to_1 level3_21( .Shift_or_not( dataB[2] ), .original( temp[1][20] ), .shift_data( temp[1][16] ), .temp( temp[2][20] ) );
MUX_2_to_1 level3_22( .Shift_or_not( dataB[2] ), .original( temp[1][21] ), .shift_data( temp[1][17] ), .temp( temp[2][21] ) );
MUX_2_to_1 level3_23( .Shift_or_not( dataB[2] ), .original( temp[1][22] ), .shift_data( temp[1][18] ), .temp( temp[2][22] ) );
MUX_2_to_1 level3_24( .Shift_or_not( dataB[2] ), .original( temp[1][23] ), .shift_data( temp[1][19] ), .temp( temp[2][23] ) );
MUX_2_to_1 level3_25( .Shift_or_not( dataB[2] ), .original( temp[1][24] ), .shift_data( temp[1][20] ), .temp( temp[2][24] ) );
MUX_2_to_1 level3_26( .Shift_or_not( dataB[2] ), .original( temp[1][25] ), .shift_data( temp[1][21] ), .temp( temp[2][25] ) );
MUX_2_to_1 level3_27( .Shift_or_not( dataB[2] ), .original( temp[1][26] ), .shift_data( temp[1][22] ), .temp( temp[2][26] ) );
MUX_2_to_1 level3_28( .Shift_or_not( dataB[2] ), .original( temp[1][27] ), .shift_data( temp[1][23] ), .temp( temp[2][27] ) );
MUX_2_to_1 level3_29( .Shift_or_not( dataB[2] ), .original( temp[1][28] ), .shift_data( temp[1][24] ), .temp( temp[2][28] ) );
MUX_2_to_1 level3_30( .Shift_or_not( dataB[2] ), .original( temp[1][29] ), .shift_data( temp[1][25] ), .temp( temp[2][29] ) );
MUX_2_to_1 level3_31( .Shift_or_not( dataB[2] ), .original( temp[1][30] ), .shift_data( temp[1][26] ), .temp( temp[2][30] ) );
MUX_2_to_1 level3_32( .Shift_or_not( dataB[2] ), .original( temp[1][31] ), .shift_data( temp[1][27] ), .temp( temp[2][31] ) );

// 第四層 
MUX_2_to_1 level4_1( .Shift_or_not( dataB[3] ), .original( temp[2][0] ), .shift_data( 1'b0 ), .temp( temp[3][0] ) );
MUX_2_to_1 level4_2( .Shift_or_not( dataB[3] ), .original( temp[2][1] ), .shift_data( 1'b0 ), .temp( temp[3][1] ) );
MUX_2_to_1 level4_3( .Shift_or_not( dataB[3] ), .original( temp[2][2] ), .shift_data( 1'b0 ), .temp( temp[3][2] ) );
MUX_2_to_1 level4_4( .Shift_or_not( dataB[3] ), .original( temp[2][3] ), .shift_data( 1'b0 ), .temp( temp[3][3] ) );
MUX_2_to_1 level4_5( .Shift_or_not( dataB[3] ), .original( temp[2][4] ), .shift_data( 1'b0 ), .temp( temp[3][4] ) );
MUX_2_to_1 level4_6( .Shift_or_not( dataB[3] ), .original( temp[2][5] ), .shift_data( 1'b0 ), .temp( temp[3][5] ) );
MUX_2_to_1 level4_7( .Shift_or_not( dataB[3] ), .original( temp[2][6] ), .shift_data( 1'b0 ), .temp( temp[3][6] ) );
MUX_2_to_1 level4_8( .Shift_or_not( dataB[3] ), .original( temp[2][7] ), .shift_data( 1'b0 ), .temp( temp[3][7] ) );
MUX_2_to_1 level4_9( .Shift_or_not( dataB[3] ), .original( temp[2][8] ), .shift_data( temp[2][0] ), .temp( temp[3][8] ) );
MUX_2_to_1 level4_10( .Shift_or_not( dataB[3] ), .original( temp[2][9] ), .shift_data( temp[2][1] ), .temp( temp[3][9] ) );
MUX_2_to_1 level4_11( .Shift_or_not( dataB[3] ), .original( temp[2][10] ), .shift_data( temp[2][2] ), .temp( temp[3][10] ) );
MUX_2_to_1 level4_12( .Shift_or_not( dataB[3] ), .original( temp[2][11] ), .shift_data( temp[2][3] ), .temp( temp[3][11] ) );
MUX_2_to_1 level4_13( .Shift_or_not( dataB[3] ), .original( temp[2][12] ), .shift_data( temp[2][4] ), .temp( temp[3][12] ) );
MUX_2_to_1 level4_14( .Shift_or_not( dataB[3] ), .original( temp[2][13] ), .shift_data( temp[2][5] ), .temp( temp[3][13] ) );
MUX_2_to_1 level4_15( .Shift_or_not( dataB[3] ), .original( temp[2][14] ), .shift_data( temp[2][6] ), .temp( temp[3][14] ) );
MUX_2_to_1 level4_16( .Shift_or_not( dataB[3] ), .original( temp[2][15] ), .shift_data( temp[2][7] ), .temp( temp[3][15] ) );
MUX_2_to_1 level4_17( .Shift_or_not( dataB[3] ), .original( temp[2][16] ), .shift_data( temp[2][8] ), .temp( temp[3][16] ) );
MUX_2_to_1 level4_18( .Shift_or_not( dataB[3] ), .original( temp[2][17] ), .shift_data( temp[2][9] ), .temp( temp[3][17] ) );
MUX_2_to_1 level4_19( .Shift_or_not( dataB[3] ), .original( temp[2][18] ), .shift_data( temp[2][10] ), .temp( temp[3][18] ) );
MUX_2_to_1 level4_20( .Shift_or_not( dataB[3] ), .original( temp[2][19] ), .shift_data( temp[2][11] ), .temp( temp[3][19] ) );
MUX_2_to_1 level4_21( .Shift_or_not( dataB[3] ), .original( temp[2][20] ), .shift_data( temp[2][12] ), .temp( temp[3][20] ) );
MUX_2_to_1 level4_22( .Shift_or_not( dataB[3] ), .original( temp[2][21] ), .shift_data( temp[2][13] ), .temp( temp[3][21] ) );
MUX_2_to_1 level4_23( .Shift_or_not( dataB[3] ), .original( temp[2][22] ), .shift_data( temp[2][14] ), .temp( temp[3][22] ) );
MUX_2_to_1 level4_24( .Shift_or_not( dataB[3] ), .original( temp[2][23] ), .shift_data( temp[2][15] ), .temp( temp[3][23] ) );
MUX_2_to_1 level4_25( .Shift_or_not( dataB[3] ), .original( temp[2][24] ), .shift_data( temp[2][16] ), .temp( temp[3][24] ) );
MUX_2_to_1 level4_26( .Shift_or_not( dataB[3] ), .original( temp[2][25] ), .shift_data( temp[2][17] ), .temp( temp[3][25] ) );
MUX_2_to_1 level4_27( .Shift_or_not( dataB[3] ), .original( temp[2][26] ), .shift_data( temp[2][18] ), .temp( temp[3][26] ) );
MUX_2_to_1 level4_28( .Shift_or_not( dataB[3] ), .original( temp[2][27] ), .shift_data( temp[2][19] ), .temp( temp[3][27] ) );
MUX_2_to_1 level4_29( .Shift_or_not( dataB[3] ), .original( temp[2][28] ), .shift_data( temp[2][20] ), .temp( temp[3][28] ) );
MUX_2_to_1 level4_30( .Shift_or_not( dataB[3] ), .original( temp[2][29] ), .shift_data( temp[2][21] ), .temp( temp[3][29] ) );
MUX_2_to_1 level4_31( .Shift_or_not( dataB[3] ), .original( temp[2][30] ), .shift_data( temp[2][22] ), .temp( temp[3][30] ) );
MUX_2_to_1 level4_32( .Shift_or_not( dataB[3] ), .original( temp[2][31] ), .shift_data( temp[2][23] ), .temp( temp[3][31] ) );

// 第五層 
MUX_2_to_1 level5_1( .Shift_or_not( dataB[4] ), .original( temp[3][0] ), .shift_data( 1'b0 ), .temp( temp[4][0] ) );
MUX_2_to_1 level5_2( .Shift_or_not( dataB[4] ), .original( temp[3][1] ), .shift_data( 1'b0 ), .temp( temp[4][1] ) );
MUX_2_to_1 level5_3( .Shift_or_not( dataB[4] ), .original( temp[3][2] ), .shift_data( 1'b0 ), .temp( temp[4][2] ) );
MUX_2_to_1 level5_4( .Shift_or_not( dataB[4] ), .original( temp[3][3] ), .shift_data( 1'b0 ), .temp( temp[4][3] ) );
MUX_2_to_1 level5_5( .Shift_or_not( dataB[4] ), .original( temp[3][4] ), .shift_data( 1'b0 ), .temp( temp[4][4] ) );
MUX_2_to_1 level5_6( .Shift_or_not( dataB[4] ), .original( temp[3][5] ), .shift_data( 1'b0 ), .temp( temp[4][5] ) );
MUX_2_to_1 level5_7( .Shift_or_not( dataB[4] ), .original( temp[3][6] ), .shift_data( 1'b0 ), .temp( temp[4][6] ) );
MUX_2_to_1 level5_8( .Shift_or_not( dataB[4] ), .original( temp[3][7] ), .shift_data( 1'b0 ), .temp( temp[4][7] ) );
MUX_2_to_1 level5_9( .Shift_or_not( dataB[4] ), .original( temp[3][8] ), .shift_data( 1'b0 ), .temp( temp[4][8] ) );
MUX_2_to_1 level5_10( .Shift_or_not( dataB[4] ), .original( temp[3][9] ), .shift_data( 1'b0 ), .temp( temp[4][9] ) );
MUX_2_to_1 level5_11( .Shift_or_not( dataB[4] ), .original( temp[3][10] ), .shift_data( 1'b0 ), .temp( temp[4][10] ) );
MUX_2_to_1 level5_12( .Shift_or_not( dataB[4] ), .original( temp[3][11] ), .shift_data( 1'b0 ), .temp( temp[4][11] ) );
MUX_2_to_1 level5_13( .Shift_or_not( dataB[4] ), .original( temp[3][12] ), .shift_data( 1'b0 ), .temp( temp[4][12] ) );
MUX_2_to_1 level5_14( .Shift_or_not( dataB[4] ), .original( temp[3][13] ), .shift_data( 1'b0 ), .temp( temp[4][13] ) );
MUX_2_to_1 level5_15( .Shift_or_not( dataB[4] ), .original( temp[3][14] ), .shift_data( 1'b0 ), .temp( temp[4][14] ) );
MUX_2_to_1 level5_16( .Shift_or_not( dataB[4] ), .original( temp[3][15] ), .shift_data( 1'b0 ), .temp( temp[4][15] ) );
MUX_2_to_1 level5_17( .Shift_or_not( dataB[4] ), .original( temp[3][16] ), .shift_data( temp[3][0] ), .temp( temp[4][16] ) );
MUX_2_to_1 level5_18( .Shift_or_not( dataB[4] ), .original( temp[3][17] ), .shift_data( temp[3][1] ), .temp( temp[4][17] ) );
MUX_2_to_1 level5_19( .Shift_or_not( dataB[4] ), .original( temp[3][18] ), .shift_data( temp[3][2] ), .temp( temp[4][18] ) );
MUX_2_to_1 level5_20( .Shift_or_not( dataB[4] ), .original( temp[3][19] ), .shift_data( temp[3][3] ), .temp( temp[4][19] ) );
MUX_2_to_1 level5_21( .Shift_or_not( dataB[4] ), .original( temp[3][20] ), .shift_data( temp[3][4] ), .temp( temp[4][20] ) );
MUX_2_to_1 level5_22( .Shift_or_not( dataB[4] ), .original( temp[3][21] ), .shift_data( temp[3][5] ), .temp( temp[4][21] ) );
MUX_2_to_1 level5_23( .Shift_or_not( dataB[4] ), .original( temp[3][22] ), .shift_data( temp[3][6] ), .temp( temp[4][22] ) );
MUX_2_to_1 level5_24( .Shift_or_not( dataB[4] ), .original( temp[3][23] ), .shift_data( temp[3][7] ), .temp( temp[4][23] ) );
MUX_2_to_1 level5_25( .Shift_or_not( dataB[4] ), .original( temp[3][24] ), .shift_data( temp[3][8] ), .temp( temp[4][24] ) );
MUX_2_to_1 level5_26( .Shift_or_not( dataB[4] ), .original( temp[3][25] ), .shift_data( temp[3][9] ), .temp( temp[4][25] ) );
MUX_2_to_1 level5_27( .Shift_or_not( dataB[4] ), .original( temp[3][26] ), .shift_data( temp[3][10] ), .temp( temp[4][26] ) );
MUX_2_to_1 level5_28( .Shift_or_not( dataB[4] ), .original( temp[3][27] ), .shift_data( temp[3][11] ), .temp( temp[4][27] ) );
MUX_2_to_1 level5_29( .Shift_or_not( dataB[4] ), .original( temp[3][28] ), .shift_data( temp[3][12] ), .temp( temp[4][28] ) );
MUX_2_to_1 level5_30( .Shift_or_not( dataB[4] ), .original( temp[3][29] ), .shift_data( temp[3][13] ), .temp( temp[4][29] ) );
MUX_2_to_1 level5_31( .Shift_or_not( dataB[4] ), .original( temp[3][30] ), .shift_data( temp[3][14] ), .temp( temp[4][30] ) );
MUX_2_to_1 level5_32( .Shift_or_not( dataB[4] ), .original( temp[3][31] ), .shift_data( temp[3][15] ), .temp( temp[4][31] ) );

assign dataOut = temp[4] ;


endmodule