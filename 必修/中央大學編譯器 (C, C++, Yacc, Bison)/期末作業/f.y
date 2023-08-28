%{
#include <stdio.h>
#include<string.h>

typedef struct DataType{
  int type; // 0: string, 1: int 
  int num;
  char *token;
}DataType;

typedef struct StackType{
  struct StackType *next;
  struct StackType *prev;
  DataType data;
}StackType;

typedef struct SymType{
  struct SymType *next;
  struct SymType *prev;
  char *var;
  int value;
}SymType;


void yyerror(const char *message);
void AddStack( DataType i );
void PopStack( StackType *tmp, StackType *root, StackType *tail );
void PrintStack();
void PrintSymbtab(SymType *root);
void Execute();
void AddSYMB_TAB( DataType i );
int GetSYMB_value( char* str );

int error_1 = 0, error_2 = 0; // 1: duplicate var definition, 2: can't find var 
int expect = 0, last_expect = 0, error_type = 0;
StackType *stack_root, *stack_cur, *stack_tail;
SymType *symbtab_root, *symbtab_cur, *symbtab_tail;
%}

%code requires {
    struct eq{
        int num;
        int result;
    };
    struct rt{
        int value;
        int type;
    };
}

%union {
  char* str;
  int ival;
  struct rt returntype;
  struct eq args;
}

%type <returntype> PROGRAM
%type <returntype> STMT
%type <returntype> EXP
%type <returntype> DEF_STMT
%type <returntype> PRINT_STMT
%type <returntype> MULTI_EXP
%type <returntype> BOOL_VAL
%type <returntype> NUM_OP
%type <returntype> LOGICAL_OP
%type <ival> FUN_EXP
%type <ival> FUN_CALL
%type <ival> MULTI_ID
%type <returntype> IF_EXP
%type <returntype> PLUS
%type <returntype> MULTI_PLUS
%type <returntype> MINUS
%type <returntype> MULTIPLY
%type <returntype> MULTI_MULT
%type <returntype> DIVIDE
%type <returntype> MODULUS
%type <str> GREATER
%type <str> SMALLER
%type <ival> EQUAL
%type <args> MULTI_EQ
%type <ival> AND_OP
%type <ival> OR_OP
%type <ival> NOT_OP
%type <returntype> ID
%type <str> PLUSexpect
%type <str> MINUSexpect
%type <str> MULTexpect
%type <str> DIVIDEexpect
%type <str> MODexpect
%type <str> GREATERexpect
%type <str> SMALLERexpect
%type <str> EQUALexpect
%type <str> ANDexpect
%type <str> ORexpect
%type <str> NOTexpect

%token <str> id
%token <ival> number
%token <str> define
%token <str> print_num
%token <str> print_bool
%token <str> truee
%token <str> falsee
%token <str> mod
%token <str> and
%token <str> or
%token <str> not
%token <str> fun
%token <str> iff

%%
START:		PROGRAM
			;
PROGRAM :	 PROGRAM STMT{ 
							if ( error_1 == 1 ) { printf("Duplicate definition of variable\n"); }
							if ( error_2 == 1 ) { printf("Can't find variable\n"); }

							//if ( error_type == 1 ) { printf("Expect 'number' but got 'boolean'.\n"); }
							//else if ( error_type == 2 ) { printf("Expect 'boolean' but got 'number'.\n"); }
							// PrintStack(stack_root); 
							Execute(); error_1 = 0, error_2 = 0, error_type = 0; expect = 0; last_expect = 0;  // initialize 
						       }
			| STMT {
					if ( error_1 == 1 ) { printf("Duplicate definition of variable\n"); }
					if ( error_2 == 1 ) { printf("Can't find variable\n"); }

					//if ( error_type == 1 ) { printf("Expect 'number' but got 'boolean'.\n"); }
					//else if ( error_type == 2 ) { printf("Expect 'boolean' but got 'number'.\n"); }
					// PrintStack(stack_root); 
					Execute(); error_1 = 0, error_2 = 0, error_type = 0; expect = 0; last_expect = 0;  // initialize 
				      }
			;
STMT:		EXP { $$ = $1; }
			| DEF_STMT
			| PRINT_STMT { $$ = $1; } // stack_root, *stack_cur, *stack_tail;
        		;
DEF_STMT:	'(' define  id EXP ')' 
			{ 
			  DataType newdata1; newdata1.token = $2; newdata1.type = 0; newdata1.num = -1;  AddStack(newdata1);
			  DataType newdata2; newdata2.token = $3; newdata2.type = 0; newdata2.num = $4.value;  AddSYMB_TAB(newdata2); PrintSymbtab(symbtab_root);
			}
			;
PRINT_STMT:	'(' print_num EXP ')' { 
								$$ = $3; DataType newdata; newdata.token = $2; newdata.type = 0; newdata.num = -1; AddStack(newdata); 
								//if ( error_type == 0 && error_1 == 0 && error_2 == 0 ) { printf("%d\n", $3.value);  }
								printf("%d\n", $3.value); 
							   }
			| '('print_bool EXP ')' { 
								$$ = $3; DataType newdata; newdata.token = $2; newdata.type = 0; newdata.num = -1; AddStack(newdata);
								//if ( error_type == 0 && error_1 == 0 && error_2 == 0 ) { 
							    	//												}
								if ( $3.value != 0 ) {printf("true\n");  }
								else{ printf("false\n"); }
							     }
			;
MULTI_EXP:	MULTI_EXP EXP
			| EXP
			;
EXP:			BOOL_VAL { $$ = $1; }
			| number{ 	
						$$.type = 1;
						$$.value = $1; 
						if (error_type == 0) {
						  if (expect == 2) { // expect bool 
						    error_type = 2;
						  }
						}
					  } // @ = num 
			| ID { $$.type = 0; $$.value = $1.value; } 							// $$.type = $1.type;
			| NUM_OP { $$.type = $1.type; $$.value = $1.value; expect = last_expect; }  // printf("789\n");
			| LOGICAL_OP { $$.type = 2; $$.value = $1.value; expect = last_expect;}
			| FUN_EXP { $$.type = 0; $$.value = $1; } 
			| FUN_CALL { $$.type = 0; $$.value = $1; } 
			| IF_EXP { $$ = $1; } 
			;
ID:			id { $$.value = GetSYMB_value( $1 );} // printf("GET value: %d\n", $$.value);
			;
BOOL_VAL:	truee { 	
					$$.type = 2;
					$$.value = 1; 
					if (error_type == 0) {
						if (expect == 1) { // expect num 
						    error_type = 1;
						 }
					}
				   } // DataType newdata; newdata.token = $1; newdata.type = 1; newdata.num = 1;  AddStack(newdata);
			|falsee { 	
						$$.type = 2;
						$$.value = 0; 
						if (error_type == 0) {
							if (expect == 1) { // expect num 
						    		error_type = 1;
						 	}
						}
				      } // DataType newdata; newdata.token = $1; newdata.type = 1; newdata.num = 0;  AddStack(newdata); 	
			;
NUM_OP:	PLUS { $$.value = $1.value; $$.type = 1; } // printf("123\n");
			|MINUS { $$ = $1;  $$.type = 1; }
			|MULTIPLY { $$ = $1;  $$.type = 1; }
			|DIVIDE { $$ = $1; $$.type = 1;  }
			|MODULUS { $$ = $1;  $$.type = 1; }
			|GREATER { if ( $1 == "true" ) { $$.value = 1; } else{ $$.value = 0;  } $$.type = 2;  }				
			|SMALLER { if ( $1 == "true" ) { $$.value = 1; } else{ $$.value = 0;  } $$.type = 2; }						
			|EQUAL {  $$.value = $1; $$.type = 2; } // printf("EQ %d\n", $$.value); 
			;
PLUS:		'(' PLUSexpect EXP MULTI_PLUS ')' { 
									DataType newdata; newdata.token = (char*) malloc(sizeof(char)); *newdata.token = '+'; newdata.type = 0; newdata.num = -1;  
									AddStack(newdata); 
									// printf("+: %d %d\n", $3.value, $4.value); 
									$$.value = $3.value + $4.value; 
									$$.type = 1;
								      }
			;
PLUSexpect:	 '+' { last_expect = expect; expect = 1; } 
			;
MULTI_PLUS: MULTI_PLUS EXP { $$.value = $1.value + $2.value; }
			| EXP { $$.value = $1.value; } // printf("456\n");
			;
MINUS:		'(' MINUSexpect  EXP EXP ')' { 
							DataType newdata; newdata.token = (char*) malloc(sizeof(char)); *newdata.token = '-'; newdata.type = 0; newdata.num = -1;  
							AddStack(newdata); 
							// printf("-: %d %d\n", $3.value, $4.value); 
							$$.value = $3.value - $4.value;
							$$.type = 1;
						      } 
			;
MINUSexpect: '-' { last_expect = expect; expect = 1; } 
			;
MULTIPLY:	'(' MULTexpect EXP MULTI_MULT ')' { 
									DataType newdata; newdata.token = (char*) malloc(sizeof(char)); *newdata.token = '*'; newdata.type = 0; newdata.num = -1; 
									AddStack(newdata); 
									// printf("*: %d %d\n", $3.value, $4.value); 
									$$.value = $3.value * $4.value; 
									$$.type = 1;
									} 
			;
MULTexpect: '*' { last_expect = expect; expect = 1; } 
			;
MULTI_MULT: MULTI_MULT EXP { $$.value = $1.value * $2.value; }
			| EXP { $$.value = $1.value; }
			;
DIVIDE:		'(' DIVIDEexpect EXP EXP ')' { 
							DataType newdata; newdata.token = (char*) malloc(sizeof(char)); *newdata.token = '/'; newdata.type = 0; newdata.num = -1;  
							AddStack(newdata); 
							// printf("/: %d %d\n", $3.value, $4.value); 
							$$.value = $3.value / $4.value;
							$$.type = 1;
						      }
			;
DIVIDEexpect: '/' { last_expect = expect; expect = 1; } 
			;
MODULUS:	'(' MODexpect  EXP EXP ')' { 
								DataType newdata; newdata.token = $2; newdata.type = 0; newdata.num = -1; 
								AddStack(newdata);
								// printf("mod: %d %d\n", $3.value, $4.value);
								$$.value = $3.value % $4.value;
								$$.type = 1;
							   } 
			;
MODexpect:  mod { $$ = $1; last_expect = expect; expect = 1; } 
			;
GREATER:	'(' GREATERexpect EXP EXP ')' { 
							  DataType newdata; newdata.token = (char*) malloc(sizeof(char)); *newdata.token = '>'; newdata.type = 0; newdata.num = -1;  
							  AddStack(newdata); 
							  if ( $3.value > $4.value ) { $$ = "true"; } else{ $$ = "false";  }
							}
			;
GREATERexpect: '>' { last_expect = expect; expect = 1; } 
			;
SMALLER:	'(' SMALLERexpect EXP EXP ')' { 
							  DataType newdata; newdata.token = (char*) malloc(sizeof(char)); *newdata.token = '<'; newdata.type = 0; newdata.num = -1;  
							  AddStack(newdata); 
							  if ( $3.value < $4.value ) { $$ = "true"; } else{ $$ = "false";  }
							} 
			;
SMALLERexpect: '<' { last_expect = expect; expect = 1; } 
			;
EQUAL:		'(' EQUALexpect EXP MULTI_EQ ')' {     int i = $3.value, j = $4.num;
									DataType newdata; newdata.token = (char*) malloc(sizeof(char)); *newdata.token = '='; newdata.type = 0; newdata.num = -1;  
									AddStack(newdata); 
									if ( $4.result == 0 ) { $$ = 0; }
									else if ( i == j ) { $$ = 1; } 
									else { $$ = 0; }
								  } 
			;
EQUALexpect: '=' { last_expect = expect; expect = 1; } 
			;
MULTI_EQ: 	MULTI_EQ EXP {    int i = $1.num, j = $2.value;
							if ( i ==  j ) { $$.num = i; $$.result = 1; } else { $$.result = 0; }
						   } // push_eq_stack 
			| EXP {
					int i = $1.value;
					$$.num = i; $$.result = 1;
				   }
			;
LOGICAL_OP: AND_OP { if ( $1 > 0 ) { $$.value = 1; } else{ $$.value = 0;  } $$.type = 2; }
			|OR_OP { if ( $1 > 0 ) { $$.value = 1; } else{ $$.value = 0;  } $$.type = 2; }
			|NOT_OP { $$.value = $1; $$.type = 2; }
			;
AND_OP:	'(' ANDexpect EXP MULTI_MULT ')' { 
										DataType newdata; newdata.token = $2; newdata.type = 0; newdata.num = -1;  
										AddStack(newdata); 
										$$ = $3.value * $4.value;  
									 }
			;
ANDexpect:  and { $$ = $1; last_expect = expect; expect = 2; } 
			;
OR_OP:		'(' ORexpect EXP MULTI_PLUS ')' { 		
										DataType newdata; newdata.token = $2; newdata.type = 0; newdata.num = -1;  
										AddStack(newdata);  
										$$ = $3.value + $4.value; 
								    }
			; 
ORexpect:   or { $$ = $1; last_expect = expect; expect = 2; } 
			;
NOT_OP:		'(' NOTexpect EXP ')' { 
						  DataType newdata; newdata.token = $2; newdata.type = 0; newdata.num = -1;  
						  AddStack(newdata); 
						  if ( $3.value == 0 ) { $$ = 1; } else{ $$ = 0;  } 
						}
			;
NOTexpect:   not { $$ = $1; last_expect = expect; expect = 2; } 
			;
FUN_EXP:	'(' fun '(' ')' EXP ')' { $$=0; DataType newdata; newdata.token = $2; newdata.type = 0; newdata.num = -1;  AddStack(newdata);  }
			|'(' fun '(' MULTI_ID ')' EXP ')' { $$=0; DataType newdata; newdata.token = $2; newdata.type = 0; newdata.num = -1;  AddStack(newdata);  }
			;
MULTI_ID:	MULTI_ID id { DataType newdata; newdata.token  = $2; newdata.type = 0; newdata.num = -1; AddStack(newdata); }
			| id { DataType newdata; newdata.token  = $1; newdata.type = 0; newdata.num = -1; AddStack(newdata); }
			;
FUN_CALL:	'(' FUN_EXP ')' {$$=$2;}
			| '(' FUN_EXP MULTI_EXP ')' {$$=$2;}
			| '(' id ')' { $$=0; DataType newdata; newdata.token = $2; newdata.type = 0; newdata.num = -1;  AddStack(newdata);  }
			| '(' id MULTI_EXP ')' { $$=0; DataType newdata; newdata.token = $2; newdata.type = 0; newdata.num = -1;  AddStack(newdata);  }
			;
IF_EXP:		'(' iff EXP EXP EXP ')' {   if ( $3.type == 1 ) { error_type = 2; }  
							        if( $3.value != 0 ) { $$.value = $4.value; $$.type = $4.type; } 
							        else { $$.value = $5.value; $$.type = $5.type;} 
								expect = last_expect;
							   }
			;
%%

void AddStack( DataType i ) {
  StackType *new_node = (StackType*) malloc(sizeof(StackType));
  new_node->next = NULL;
  new_node->prev = NULL;
  new_node->data.token = i.token;
  new_node->data.num = i.num;
  new_node->data.type = i.type;

  if ( stack_root== stack_cur ) {
    stack_root->next = new_node;
    new_node->prev = stack_root;
    stack_cur = new_node;
    stack_tail = new_node;
  }
  else {
    stack_cur->next = new_node;
    new_node->prev = stack_cur;
    stack_cur = new_node;
    stack_tail = new_node;
  }
}

void PopStack(StackType *tmp, StackType *root, StackType *tail ) {
  if ( tmp->next != NULL && tmp->prev != NULL ) {
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    free(tmp);
  }
  else if ( tmp->next == NULL && tmp->prev != NULL ) { // tail 
    tail = tmp->prev;
    tmp->prev->next = NULL;
    free(tmp);
  }
  else if ( tmp == root ) { // dummy head node 
    free(tmp);
  }
}

void PrintStack( StackType *root  ) {
  StackType *tmp;
  tmp = root->next;
  while ( tmp  != NULL ) {
    char *ch= tmp->data.token;
    // printf("##%d##%d##%s##\n", tmp->data.type, tmp->data.num, tmp->data.token );
    tmp = tmp->next;
  }
}

void Execute() { // clear stack 
  stack_cur = stack_root ->next;
  while ( stack_cur != NULL ) {
    PopStack(stack_cur, stack_root, stack_tail );
    stack_cur = stack_root ->next;
  }

  stack_cur = stack_root;
  stack_tail = stack_cur;
}

void AddSYMB_TAB( DataType i ) {
  int result = 1;

  SymType *tmp;
  tmp = symbtab_root->next;
  while ( tmp  != NULL ) {
    if ( strcmp(tmp->var, i.token) == 0 ) {
      result = 0;
      break;
    }
    tmp = tmp->next;
  }

  if ( result ==1 ) {
    SymType *new_node = (SymType*) malloc(sizeof(SymType));
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->var = i.token;
    new_node->value = i.num;

    if ( symbtab_root== symbtab_cur ) {
      symbtab_root->next = new_node;
      new_node->prev = symbtab_root;
      symbtab_cur = new_node;
      symbtab_tail = new_node;
    }
    else {
      symbtab_cur->next = new_node;
      new_node->prev = symbtab_cur;
      symbtab_cur = new_node;
      symbtab_tail = new_node;
    }
  }
  else {
    error_1 = 1;
  }
}

int GetSYMB_value( char* str ) {
  int find = 0, result = 0;
  PrintSymbtab(symbtab_root);
  // printf("finding var: %s\n", str);
  SymType *tmp;
  tmp = symbtab_root->next;
  while ( tmp  != NULL ) {
    if ( strcmp(str, tmp->var) == 0) {
      find = 1;
      // printf("found!!\n");
      result = tmp->value;
      break;
    }
     tmp = tmp->next;
  }

  if ( find == 0 ) {
    error_2 = 1;
  }

  // printf("GETting value: %d\n", result);
  return result;
}

void PrintSymbtab(SymType *root) {
  SymType *tmp;
  tmp = root->next;
  while ( tmp  != NULL ) {
    // printf("symtab: %d##%s##\n", tmp->value, tmp->var );
    tmp = tmp->next;
  }
}

void yyerror (const char *message)
{
  fprintf (stderr, "%s\n",message);
}


int main(int argc, char *argv[]) {
  StackType *new_node1 = (StackType*) malloc(sizeof(StackType));
  new_node1->next = NULL;
  new_node1->prev = NULL;
  new_node1->data.num = -1;
  new_node1->data.type = -1;
  new_node1->data.token = NULL;
  stack_root = new_node1;
  stack_cur = stack_root;
  stack_tail = stack_cur;

  SymType *new_node2 = (SymType*) malloc(sizeof(SymType));
  new_node2->next = NULL;
  new_node2->prev = NULL;
  new_node2->value = -1;
  new_node2->var = NULL;
  symbtab_root = new_node2;
  symbtab_cur = symbtab_root;
  symbtab_tail = symbtab_cur;

  yyparse();

  return(0);
}