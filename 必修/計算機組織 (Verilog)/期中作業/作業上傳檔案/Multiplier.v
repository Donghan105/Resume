`timescale 1ns/1ns
module Multiplier( clk, dataA, dataB, Signal, dataOut, reset );
input clk ;
input reset ;
input [31:0] dataA ;
input [31:0] dataB ;
input [5:0] Signal ;
output [63:0] dataOut ;

//   Signal ( 6-bits)?
//   MULTU : 25

reg [63:0] temp;
reg [6:0] count;
reg [63:0] hilo; // hi:result, lo:multiplier
parameter MULTU= 6'b011001; // 25 
parameter OUT = 6'b111111; // 63
parameter MFLO = 6'b010010; // 18
/*
�w�q�U�ذT��
*/
/*
=====================================================
�U���������d�ҡA�{�����g�п�ӦѮv�W�һ�������k�Ӽg
=====================================================
*/
always@( posedge clk or reset )
begin
  if ( reset )
  begin
    count = 0;
	hilo = 64'b0;
  end
	
  if(Signal==MULTU)
  begin
    if( count == 0 )
    begin
      hilo = { 32'b0, dataA };
    end
	
	if (hilo[0] == 1'b1)
    begin
	  hilo[63:32] = hilo[63:32]+dataB;
    end
	
	hilo = hilo>>1;
    count = count + 1;	
  end
  
  else
  begin
    if(Signal==OUT)
    begin
      temp = hilo;
    end
    else if(Signal==MFLO)
    begin
	  count = 0;
    end
  end
end
assign dataOut = temp ;

/*
=====================================================
�W���������d�ҡA�{�����g�п�ӦѮv�W�һ�������k�Ӽg
=====================================================
*/
endmodule