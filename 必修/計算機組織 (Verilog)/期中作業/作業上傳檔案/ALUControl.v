`timescale 1ns/1ns
module ALUControl( clk, Signal, SignaltoALU, SignaltoSHT, SignaltoMUL, SignaltoMUX );
input clk ;
input [5:0] Signal ;
output [5:0] SignaltoALU ;
output [5:0] SignaltoSHT ;
output [5:0] SignaltoMUL ;
output [5:0] SignaltoMUX ;

//   Signal ( 6-bits)?
//   AND  : 36
//   OR   : 37
//   ADD  : 32
//   SUB  : 34
//   SLT  : 42
//   SLL  : 0
//   MULTU : 25



reg [5:0] temp ;
reg [6:0] counter ;


parameter AND = 6'b100100; // 36 
parameter OR  = 6'b100101; // 37 
parameter ADD = 6'b100000; // 32 
parameter SUB = 6'b100010; // 34 
parameter SLT = 6'b101010; // 42 

parameter SLL = 6'b000000; // 0 

parameter MULTU= 6'b011001; // 25 
parameter MFHI= 6'b010000; // 16 
parameter MFLO= 6'b010010; // 18 
/*
定義各種訊號
*/

/*
=====================================================
下面為模擬範例，程式撰寫請遵照老師上課說明的方法來寫
=====================================================
*/
always@( Signal )
begin
  if ( Signal == MULTU )
  begin
    counter = 0 ;
  end
/*
如果訊號改變成除法 就把counter歸0
*/
end

always@( posedge clk )
begin
  temp = Signal ;
  if ( Signal == MULTU )
  begin
    if ( counter == 32 )
    begin
      temp = 6'b111111 ; // Open HiLo reg for Mul 
      counter = 0 ;
    end
    counter = counter + 1 ;
  end
/*
數32個clk然後開啟HiLo暫存器給除法器放值進去
*/
end


assign SignaltoALU = temp ;
assign SignaltoSHT = temp ;
assign SignaltoMUL = temp ;
assign SignaltoMUX = temp ;


endmodule