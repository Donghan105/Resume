%{
#include "y.tab.h"
#include <stdio.h>
%}
print_num print\-num
print_bool print\-bool
mod mod
define define
func fun
iff if
and and
or or
not not
op {and}|{or}|{not}
resw {print_num}|{print_bool}|{mod}|{define}|{func}|{if}|{op}
letter [a-z]
digit [0-9]
separator "\t"|"\n"|"\r"|" "
number 0|[1-9]{digit}*|"-"[1-9]{digit}*
id {letter}({letter}|{digit}|"-")*
truee "#"t
falsee "#"f
bool_val {true}|{false}
%%
{separator}	{/* do nothing */}
{define}		{ yylval.str = strdup(yytext); return(define);} 
{print_num}	{ yylval.str = strdup(yytext); return(print_num);} 
{print_bool}	{ yylval.str = strdup(yytext); return(print_bool);} 
{truee}		{ yylval.str = strdup(yytext); return(truee);} 
{falsee}		{ yylval.str = strdup(yytext); return(falsee);}
{mod}		{ yylval.str = strdup(yytext); return(mod);}
{and}		{ yylval.str = strdup(yytext); return(and);}
{or}			{ yylval.str = strdup(yytext); return(or);}
{not}		{ yylval.str = strdup(yytext); return(not);}
{func}		{ yylval.str = strdup(yytext); return(fun);}
{iff}			{ yylval.str = strdup(yytext); return(iff);}
{number}	{ yylval.ival = atoi(yytext); return(number);} // printf("number: %s\n", yytext);
{id}			{ yylval.str = strdup(yytext); return(id);} // printf("id: %s\n", yytext); 
"+"|"("|")"|"-"|"*"|"/"|">"|"<"|"=" { return(yytext[0]);} // printf("delimiter %s\n", yytext); 
.			{ return(0); /* EOF */ }
%%
//int main() {
//  yylex();
//  return 0;
//}