// StyleCheckType vector
# include <iostream>
# include <stdio.h>
# include <string> // string���O, strcpy, assign, substr, erase, find, erase, strstr 
# include <vector>  // �ʺA�}�C 
# include <stdlib.h> // atoi 
# include <sstream> // char to string  
using namespace std;

struct Lines{ 
  int row;
  int column;
  string token;
  string type;
}; // �ϥΪ̿�J��code 

struct ReturnResult{
  int i;
  bool result;
  int stats;
  int error;
  int unrecognized;
  int unexpected;
  int undefined;
  int defining;
}; // �ˬd��k�ɪ�return  

// error 1: Error 
// error 2: Waiting for remaining input 
// error 3: Quit 

struct Defined{
  string variable; // �ܼƦW��  
  string data; // ���e  
  string type; // ���O 
  string code;
  int times; // �ĴX���Q�w�q  
};

struct CalStack{
  string data; // ���e  
  string type; // ���O 
  int error;
};

// error 1: Error 
// error 2: Waiting for remaining input 
// error 3: Quit 


vector<Lines> glob_codes; // �ϥΪ̿�J��code  
vector<Defined> glob_defined_list; // �w�ŧi��variable 
vector<Defined> glob_last_defined_list; // �����W�@���w�ŧi��variable 
vector<Defined> glob_compnd_defined_list; // ����{int a, b... } 
vector<Defined> glob_multi_declaration; // ����int a, b...  

// OurC feature variables 
int glob_type_wrong = -1, glob_line = 1;
int glob_cmpnd_statmnt = 0;

int FindToken( int i ) ; // ���Lwhitespace ��X�U��token 
bool Type_list( string str ) ;
bool Token_list( string str ) ; // �P�_��k�� �X�k��token 
bool Is_Token( string str ) ; // ��token�� �һݭn���ѥX��token  
bool Is_SP_Token( string str ) ; // special token, line-comment, '/', '\\' 
bool Is_White_Space( string TMP ) ; // whitespace 
string Is_NUM( string str ) ; // ��r�B�z, ascii code�P�w�O�_���Ʀr  
bool Is_CONST( string str ) ;
bool Is_zero( string str ) ;
bool Is_IDENT( string str, int index ) ; // identifier 
string Not_IDENT( string token ) ; // �t���d��~�r��, ���Oident, �Nident�ର�ӽd��~�r�� @bc -> @ 
int FindFarthest( vector<int>& arr, int index ) ;

void Line_Comment() ; // if found line-comment, delete words after line-comment 
void Num_split_with_unrecog() ; // 20& -> 20 &, 35.5@ -> 35.5 @, 40.2.2@ -> 40.2 .2 @ 
void Examine_float() ; // �ˬd�O���O�B�I�� 
void Double_decimal_3( double d ) ; // printf(%1.3f) 
void Unrecognized_or_Unexpected( string str, int index, int line ) ; // �ˬd�O�_�Ҧ�ident���w�gdefine�L  
void Print_Unrecognized_or_Unexpected( ReturnResult tmp ) ;
bool Find_IDENT( string str ) ; // make sure that this IDENT is defined or not
bool Find_compnd_IDENT( string str ) ;
bool Find_IDENT_in_last_version( string str ) ;
Defined Get_IDENT_value( string str ) ; // make sure that this IDENT is defined or not
Defined Overwrite_IDENT( Defined new_element ) ;
void Del_IDENT( string var ) ;
int Check_divide_zero() ; // �^��0: ����S��token or �S��, �^��-1: ��undefined token, �^��1: �o�Ͱ��H0 
int Check_all_defined( int start, int end ) ; // �ˬd�O��ؤ����@�ؿ��~  
int Define_new_or_not() ;

void ClearTakenToken( int j ) ; // del the token that already gets 
int Syntax_check( int i ) ; // �ˬd���L��k���~  

// OurC feature functions 
void Cin_cout() ;
void Set_type() ;
void Set_compnd_defined_list();
void Update_type() ;
void Char_String_tab_newline() ;
int Executed_or_Entered() ;
void ListAllVariables() ;
void ListVariable( string variable ) ;
void ListAllFunctions() ;
void ListFunction( string variable ) ;
void Multi_Declaration( int end ) ;
void Declared_code( string variable, int end ) ;

ReturnResult User_input( ReturnResult index, int last_i ) ; // user_input, stats 1 2, last_i ok 
ReturnResult Definition( ReturnResult index ) ; // �w�qfunc or �ŧi�ܼ�  ok 
ReturnResult Type_specifier( ReturnResult index ) ; // ���Fvoid�H�~�����O  ok 
ReturnResult Rest_of_declarators( ReturnResult index ) ; // rest_of_declarators, stats 1 2 3 ok 
ReturnResult Function_definition( ReturnResult index, int last_i ) ; // �w�qfunc, last_i ok 
ReturnResult Formal_parameter_list( ReturnResult index, int last_i ) ; // stats 1 2, last_i ok 
ReturnResult Compound_statement( ReturnResult index, int last_i ) ; // Compound_statement, stats 1 2 
ReturnResult Declaration( ReturnResult index ) ;   // �ŧi�ܼ� ok 
ReturnResult Statement( ReturnResult index ) ; // statement ok 
ReturnResult Expression( ReturnResult index, int last_i ) ; // expression, stats 1 2, last_i ok 
ReturnResult Basic_expression( ReturnResult index ) ; // basic_expression, stats 1 2 ok 

ReturnResult Roisbe( ReturnResult index, int last_i ) ; // stats 1 2 ok 
ReturnResult Ropisbe( ReturnResult index, int last_i ) ; // rest_of_PPMM_Identifier_started_basic_exp ok 
bool Is_sign( string str ) ; // ok 
ReturnResult Actual_parameter_list( ReturnResult index, int last_i ) ; // stats 1 2, last_i ok 
ReturnResult Assignment_operator( ReturnResult index ) ; // ok 

ReturnResult Romce_and_romloe( ReturnResult index, int last_i ) ; // Romce_and_romloe, stats 1 ok 
ReturnResult Romloe( ReturnResult index, int last_i ) ; // romloe, stats 1 2 ok 
ReturnResult Romlae( ReturnResult index, int last_i ) ; // romlae, stats 1 2 ok 
ReturnResult Romboe( ReturnResult index, int last_i ) ; // romboe, stats 1 2 ok 
ReturnResult Rombeoe( ReturnResult index, int last_i ) ; // rombeoe, stats 1 2 ok 
ReturnResult Rombae( ReturnResult index, int last_i ) ; // rombae, stats 1 2 ok 
ReturnResult Romee( ReturnResult index, int last_i ) ; // romee, stats 1 2 ok 
ReturnResult Romre( ReturnResult index, int last_i ) ; // rest_of_maybe_relational_exp, stats 1 2 ok 
ReturnResult Romse( ReturnResult index, int last_i ) ; // rest_of_maybe_shift_exp, stats 1 2 ok 
ReturnResult Romae( ReturnResult index, int last_i ) ; // rest_of_maybe_additive_exp, stats 1 2 ok 
ReturnResult Romme( ReturnResult index, int last_i ) ; // rest_of_maybe_mult_exp, stats 1 2 ok 

ReturnResult M_lae( ReturnResult index ) ; // maybe_logical_AND_exp, stats 1 ok 
ReturnResult M_boe( ReturnResult index ) ; // maybe_bit_OR_exp, stats 1 ok 
ReturnResult M_beoe( ReturnResult index ) ; // maybe_bit_ex_OR_exp, stats 1 ok 
ReturnResult M_bae( ReturnResult index ) ; // maybe_bit_AND_exp, stats 1 ok 
ReturnResult M_ee( ReturnResult index ) ; // maybe_equality_exp, stats 1 ok 
ReturnResult M_re( ReturnResult index ) ; // maybe_relational_exp, stats 1 ok 
ReturnResult M_se( ReturnResult index ) ; // maybe_shift_exp, stats 1 ok 
ReturnResult M_ae( ReturnResult index ) ; // maybe_additive_exp, stats 1 ok 
ReturnResult M_me( ReturnResult index ) ; // maybe_mult_exp, stats 1 ok 

ReturnResult Unary_exp( ReturnResult index ) ; // ����++ --�Osign �٬O PP/MM 
ReturnResult Signed_unary_exp( ReturnResult index ) ;
ReturnResult Unsigned_unary_exp( ReturnResult index ) ;

int FindToken( int i ) { // ���Lwhitespace ��X�U��token  
  int while_break = 0;
  string token, type;
  while ( i < glob_codes.size() && while_break == 0 ) {
    token = glob_codes[i].token;
    type = glob_codes[i].type;
    if ( Is_White_Space( token ) == false || Is_CONST( type ) == true ) {
      while_break++; // break 
    } // end if 
    else {
      i++;
    }  // end else
  } // end while 

  if ( i >= glob_codes.size() ) {
    i = -1;
  } // end if 

  return i;
} // FindToken() 



bool Type_list( string str ) {
  if ( str == "int" || str == "float" || str == "char" || str == "bool" ||  str == "if" || str == "do" ) {
    return true;
  } // end if 
  else if ( str == "void" || str == "string" || str == "else" || str == "while" || str == "return" ) {
    return true;
  } // end else if 
  else {
    return false;
  } // end else 
} // Type_list() 

bool Token_list( string str ) { // �P�_��k�� �X�k��token 
  if ( str == "(" || str == ")" || str == "[" || str == "]" || str == "{" || str == "}" ) {
    return true;
  } // end if 
  else if ( str == "+" || str == "-" || str == "*" || str == "/" || str == "%" ) {
    return true;
  } // end else if 
  else if ( str == ">" || str == "<" || str == "=" || str == "!" ) {
    return true;
  } // end else if 
  else if ( str == "?" || str == ":" || str == "&" || str == "|" ) {
    return true;
  } // end else if 
  else if ( str == ";" || str == "," ) {
    return true;
  } // end else if 
  else if ( str == "\'" || str == "\"" || str == "." || str == "\\" ) {
  // str == "\'" || str == "\"" || str == "." || str == "\\" 
    return true;
  } // end else if 
  else {
    return false;
  } // end else 
} // Token_list() 

bool Is_Token( string str ) { // ��token�� �һݭn���ѥX��token  
  if ( str == "(" || str == ")" || str == "[" || str == "]" || str == "{" || str == "}" ) {
    return true;
  } // end if 
  else if ( str == "+" || str == "-" || str == "*" || str == "/" || str == "%" ) {
    return true;
  } // end else if 
  else if ( str == ">" || str == "<" || str == "=" || str == "!" ) {
    return true;
  } // end else if 
  else if ( str == "?" || str == ":" || str == "&" || str == "|" ) {
    return true;
  } // end else if 
  else if ( str == ";" || str == "," ) {
    return true;
  } // end else if 
  else if ( str == "\'" || str == "\"" || str == "." || str == "\\" ) {
  // str == "\'" || str == "\"" || str == "." || str == "\\" 
    return true;
  } // end else if 
  else {
    return false;
  } // end else 
} // Is_Token() 

bool Is_SP_Token( string str ) { // special token, line-comment, '/', '\\' 
  if ( str == "/" || str == "\\" ) {
    return true;
  } // end if 
  else {
    return false;
  } // end else 
} // Is_SP_Token() 

bool Is_White_Space( string TMP ) { // whitespace 
  if ( TMP == " " || TMP == "\t" || TMP == "\n" ) {
    return true;
  } // end if 
  else {
    return false;
  } // end else 
} // Is_White_Space() 

string Is_NUM( string str ) { // ��r�B�z, ascii code�P�w�O�_���Ʀr  
  bool result = true;
  int dotcount = 0;
  for ( int i = 0;  i < str.size() ; i++ ) { // �@�Ӧr���@�Ӧr�� �P�_ 
    if ( ( int ) str[i] == 46 ) {
      dotcount++;
      if ( dotcount > 1 ) {
        result = false;
        i = str.size();
      } // end if 
    } // end if 
    else if ( i == 0 && str.size() > 1 && ( ( int ) str[i] == 45 || ( int ) str[i] == 43 ) ) {
      // +, - sign (����u�� + or -, ���str�ݦ���L�Ʀr )
      result = true;
    } // end else if 
    else if ( ( int ) str[i] < 48 || ( int ) str[i] > 57 ) { // 0~9 ascii code
      result = false;
      i = str.size();
    } // end else if 
  } // end for 
  
  if ( result == false ) { return "error"; } // end if 
  else if ( dotcount == 1 ) { return "float"; } // end else if 
  else if ( dotcount == 0 ) { return "integer"; } // end else if 
  else { return "what?"; } // end else 
} // Is_NUM() 

bool Is_CONST( string str ) {
  bool result = false;
  if ( str == "integer" || str == "float" || str == "char" || str == "string" || str == "bool" ) {
    result = true;
  } // end if 

  return result;
} // Is_CONST() 

bool Is_zero( string str ) {
  double d;
  d = atof( str.c_str() );
  if ( d == 0 ) {
    return true;
  } // end if 
  else {
    return false;
  } // end else  
} // Is_zero() 

bool Is_IDENT( string str, int index ) { // identifier 
  bool result = true;
  if ( ( ( int ) str[0] < 65 || ( int ) str[0] > 90 ) ) {  // 1st char not letters
    if ( ( ( int ) str[0] < 97 || ( int ) str[0] > 122 ) ) {
      result = false;
    } // end if
  } // end if

  for ( int i = 1; i < str.size() ; i++ ) { // �Ѿl��char  
    if ( ( ( int ) str[i] < 65 || ( int ) str[i] > 90 ) ) { // not letters
      if ( ( int ) str[i] < 97 || ( int ) str[i] > 122 ) {
        if ( ( ( int ) str[i] < 48 || ( int ) str[i] > 57 ) && ( int ) str[i] != 95 ) {
        // not digits and underline 
          result = false;
        } // end if 
      } // end if 
    } // end if 
  } // end for 

  //  if ( str == "quit" ) {
  //    result = true;
  //    for ( int i = 0; i < index ; i++ ) { // "quit" is the first token or not
  //      if ( Is_White_Space( glob_codes[i].token ) == true ) { // �Y��quit�e���u��whitespace(�����Ĥ@��token) 
  //        result = false;
  //      } // end if 
  //      else { // quit := 3; �Oident  
  //        result = true;
  //        i = index; // break
  //      } // end else 
  //    } // end for 
  //  } // end if 

  return result;
} // Is_IDENT() 

string Not_IDENT( string token ) { // �t���d��~�r��, ���Oident, �Nident�ର�ӽd��~�r�� @bc -> @ 
  string str;
  str = token;
  if ( ( ( int ) token[0] < 65 || ( int ) token[0] > 90 ) ) {  // 1st char not letters
    if ( ( ( int ) token[0] < 97 || ( int ) token[0] > 122 ) ) {
      str = token[0];
    } // end if
  } // end if

  if ( str == token ) { // �Y�q�L�Ĥ@��char������ ( ����i���r��, degits, ���u )  
    for ( int i = 1; i < token.size() ; i++ ) {
      if ( ( ( int ) token[i] < 65 || ( int ) token[i] > 90 ) ) { // not letters
        if ( ( int ) token[i] < 97 || ( int ) token[i] > 122 ) { 
          if ( ( ( int ) token[i] < 48 || ( int ) token[i] > 57 ) && ( int ) token[i] != 95 ) {
          // not digits and underline 
            str = token[i]; // ab3_# -> # 
            i = token.size(); // break 
          } // end if 
        } // end if 
      } // end if 
    } // end for 
  } // end if 
  //  if ( token.find( "quit" ) != -1 ) { // �Yquit@ �ର quit  
  //    str = "quit";
  //  } // end if 

  return str;
} // Not_IDENT() 

int FindFarthest( vector<int>& arr, int index ) {

  for ( int i = 0; i < arr.size() ; i++ ) {
    if ( arr[i] > index ) {
      index = arr[i];
    } // end if 
  } // end for 

  return index; 
} // FindFarthest() 

void Line_Comment() { // if found line-comment, delete words after line-comment 
  int row = -1;
  for ( int i = 0; i < glob_codes.size() ; i++ ) {
    if ( glob_codes[i].token == "//" ) {
      row = glob_codes[i].row;
    } // end if 

    if ( row == glob_codes[i].row ) {
      glob_codes.erase( glob_codes.begin() + i );
      i--;
    } // end if 
  } // end for 
} // Line_Comment()

void Num_split_with_unrecog() { // 20& -> 20 &, 35.5@ -> 35.5 @, 40.2.2@ -> 40.2 .2 @ 
  for ( int i = 0; i < glob_codes.size() ; i++ ) {
    int start = -1, end = -1, flag = 0;
    int dotcount = 0;
    if ( glob_codes[i].type == "unknown" ) {
      string str = glob_codes[i].token;

      for ( int j = 0; j < str.size() ; j++ ) {
        if ( j != 0 ) {
          if ( ( int ) str[j] >= 48 && ( int ) str[j] <= 57 ) { // is 0-9 degit
            end = j;
          } // end if 
          else if ( ( int ) str[j] == 46 ) { // is dot 
            if ( dotcount == 0 ) {
              dotcount++;
              end = j;
            } // end if 
            else { // �X�{2nd dot, �}�l��token   
              Lines newtoken;
              newtoken.token = str.substr( end + 1, str.size() - end - 1 ); // �Q�����b��q��token 
              newtoken.type = "unknown";
              newtoken.column = glob_codes[i].column + 1;
              newtoken.row = glob_codes[i].row + 1;
              glob_codes.insert( glob_codes.begin() + i + 1, newtoken ); // ���J�bi+1 

              glob_codes[i].token = str.substr( 0, end + 1 ); // �Q�����b�e�q��token 
              glob_codes[i].type = "float";

              j = str.size(); // break for 
            } // end else 
          } // end else if  
          else { // ���Φ�2��token 
            Lines newtoken;
            newtoken.token = str.substr( end + 1, str.size() - end - 1 ); // �Q�����b��q��token 
            newtoken.type = "unknown";
            newtoken.column = glob_codes[i].column + 1;
            newtoken.row = glob_codes[i].row + 1;
            glob_codes.insert( glob_codes.begin() + i + 1, newtoken ); // ���J�bi+1 

            glob_codes[i].token = str.substr( 0, end + 1 ); // �Q�����b�e�q��token 

            if ( dotcount == 0 ) { // is integer
              glob_codes[i].type = "integer";
            } // end if 
            else {
              glob_codes[i].type = "float";
            } // end else 

            j = str.size(); // break for 
          } // end else 
        } // end if 
        else if ( j == 0 ) {
          if ( ( ( int ) str[j] >= 48 && ( int ) str[j] <= 57 ) ) {
          // first degit is 0-9 or '.' 
            start = 0;
            end = 0;
          } // end if
          else if ( ( int ) str[j] == 46 ) {
            start = 0;
            end = 0;
            dotcount++;
          } // end else if 
          else { // �����X����Ʀr 
            j = str.size(); // break for 
          } // end else
        } // end else if 
      } // end for 
    } // end if 
  } // end for
} // Num_split_with_unrecog() 

void Examine_float() { // �ˬd�O���O�B�I�� 
  int flag = 0, start, end;
  int dotcount = 0;
  string temp = "NULL";
  for ( int i = 0; i < glob_codes.size() ; i++ ) { // �ϥΪ̿�J��code��token  
    if ( glob_codes[i].type == "integer" ) { // ��token�����  
      if ( flag == 0 ) { // �e���٤��{���O�p��  
        temp = glob_codes[i].token;
        start = i;
        flag = 1; // �q�{�b�_�}�l�Ntokens�զX���p�� 
      } // end if 
      else if ( flag == 1 ) { // �w�{�w�O�p��  
        temp += glob_codes[i].token; // �}�l�զX�p��  
      } // end if 
    } // end if 
    else if ( glob_codes[i].token == "." ) { // ��token��'.' 
      if ( flag == 0 ) { // �e���٥�����L�Ʀr ���p�Ưର.23  
        flag = 1; // �q�{�b�_�}�l�Ntokens�զX���p�� 
        start = i;
        dotcount++; // �����I�쪺'.'�ƶq 
        temp = "."; // �p��buffer 
      } // end if 
      else if ( flag == 1 ) { // �q�{�b�_�}�l�Ntokens�զX���p�� 
        if ( dotcount == 0 ) {
          dotcount++; // �����I�쪺'.'�ƶq 
          temp += "."; // �p��buffer 
        } // end if 
        else { // �J��ĤG��'.' 2..1-> 2. .1��p��  
          end = i-1;
          glob_codes[start].type = "float";
          glob_codes[start].token = temp;
          for ( int j = start + 1; j <= end ; j++ ) {
            glob_codes.erase( glob_codes.begin() + start + 1 );
            i--;
          } // end for 

          temp = ".";
          start = i;
          dotcount = 1;
        } // end else 
      } // end else if 
    } // end if 

    if ( i < glob_codes.size() - 1 && glob_codes[i+1].type != "integer" && glob_codes[i+1].token != "." ) {
      if ( flag == 1 ) {
        end = i;
        if ( start != end ) { // case: .23.
          glob_codes[start].type = "float";
          glob_codes[start].token = temp;
          for ( int j = start + 1; j <= end ; j++ ) {
            glob_codes.erase( glob_codes.begin() + start + 1 );
            i--;
          } // end for 
        } // end if 

        temp = "NULL";
        dotcount = 0;
        flag = 0;
      } // end if 
    } // end if 
  } // end for 
} // Examine_float() 

void Double_decimal_3( double d ) { // printf(%1.3f) 
  printf( "%1.3f\n", d );
} // Double_decimal_3() 

void Unrecognized_or_Unexpected( string str, int index, int line ) { // �ˬd�O�_�Ҧ�ident���w�gdefine�L  
// Is_ident, Token_list , Is_num
  bool result_token; 
  result_token = Token_list( str );
  if ( result_token == true ) {
    cout << "Line " << line << " : unexpected token : \'" << str << "\'" << endl; 
  } // end if 
  else if ( Is_NUM( str ) == "float" || Is_NUM( str ) == "integer" ) {
    cout << "Line " << line << " : unexpected token : \'" << str << "\'" << endl;
  } // end else if
  else if ( Is_IDENT( str, index ) == true ) {
    cout << "Line " << line << " : unexpected token : \'" << str << "\'" << endl;
  } // end else if
  else {
    str = Not_IDENT( str );

    if ( Token_list( str ) == true || Is_White_Space( str ) == true ) {
      cout << "Line " << line << " : unexpected token : \'" << str << "\'" << endl;
    } // end if 
    else {
      cout << "Line " << line << " : unrecognized token with first char : \'" << str[0] << "\'" << endl;
    } // end else 
  } // end else 
} // Unrecognized_or_Unexpected() 

void Print_Unrecognized_or_Unexpected( ReturnResult tmp ) {
  string token;
  if ( tmp.unrecognized != -1 ) { // error 3: Unrecognized 
    glob_codes[tmp.unrecognized].token = Not_IDENT( glob_codes[tmp.unrecognized].token );
    string unrecognized = " : unrecognized token with first char : \'";
    token = glob_codes[tmp.unrecognized].token;
    cout << "Line " << glob_line << unrecognized << token << "\'" << endl;
  } // end if 
  else if ( tmp.unexpected != -1 ) { // // error 2: Unexpected token 
    token = glob_codes[tmp.unexpected].token;
    cout << "Line " << glob_line << " : unexpected token : \'" << token << "\'" << endl; 
  } // end else if
  //      else if ( tmp.undefined == != -1 ) { // // error 4: Undefined 
  //        cout << "Undefined identifier : \'" << glob_codes[tmp.i].token << "\'" << endl;
  //      } // end else if
  else { // �֩w�O���줣�n���F�� �����ҬO���ؿ��~ (ex: ��token�OUnrecognized, Unexpected)
    Unrecognized_or_Unexpected( glob_codes[tmp.i].token, tmp.i, glob_line );
  } // end else 

  //      int new_var = Define_new_or_not();
  //      while ( new_var != -1 ) { // ���榳error, �Y���s�ŧi�ܼƫh�ݲ���  
  //        Del_IDENT( glob_codes[new_var].token );
  //        glob_codes.erase( glob_codes.begin() + new_var );
  //        new_var = Define_new_or_not();		
  //      } end while
} // Print_Unrecognized_or_Unexpected() 

bool Find_IDENT( string str ) { // make sure that this IDENT is defined or not
  bool result = false;
  
  for ( int i = 0; i < glob_defined_list.size() ; i++ ) {
    if ( str == glob_defined_list[i].variable ) {
      result = true;
      i = glob_defined_list.size(); // break 
    } // end if 
  } // end for 

  return result;
} // Find_IDENT()  

bool Find_compnd_IDENT( string str ) { // make sure that this IDENT is defined or not
  bool result = false;
  
  for ( int i = 0; i < glob_compnd_defined_list.size() ; i++ ) {
    if ( str == glob_compnd_defined_list[i].variable ) {
      result = true;
      i = glob_compnd_defined_list.size(); // break 
    } // end if 
  } // end for 

  return result;
} // Find_compnd_IDENT()  

bool Find_IDENT_in_last_version( string str ) {
  bool result = false;
  
  for ( int i = 0; i < glob_last_defined_list.size() ; i++ ) {
    if ( str == glob_last_defined_list[i].variable ) {
      result = true;
      i = glob_last_defined_list.size(); // break 
    } // end if 
  } // end for 

  return result;
} // Find_IDENT_in_last_version()

Defined Get_IDENT_value( string str ) { // make sure that this IDENT is defined or not 
  Defined element;
  for ( int i = 0; i < glob_defined_list.size() ; i++ ) {
    if ( str == glob_defined_list[i].variable ) {
      element.data = glob_defined_list[i].data;
      element.type = glob_defined_list[i].type;
      element.variable = glob_defined_list[i].variable;
      element.code = glob_defined_list[i].code;
      element.times =  glob_defined_list[i].times;
      i = glob_defined_list.size(); // break 
    } // end if 
  } // end for 

  return element;
} // Get_IDENT_value() 

Defined Overwrite_IDENT( Defined new_element ) {
  bool result = false;
  string type, var;
  var = new_element.variable;
  type = new_element.type;

  for ( int i = 0; i < glob_defined_list.size() ; i++ ) {
    if ( glob_defined_list[i].variable == var && glob_defined_list[i].type == type ) {
      glob_defined_list[i] = new_element;
      glob_defined_list[i].times = glob_defined_list[i].times + 1;
      i = glob_defined_list.size(); // break 
    } // end if 
  } // end for 

  return new_element;
} // Overwrite_IDENT() 

void Del_IDENT( string var ) {
  int a = 1;
  for ( int i = 0; i < glob_defined_list.size() ; i++ ) {
    if ( var == glob_defined_list[i].variable ) {
      glob_defined_list.erase( glob_defined_list.begin() + i );
      i = glob_defined_list.size(); // break
    } // end if 
  } // end for 
} // Del_IDENT() 

int Check_divide_zero() { // �^��0: ����S��token or �S��, �^��-1: ��undefined token, �^��1: �o�Ͱ��H0  
  int i = 0, flag = 0;
  int error = 0;
  string last_token;
  if ( FindToken( 0 ) != -1 ) {
    last_token = glob_codes[FindToken( 0 )].token;
  } // end if 
  else {
    i = -1;
  } // end else 
  
  while ( i < glob_codes.size() && i != -1 ) {
    if ( glob_codes[i].token == "/" ) {
      if ( Find_IDENT( last_token ) == true ) {
        Defined element;
        element = Get_IDENT_value( last_token );
        if ( element.data == "unknown" ) { // undefined token
          error = -1;
          i = glob_codes.size(); // break 
        } // end if 
        else {
          if ( flag == 0 ) {
            flag = 1;
          } // end if  
        } // end else 
      } // end if 
      else if ( Find_compnd_IDENT( last_token ) == true ) {
        Defined element;
        element = Get_IDENT_value( last_token );
        if ( element.data == "unknown" ) { // undefined token
          error = -1;
          i = glob_codes.size(); // break 
        } // end if 
        else {
          if ( flag == 0 ) {
            flag = 1;
          } // end if  
        } // end else 
      } // end if 
      else {
        if ( flag == 0 ) {
          flag = 1;
        } // end if  	
      } // end else 

      last_token = glob_codes[i].token;
    } // end if 
    else {
      if ( flag == 1 ) {
        if ( Is_zero( glob_codes[i].token ) == true ) {
          error = 1;
          i = glob_codes.size(); // break
        } // end if 
        else {
          flag = 0;
          last_token = glob_codes[i].token;
        } // end else 
      } // end if 
    } // end else 

    i++;
    i = FindToken( i );
  } // end while 

  return error;
} // Check_divide_zero() 

int Check_all_defined( int start, int end ) { // �ˬd�O��ؤ����@�ؿ��~  
  int result = -1;
  if ( end == -1 ) { end = glob_codes.size() - 1; } // end for, special case, �ݨϥΪ̸ɤWinput  

  for ( int i = end; i >= start ; i-- ) { // �˵ۨ� �N��o��̦������~index 
    if ( glob_codes[i].type == "IDENT" ) {
      if ( Find_IDENT( glob_codes[i].token ) == false ) {
        result = i;
      } // end if 
      else { // �����, ���T�w�O�_��������l��
        Defined element;
        int index = i + 1;
        element = Get_IDENT_value( glob_codes[i].token );
        index = FindToken( index );
        if ( element.data == "unknown" && glob_codes[index].token != ":=" ) {
          result = i;
          for ( int j = 0; j < glob_defined_list.size() ; j++ ) {
            if ( glob_defined_list[j].variable == glob_codes[i].token ) {
              glob_defined_list.erase( glob_defined_list.begin() + j );
              j = glob_defined_list.size(); // break��for 
            } // end if 
          } // end for 
        } // end if 
      } // end else
    } // end if
    else {
      Defined element;
      element = Get_IDENT_value( glob_codes[i].token );
      if ( Find_IDENT( glob_codes[i].token ) == true && element.data == "unknown" ) {
        result = i;
      } // end if 
    } // end else 
  } // end for 

  return result;
} // Check_all_defined() 

int Define_new_or_not() {
  int result = -1;
  Defined element;
  for ( int i = 0; i < glob_codes.size() ; i++ ) {
    element = Get_IDENT_value( glob_codes[i].token );
    if ( glob_codes[i].type == "IDENT" && element.data == "unknown" ) {
      result = i;
      i = glob_codes.size(); // break
    } // end if 
  } // end for 

  return result;
} // Define_new_or_not()

void ClearTakenToken( int j ) { // del the token that already gets 
  for ( int i = 0; i <= j ; i++ ) {
    glob_codes.erase( glob_codes.begin() );
  } // end for 
} // ClearTakenToken() 

int Syntax_check( int i ) { // �ˬd���L��k���~  
  // PrintCodes();

  int defined_or_not, index, done;
  int dvd_0 = 0;
  string var;
  ReturnResult tmp;
  CalStack result;
  tmp.i = i;
  tmp.stats = 1;
  tmp.result = true;
  tmp.error = 0;
  tmp.unrecognized = -1;
  tmp.unexpected = -1;
  tmp.undefined = -1;
  tmp.defining = -1;
  glob_last_defined_list = glob_defined_list; // �O�s�̪쪩  

  while ( tmp.error == 0 && tmp.result == true && glob_codes.size() > 0 && dvd_0 == 0 ) {
    Set_compnd_defined_list();
    tmp.result = false;
    tmp = User_input( tmp, i );
    if ( tmp.result == false ) {
      Print_Unrecognized_or_Unexpected( tmp );
      ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
    } // end if
    else {
      index = FindToken( 0 );
      if ( FindToken( 0 ) == -1 ) { // �S���i��token �u��whitespace  
        ClearTakenToken( glob_codes.size() - 1 ); // �M�����	
      } // end else if 
      else if ( glob_codes[index].token == "Done" ) { // 1st token is quit 
        done = index + 1;
        done = FindToken( done );

        if ( done == -1 ) { // waiting for remaining input 
          tmp.error = 2;
        } // end if 
        else if ( glob_codes[done].token == "(" ) {
          done++;
          done = FindToken( done );

          if ( done == -1 ) { // waiting for remaining input 
            tmp.error = 2;
          } // end if 
          else if ( glob_codes[done].token == ")" ) {
            tmp.error = 3; // quit 
          } // end if 
          else {
            tmp.result = false;
            tmp.i = done;
            tmp.undefined = -1;
            tmp.unexpected = -1;
            tmp.unrecognized = -1;
            Print_Unrecognized_or_Unexpected( tmp );
            ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
          } // end else
        } // end if 
        else {
          tmp.result = false;
          tmp.i = done;
          Print_Unrecognized_or_Unexpected( tmp );
          ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
        } // end else 
      } // end if 
      else if ( glob_codes[index].token == "ListAllVariables" ) { // 1st token is ListAllVariables 
        done = index + 1;
        done = FindToken( done );

        if ( done == -1 ) { // waiting for remaining input 
          tmp.error = 2;
        } // end if 
        else if ( glob_codes[done].token == "(" ) {
          done++;
          done = FindToken( done );

          if ( done == -1 ) { // waiting for remaining input 
            tmp.error = 2;
          } // end if 
          else if ( glob_codes[done].token == ")" ) {
            done++;
            done = FindToken( done );

            if ( done == -1 ) { // waiting for remaining input 
              tmp.error = 2;
            } // end if 
            else if ( glob_codes[done].token == ";" ) {
              ListAllVariables();
              cout << "Statement executed ..." << endl;
              cout << "> ";
              ClearTakenToken( tmp.i ); // �M���w���L (���ʥ���ݤU����J�ɤW����� �T�w��k�w����ѪR ) 
              tmp.i = i; // �qindex 0 ���s�}�l���R�U�@���k  
              tmp.stats = 1;
              glob_line = 0;
            } // end if 
            else {
              tmp.result = false;
              tmp.i = done;
              tmp.undefined = -1;
              tmp.unexpected = -1;
              tmp.unrecognized = -1;
              Print_Unrecognized_or_Unexpected( tmp );
              ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
            } // end else 
          } // end else if 
          else {
            tmp.result = false;
            tmp.i = done;
            tmp.undefined = -1;
            tmp.unexpected = -1;
            tmp.unrecognized = -1;
            Print_Unrecognized_or_Unexpected( tmp );
            ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
          } // end else
        } // end if 
        else {
          tmp.result = false;
          tmp.i = done;
          Print_Unrecognized_or_Unexpected( tmp );
          ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
        } // end else 
      } // end if 
      else if ( glob_codes[index].token == "ListVariable" ) { // 1st token is ListVariable 
        done = index + 1;
        done = FindToken( done );

        if ( done == -1 ) { // waiting for remaining input 
          tmp.error = 2;
        } // end if 
        else if ( glob_codes[done].token == "(" ) {
          done++;
          done = FindToken( done );

          if ( done == -1 ) { // waiting for remaining input 
            tmp.error = 2;
          } // end if 
          else if ( Is_IDENT( glob_codes[done].token, done ) == true ) {
            var = glob_codes[done].token;
            done++;
            done = FindToken( done );

            if ( done == -1 ) { // waiting for remaining input 
              tmp.error = 2;
            } // end if 
            else if ( glob_codes[done].token == ")" ) {
              done++;
              done = FindToken( done );

              if ( done == -1 ) { // waiting for remaining input 
                tmp.error = 2;
              } // end if 
              else if ( glob_codes[done].token == ";" ) {
                ListVariable( var );
                cout << "> ";
                ClearTakenToken( tmp.i );
                // �M���w���L (���ʥ���ݤU����J�ɤW����� �T�w��k�w����ѪR ) 
                tmp.i = i; // �qindex 0 ���s�}�l���R�U�@���k  
                tmp.stats = 1;
                glob_line = 0;
              } // end if 
              else {
                tmp.result = false;
                tmp.i = done;
                tmp.undefined = -1;
                tmp.unexpected = -1;
                tmp.unrecognized = -1;
                Print_Unrecognized_or_Unexpected( tmp );
                ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
              } // end else 
            } // end if 
            else {
              tmp.result = false;
              tmp.i = done;
              tmp.undefined = -1;
              tmp.unexpected = -1;
              tmp.unrecognized = -1;
              Print_Unrecognized_or_Unexpected( tmp );
              ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
            } // end else 
          } // end else if 
          else {
            tmp.result = false;
            tmp.i = done;
            tmp.undefined = -1;
            tmp.unexpected = -1;
            tmp.unrecognized = -1;
            Print_Unrecognized_or_Unexpected( tmp );
            ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
          } // end else 
        } // end if 
        else {
          tmp.result = false;
          tmp.i = done;
          Print_Unrecognized_or_Unexpected( tmp );
          ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
        } // end else 
      } // end if
      else if ( glob_codes[index].token == "ListAllFunctions" ) { // 1st token is ListAllFunctions 
        done = index + 1;
        done = FindToken( done );

        if ( done == -1 ) { // waiting for remaining input 
          tmp.error = 2;
        } // end if 
        else if ( glob_codes[done].token == "(" ) {
          done++;
          done = FindToken( done );

          if ( done == -1 ) { // waiting for remaining input 
            tmp.error = 2;
          } // end if 
          else if ( glob_codes[done].token == ")" ) {
            done++;
            done = FindToken( done );

            if ( done == -1 ) { // waiting for remaining input 
              tmp.error = 2;
            } // end if 
            else if ( glob_codes[done].token == ";" ) {
              // PrintCodes();
              ListAllFunctions();
              cout << "Statement executed ..." << endl;
              cout << "> ";
              ClearTakenToken( tmp.i ); // �M���w���L (���ʥ���ݤU����J�ɤW����� �T�w��k�w����ѪR ) 
              tmp.i = i; // �qindex 0 ���s�}�l���R�U�@���k  
              tmp.stats = 1;
              glob_line = 0;
            } // end if 
            else {
              tmp.result = false;
              tmp.i = done;
              tmp.undefined = -1;
              tmp.unexpected = -1;
              tmp.unrecognized = -1;
              Print_Unrecognized_or_Unexpected( tmp );
              ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
            } // end else 
          } // end else if 
          else {
            tmp.result = false;
            tmp.i = done;
            tmp.undefined = -1;
            tmp.unexpected = -1;
            tmp.unrecognized = -1;
            Print_Unrecognized_or_Unexpected( tmp );
            ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
          } // end else
        } // end if 
        else {
          tmp.result = false;
          tmp.i = done;
          Print_Unrecognized_or_Unexpected( tmp );
          ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
        } // end else 
      } // end if 
      else if ( glob_codes[index].token == "ListFunction" ) { // 1st token is ListFunction 
        done = index + 1;
        done = FindToken( done );

        if ( done == -1 ) { // waiting for remaining input 
          tmp.error = 2;
        } // end if 
        else if ( glob_codes[done].token == "(" ) {
          done++;
          done = FindToken( done );

          if ( done == -1 ) { // waiting for remaining input 
            tmp.error = 2;
          } // end if 
          else if ( Is_IDENT( glob_codes[done].token, done ) == true ) {
            var = glob_codes[done].token;
            done++;
            done = FindToken( done );

            if ( done == -1 ) { // waiting for remaining input 
              tmp.error = 2;
            } // end if 
            else if ( glob_codes[done].token == ")" ) {
              done++;
              done = FindToken( done );

              if ( done == -1 ) { // waiting for remaining input 
                tmp.error = 2;
              } // end if 
              else if ( glob_codes[done].token == ";" ) {
                ListFunction( var );
                cout << "> ";
                ClearTakenToken( tmp.i );
                // �M���w���L (���ʥ���ݤU����J�ɤW����� �T�w��k�w����ѪR ) 
                tmp.i = i; // �qindex 0 ���s�}�l���R�U�@���k  
                tmp.stats = 1;
                glob_line = 0;
              } // end if 
              else {
                tmp.result = false;
                tmp.i = done;
                tmp.undefined = -1;
                tmp.unexpected = -1;
                tmp.unrecognized = -1;
                Print_Unrecognized_or_Unexpected( tmp );
                ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
              } // end else 
            } // end if 
            else {
              tmp.result = false;
              tmp.i = done;
              tmp.undefined = -1;
              tmp.unexpected = -1;
              tmp.unrecognized = -1;
              Print_Unrecognized_or_Unexpected( tmp );
              ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
            } // end else 
          } // end else if 
          else {
            tmp.result = false;
            tmp.i = done;
            tmp.undefined = -1;
            tmp.unexpected = -1;
            tmp.unrecognized = -1;
            Print_Unrecognized_or_Unexpected( tmp );
            ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
          } // end else 
        } // end if 
        else {
          tmp.result = false;
          tmp.i = done;
          Print_Unrecognized_or_Unexpected( tmp );
          ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
        } // end else 
      } // end if
      else if ( tmp.undefined != -1 ) { // // error 4: Undefined 
        string token = glob_codes[tmp.undefined].token;
        cout << "Line " << glob_line << " : undefined identifier : \'" << token << "\'" << endl;
        tmp.result = false;
        
        //        int new_var = Define_new_or_not();
        //        while ( new_var != -1 ) { // ���榳error, �Y���s�ŧi�ܼƫh�ݲ���  
        //          Del_IDENT( glob_codes[new_var].token );
        //          glob_codes.erase( glob_codes.begin() + new_var );
        //          new_var = Define_new_or_not();		
        //       } end while 

        ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
      } // end else if 
      else if ( tmp.error == 2 ) {  // && tmp.i == 0 
      // waiting for remaining input (ex: abc\n) 
        for ( int i = 0; i < glob_codes.size() ; i++ ) {
          if ( glob_codes[i].type != "string" && glob_codes[i].type != "char" ) {
            if ( glob_codes[i].type != "DefType" && glob_codes[i].type != "Condition" ) {
              if ( glob_codes[i].type != "Return") {
                if ( Find_IDENT_in_last_version( glob_codes[i].token ) == false ) {
                  Del_IDENT( glob_codes[i].token ); // ���R�� �ݫ��O�����A�w�q  
                } // end if 
              } // end if 
            } // end if
          } // end if 
        } // end for
      } // end else if 
//      else if ( tmp.error == 2 && tmp.defining != -1 ) { 
//      // �� ':=', waiting for remaining input ( ex: abc\n :=\n ) 
//      } // end else if 
      else { // �}�l�T�{���H0 
        // dvd_0 = Check_divide_zero(); // �^��0: ����S��token or �S��, �^��-1: ��undefined token, �^��1: �o�Ͱ��H0 
        if ( dvd_0 == -1 ) {
          string token = glob_codes[defined_or_not].token;
          defined_or_not = Check_all_defined( 0, glob_codes.size() - 1 ) ;
          cout << "Line " << glob_line << " : undefined identifier : \'" << token << "\'" << endl;

          //          int new_var = Define_new_or_not();
          //          while ( new_var != -1 ) { // ���榳error, �Y���s�ŧi�ܼƫh�ݲ���  
          //            Del_IDENT( glob_codes[new_var].token );
          //            glob_codes.erase( glob_codes.begin() + new_var );
          //            new_var = Define_new_or_not();		
          //          } 

          tmp.result = false;
          // tmp.error = 4;
          tmp.undefined = defined_or_not;
          ClearTakenToken( glob_codes.size() - 1 ); // �M����� 
        } // end if 
        else if ( dvd_0 == 1 ) {
          tmp.error = 1;
          tmp.result = false;
          ClearTakenToken( glob_codes.size() - 1 ); // �M�����
        } // end else if 
        else if ( tmp.error == 2 ) {
        // // waiting for remaining input ( ex: abc + \n ) 
        } // end else if 
        else { // �}�l�i�J�B�� 
          // result = Calculation( tmp.i, glob_codes ); // �}�l�p�� 
//          if ( glob_cmpnd_statmnt != 0 ) {
//            Defined element;
//            for ( int j = 0; j < glob_compnd_defined_list.size() ; j++ ) {
//              if ( Find_IDENT( glob_compnd_defined_list[j].variable ) == false ) {
//                element = glob_compnd_defined_list[j];
//                glob_defined_list.push_back( element );
//              }
//		    } // end for 
//          } // end if 
 
          if ( result.type == "float" ) {
            double d;
            d = atof( result.data.c_str() );
            Double_decimal_3( d );
          } // end if 
          else {
            // cout << result.data << endl; // �L�X�p�⵲�G  
          } // end else

          if ( Executed_or_Entered() == -1 ) {
            cout << "Statement executed ..." << endl;
          } // end if 
          else {
            int first_token = FindToken( 0 );
            Defined variable;
            string str = glob_codes[Executed_or_Entered()].token;

            variable = Get_IDENT_value( glob_codes[Executed_or_Entered()].token );

            if ( variable.type == "Function" ) {
              if ( variable.times > 1 ) {
                cout << "New definition of " << str << "() entered ..." << endl;
              }
              else {
                cout << "Definition of " << str << "() entered ..." << endl;
              } // end else 
            } // end if 
            else if ( variable.type == "Variable" ) {
              if ( variable.times > 1 ) {
                cout << "New definition of " << str << " entered ..." << endl;
              }
              else {
                cout << "Definition of " << str << " entered ..." << endl;
              } // end else
            } // end else if 

            if ( variable.type != "Function" && glob_codes[first_token].token != "{" ) {
            // int a, b, c.. 
              Multi_Declaration( tmp.i );
            } // end if 

            Declared_code( glob_codes[Executed_or_Entered()].token, tmp.i );
          } // end else 

          cout << "> ";
//          for ( int j=0; j<glob_defined_list.size(); j++ ) {
//            cout << "var: " << glob_defined_list[j].variable << " type: " << glob_defined_list[j].type << endl;
//		  } 
          ClearTakenToken( tmp.i ); // �M���w���L (���ʥ���ݤU����J�ɤW����� �T�w��k�w����ѪR ) 
          tmp.i = i; // �qindex 0 ���s�}�l���R�U�@���k  
          tmp.stats = 1;
          glob_line = 0;
          glob_cmpnd_statmnt = 0;
          glob_compnd_defined_list.clear();
        } // end else 
      } // end else 
    } // end else

    tmp.unrecognized = -1;
    tmp.unexpected = -1;
    tmp.undefined = -1;
    tmp.defining = -1;
    glob_type_wrong = -1;
    glob_multi_declaration.clear();
    glob_last_defined_list = glob_defined_list; // �O�s�W�@��  
  } // end while 

  if ( tmp.error == 1 ) {
    glob_line = 0;
    cout << "error" << endl;
    cout << "> ";
    return false;
  } // end if  
  else if ( tmp.result == false ) { // recognized, unexpected, undefined 
    glob_line = 0;
    // PrintCodes();
    cout << "> ";
    return false;
  } // end else if 
  else if ( tmp.error == 2 ) { // �ݤU���ϥΪ̿�J�ɤW���  
    // cout << "Waiting for remaining inputs." << endl; 
    // PrintCodes();
    // cout << "> ";
    return false;
  } // end else if 
  else if ( tmp.error == 3 ) { // quit 
    cout << "Our-C exited ..."; 
    return true;
  } // end if  
  else { // correct
    // cout << "Syntax Correct." << endl; 
    // ClearTakenToken( tmp.i ); // �M����� 
    // PrintCodes();
    return false;
  } // end else 
} // Syntax_check() 

void Cin_cout() {
  Defined element;
  element.variable = "cin";
  element.data = "unknown";
  element.type = "Cin";
  element.times = 1;
  glob_defined_list.push_back( element );
  
  element.variable = "cout";
  element.data = "unknown";
  element.type = "Cout";
  element.times = 1;
  glob_defined_list.push_back( element );
} // Cin_cout() 

void Set_type() {
  Defined element;
  element.variable = "int";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_defined_list.push_back( element );
  
  element.variable = "float";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_defined_list.push_back( element );

  element.variable = "char";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_defined_list.push_back( element );

  element.variable = "bool";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_defined_list.push_back( element );

  element.variable = "string";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_defined_list.push_back( element );

  element.variable = "void";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_defined_list.push_back( element );

  element.variable = "if";
  element.data = "unknown";
  element.type = "Condition";
  element.times = 1;
  glob_defined_list.push_back( element );

  element.variable = "else";
  element.data = "unknown";
  element.type = "Condition";
  element.times = 1;
  glob_defined_list.push_back( element );

  element.variable = "while";
  element.data = "unknown";
  element.type = "Condition";
  element.times = 1;
  glob_defined_list.push_back( element );

  element.variable = "do";
  element.data = "unknown";
  element.type = "Condition";
  element.times = 1;
  glob_defined_list.push_back( element );

  element.variable = "return";
  element.data = "unknown";
  element.type = "Return";
  element.times = 1;
  glob_defined_list.push_back( element );

  element.variable = "Done";
  element.data = "unknown";
  element.type = "Done";
  element.times = 1;
  glob_defined_list.push_back( element );
} // Set_type() 

void Set_compnd_defined_list() {
  Defined element;

  element.variable = "cin";
  element.data = "unknown";
  element.type = "Cin";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );
  
  element.variable = "cout";
  element.data = "unknown";
  element.type = "Cout";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "int";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "float";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "char";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "bool";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "string";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "void";
  element.data = "unknown";
  element.type = "DefType";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "if";
  element.data = "unknown";
  element.type = "Condition";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "else";
  element.data = "unknown";
  element.type = "Condition";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "while";
  element.data = "unknown";
  element.type = "Condition";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "do";
  element.data = "unknown";
  element.type = "Condition";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "return";
  element.data = "unknown";
  element.type = "Return";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );

  element.variable = "Done";
  element.data = "unknown";
  element.type = "Done";
  element.times = 1;
  glob_compnd_defined_list.push_back( element );
} // Set_compnd_defined_list()  

void Update_type() { // DefType Condition Return Done 
  for ( int i = 0; i < glob_codes.size() ; i++ ) {
    if ( glob_codes[i].token == "int" ) {
      glob_codes[i].type = "DefType";
    } // end if 
    else if ( glob_codes[i].token == "float" ) {
      glob_codes[i].type = "DefType";
    } // end if 
    else if ( glob_codes[i].token == "char" ) {
      glob_codes[i].type = "DefType";
    } // end if 
    else if ( glob_codes[i].token == "bool" ) {
      glob_codes[i].type = "DefType";
    } // end if 
    else if ( glob_codes[i].token == "string" ) {
      glob_codes[i].type = "DefType";
    } // end if 
    else if ( glob_codes[i].token == "void" ) {
      glob_codes[i].type = "DefType";
    } // end if 
    else if ( glob_codes[i].token == "if" ) {
      glob_codes[i].type = "Condition";
    } // end if
    else if ( glob_codes[i].token == "else" ) {
      glob_codes[i].type = "Condition";
    } // end if
    else if ( glob_codes[i].token == "while" ) {
      glob_codes[i].type = "Condition";
    } // end if
    else if ( glob_codes[i].token == "do" ) {
      glob_codes[i].type = "Condition";
    } // end if
    else if ( glob_codes[i].token == "return" ) {
      glob_codes[i].type = "Return";
    } // end if
    else if ( glob_codes[i].token == "Done" ) {
      glob_codes[i].type = "Done";
    } // end if
  } // end for 
} // Update_type() 

void Char_String_tab_newline() {
  int start = -1, end = -1, flag = 0;
  string str;

  for ( int i = 0; i < glob_codes.size() ; i++ ) { // ���B�zstring  
    if ( flag == 0 ) {
      if ( glob_codes[i].token == "\"" ) {
        flag = 1;
        start = i;
      } // end if
    } // end if 
    else { // flag == 1 
      str = str + glob_codes[i].token;

      if ( glob_codes[i].token == "\"" ) {
        flag = 0;
        end = i;
      } // end if
    } // end else 

    if ( start != -1 && end != -1 ) {
      str = ""; 
      for ( int j = start + 1; j < end ; j++ ) { // ���/" ����  
        if ( glob_codes[j].token == "\\n" ) {
          glob_codes[j].token = "\n";
        } // end if 
        else if ( glob_codes[j].token == "\\t" ) {
          glob_codes[j].token = "\t";
        } // end else if 

        str = str + glob_codes[j].token;
      } // end for

      // cout << "str: " << str << endl;
      glob_codes[start].token = str;
      glob_codes[start].type = "string";

      for ( int j = start + 1; j <= end ; j++ ) { // �M���w��token  
        glob_codes.erase( glob_codes.begin() + start + 1 );
        i--;
      } // end for

      start = -1;
      end = -1;
    } // end if 
    else if ( start != -1 && i == glob_codes.size() - 1 ) { // �S���ĤG��/"  
      
    } // end else if 
  } // end for 

  for ( int i = 0; i < glob_codes.size() ; i++ ) { // �B�zchar  
    if ( flag == 0 ) {
      if ( glob_codes[i].token == "\'" ) {
        flag = 1;
        start = i;
      } // end if
    } // end if 
    else { // flag == 1 
      str = str + glob_codes[i].token;

      if ( glob_codes[i].token == "\'" ) {
        flag = 0;
        end = i;
      } // end if
    } // end else 

    if ( start != -1 && end != -1 ) {
      str = ""; 
      for ( int j = start + 1; j < end ; j++ ) { // ���/' ����  
        if ( glob_codes[j].token == "\\n" ) {
          glob_codes[j].token = "\n";
        } // end if 
        else if ( glob_codes[j].token == "\\t" ) {
          glob_codes[j].token = "\t";
        } // end else if 

        str = str + glob_codes[j].token;
      } // end for

      // cout << "str: " << str << endl;
      glob_codes[start].token = str;
      glob_codes[start].type = "char";

      for ( int j = start + 1; j <= end ; j++ ) { // �M���w��token  
        glob_codes.erase( glob_codes.begin() + start + 1 );
        i--;
      } // end for

      start = -1;
      end = -1;
    } // end if 
    else if ( start != -1 && i == glob_codes.size() - 1 ) { // �S���ĤG��/'  
      
    } // end else if 
  } // end for 
} // Char_String_tab_newline() 

int Executed_or_Entered() { // �^�� -1:executed, else: �^��defined id��index  
  int nexttoken, result = -1;
  for ( int i = 0; i < glob_codes.size() ; i++ ) {
    if ( glob_codes[i].token == "{" ) {
      result = -1;
      i = glob_codes.size(); // break 
    } // end if 
    else if ( glob_codes[i].type == "DefType" ) {
      nexttoken = i + 1;
      nexttoken = FindToken( nexttoken );
      result = nexttoken;
      i = glob_codes.size(); // break 
    } // end if 
  } // end for 

  return result;
} // Executed_or_Entered() 

void ListAllVariables() {
  string variable, type, data;
  int i, j;

  i = 0;
  j = glob_defined_list.size() - 1 ;

  while ( i <= j ) { // bubblesort 
    for ( i = 0 ; i < j ; i++ ) {
      if ( glob_defined_list[i].variable > glob_defined_list[i + 1].variable ) {
        Defined tmp = glob_defined_list[i];
        glob_defined_list[i] = glob_defined_list[i + 1];
        glob_defined_list[i + 1] = tmp;
      } // end if    
    } // end for 

    j--;
    i = 0;
  } // end while 

  for ( int i = 0; i < glob_defined_list.size() ; i++ ) {
    if ( glob_defined_list[i].type == "Variable" ) {
      variable = glob_defined_list[i].variable;
      type = glob_defined_list[i].type;
      data = glob_defined_list[i].data;
      cout << variable << endl;
    } // end if    
  } // end for 
} // ListAllVariables() 

void ListVariable( string variable ) {
  int found = -1;
  for ( int i = 0; i < glob_defined_list.size() ; i++ ) {
    if ( glob_defined_list[i].variable == variable && glob_defined_list[i].type == "Variable" ) {
      found = i;
      cout << glob_defined_list[i].code << endl;
    } // end if    
  } // end for

  if ( found == -1 ) {
    cout << "Line " << glob_line << " : undefined identifier : \'" << variable << "\'" << endl;
  } // end if 
  else {
    cout << "Statement executed ..." << endl;
  } // end else 
} // ListVariable() 

void ListAllFunctions() {
  string variable, type, data;
  int i, j;

  i = 0;
  j = glob_defined_list.size() - 1 ;

  while ( i <= j ) { // bubblesort 
    for ( i = 0 ; i < j ; i++ ) {
      if ( glob_defined_list[i].variable > glob_defined_list[i + 1].variable ) {
        Defined tmp = glob_defined_list[i];
        glob_defined_list[i] = glob_defined_list[i + 1];
        glob_defined_list[i + 1] = tmp;
      } // end if    
    } // end for 

    j--;
    i = 0;
  } // end while 

  for ( int i = 0; i < glob_defined_list.size() ; i++ ) {
    if ( glob_defined_list[i].type == "Function" ) {
      variable = glob_defined_list[i].variable;
      type = glob_defined_list[i].type;
      data = glob_defined_list[i].data;
      cout << variable << "()" << endl;
    } // end if    
  } // end for 
} // ListAllFunctions() 

void ListFunction( string variable ) {
  int found = -1;
  for ( int i = 0; i < glob_defined_list.size() ; i++ ) {
    if ( glob_defined_list[i].variable == variable && glob_defined_list[i].type == "Function" ) {
      found = i;
      cout << glob_defined_list[i].code << endl;
    } // end if    
  } // end for

  if ( found == -1 ) {
    cout << "Line " << glob_line << " : undefined identifier : \'" << variable << "\'" << endl;
  } // end if 
  else {
    cout << "Statement executed ..." << endl;
  } // end else 
} // ListFunction() 

void Multi_Declaration( int end ) {
  for ( int i = glob_multi_declaration.size() - 1; i >= 0 ; i -- ) {
    string str = glob_multi_declaration[i].variable;
    if ( glob_multi_declaration[i].type == "Function" ) {
      if ( glob_multi_declaration[i].times > 1 ) {
        cout << "New definition of " << str << "() entered ..." << endl;
      }
      else {
        cout << "Definition of " << str << "() entered ..." << endl;
      } // end else 
    } // end if 
    else if ( glob_multi_declaration[i].type == "Variable" ) {
      if ( glob_multi_declaration[i].times > 1 ) {
        cout << "New definition of " << str << " entered ..." << endl;
      }
      else {
        cout << "Definition of " << str << " entered ..." << endl;
      } // end else 
    } // end else if 

    Declared_code( str, end );
  } // end for 
} // Multi_Declaration() 

void Declared_code( string variable, int end ) {
  string code = "", last_token = "", next_token;
  int times = 0;
  string indentation = ""; 
  int flag = 0, if_else_while = 0;
  for ( int i = 0; i < glob_defined_list.size() ; i++ ) {
    if ( glob_defined_list[i].variable == variable ) {
      for ( int j = 0; j <= end ; j++ ) {
        string str;
        str = glob_codes[j].token;

        if ( j != end ) {
          next_token = glob_codes[j + 1].token;
        } // end if 

        if ( next_token == "}" ) {
          times--;
        } // end if 

        indentation = "";
        for ( int k = 0; k < times ; k ++ ) {
          indentation = indentation + " "; // �@���Y���Y�@��  
        } // end for  

        if ( last_token == ";" ) { // ';' 
          code = code + "\n" + indentation;
        } // end if 
        else if ( last_token == "{" ) { // '{' 
          code = code + "\n" + indentation;
        } // end else if 
        else if ( last_token == "}" ) { // '}' 
          code = code + "\n" + indentation;
        } // end else if 
        else if ( last_token == "\n" && Is_White_Space( str ) == false ) {
          if ( if_else_while == 1 ) { // if����u���@��  
            if_else_while = 0;
            times--;
          } // end if 
        } // end else if 


        if ( code == "" ) { // first token 
          code = str + " ";
        } // end if 
        else if ( str == ";" ) { // ';' 
          code = code + " " + str;
        } // end if 
        else if ( str == "{" ) { // '{' 
          code = code + str;
        } // end if 
        else if ( str == "}" ) { // '}' 
          code = code + str;
        } // end if 
        else if ( str == "(" ) { // '}' 
          code = code + str + " ";
        } // end if 
        else if ( str == ")" ) { // '}' 
          code = code + " " + str;
        } // end if 
        else if ( str == "(" ) { // '}' 
          code = code + str + " ";
        } // end if 
        else if ( str == ")" ) { // '}' 
          code = code + str + " ";
        } // end if 
        else if ( str == "," ) {
          code = code + str + " ";
        } // end if 
        else if ( j != end && Token_list( str ) == true ) { // token_list 
          code = code + " " + str + " ";
        } // end if 
        else if ( glob_codes[j].type == "string" ) {
          code = code + "\"" + str + "\" "; 
        } // end if 
        else if ( glob_codes[j].type == "char" ) {
          code = code + "\'" + str + "\' ";  
        } // end if
        else if ( glob_codes[j].type == "DefType" ) { // DefType Condition
          code = code + str + " ";  
        } // end if
        else if ( glob_codes[j].type == "Condition" ) { // Condition 
          code = code + str + " ";  
        } // end if
        else if ( glob_codes[j].type == "Return" ) { // Return ( without 'Done' ) 
          code = code + str + " ";  
        } // end if
        else if ( j != end && Is_White_Space( str ) == false ) { // ident 
          code = code + str;
        } // end if 
        else if ( j == end && Is_White_Space( str ) == false ) { // last token 
          code = code + str;
        } // end else if 

        last_token = str;

        if ( str == "{" && if_else_while == 0 ) {
          times++;
        } // end if 
        else if ( str == "if" && if_else_while == 0 ) {
          times++;
          if_else_while = 1;
        } // end if 
        else if ( str == "else" && if_else_while == 0 ) {
          times++;
          if_else_while = 1;
        } // end else if 
        else if ( str == "while" && if_else_while == 0 ) {
          times++;
          if_else_while = 1;
        } // end else if 
      } // end for 

      glob_defined_list[i].code = code;
      i = glob_defined_list.size(); // break 
    } // end if    
  } // end for
} // Declared_code() 

// ********************start of definition******************** 

ReturnResult User_input( ReturnResult index, int last_i ) { // user_input, stats 1 2, last_i 
  int token_i = FindToken( index.i );
  int save_i, last_correct_i;
  vector<int> farthest;


  if ( index.stats == 1 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      save_i = index.i;
      index = Definition( index ); // definition 

      if ( index.result == false ) {
        farthest.push_back( index.i );
        index.i = save_i;
        index = Statement( index ); // statement 

        if ( index.result == false ) {
          farthest.push_back( index.i );
          index.i = FindFarthest( farthest, index.i );
        } // end if 
    
        //        if ( index.result == true ) {
        //          last_correct_i = index.i;
        //          index.i++;
        //          index.stats = 2;
        //          index = User_input( index, last_correct_i ); // �~�����   
        //        } // end if
      } // end if 
      //      else {
      //        last_correct_i = index.i;
      //        index.i++;
      //        index.stats = 2;
      //        index = User_input( index, last_correct_i ); // �~�����   
      //      } // end else 
    } // end else 
  } // end if 
  //  else if ( index.stats == 2 ) { // �i���i�L  
  //    if ( token_i == -1 ) { // �S����h�F  
  //      index.result = true;
  //    } // end if 
  //    else {
  //      index.i = token_i;
  //      save_i = index.i;
  //      index = Definition( index ); // definition 
  //
  //      if ( index.result == false ) {
  //        index.i = save_i;
  //        index = Statement( index ); // statement 
  //
  //        if ( index.result == true ) {
  //          last_correct_i = index.i;
  //          index.i++;
  //          index.stats = 2;
  //          index = User_input( index, last_correct_i ); // �~�����   
  //        } // end if 
  //        else {
  //          index.i = last_i;
  //          index.result = true;
  //        } // end else 
  //      } // end if 
  //      else {
  //        last_correct_i = index.i;
  //        index.i++;
  //        index.stats = 2;
  //        index = User_input( index, last_correct_i ); // �~�����   
  //      } // end else 
  //    } // end else 
  //
  //    if ( index.result == false ) {
  //      index.i = last_i;
  //      index.result = true;
  //    } // end if 
  //    else if ( index.error == 2 ) { // ����H���ٻݭninput 
  //      index.error = 0;
  //   } // end else if 
  // } // end else if

  farthest.clear();
  return index;
} // User_input() 

ReturnResult Definition( ReturnResult index ) { // �w�qfunc or �ŧi�ܼ�  
  int save_i, last_correct_i, ident_i;
  int token_i = FindToken( index.i );
  vector <int> farthest;

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( glob_codes[token_i].token == "void" ) {
    // 1.�w�qvoid func 
    index.i = token_i;
    token_i++;
    token_i = FindToken( token_i );

    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func )
      ident_i = token_i;
      index.i = token_i;
      last_correct_i = index.i;

      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if
      else {
        index.i = token_i;
        index = Function_definition( index, last_correct_i ); // function_definition_without_ID 
      } // end else 

      if ( index.result == true ) {  // add IDENT to defined-list
        if ( glob_cmpnd_statmnt == 1 && Find_compnd_IDENT( glob_codes[ident_i].token ) == false ) {
          glob_codes[ident_i].type = "IDENT";

          Defined element;
          element.variable = glob_codes[ident_i].token;
          element.data = "unknown";
          element.type = "Function";
          element.code = "";
          element.times = 1;
          glob_compnd_defined_list.push_back( element );
        } // end if 
        else if ( Find_IDENT( glob_codes[ident_i].token ) == false ) {
          glob_codes[ident_i].type = "IDENT";

          Defined element;
          element.variable = glob_codes[ident_i].token;
          element.data = "unknown";
          element.type = "Function";
          element.code = "";
          element.times = 1;
          glob_defined_list.push_back( element );
        } // end else if 
        else {
          Defined tmp;
          tmp = Get_IDENT_value( glob_codes[ident_i].token );
          if ( tmp.type == "Function" ) {
            Overwrite_IDENT( tmp );
          } // end if 
        } // end else 
      } // end if
    } // end else if
    else {
      index.i = token_i;
      index.result = false;
    } // end else 
  } // end else if 
  else {
    index.i = token_i;
    index = Type_specifier( index ); // type_specifier 

    if ( index.result == true ) {
      if ( glob_type_wrong == index.i ) {
        index.undefined = index.i;
      } // end if 

      token_i = index.i + 1;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if 
      else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func ) 
        ident_i = token_i;
        index.i = token_i;
        last_correct_i = index.i;

        token_i++;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if
        else {
          index.i = token_i;
          save_i = index.i;
          // 2.�w�qchar int...�� func 
          index = Function_definition( index, last_correct_i ); // function_definition_without_ID 

          if ( index.result == false ) {
            farthest.push_back( index.i );
            index.i = save_i;
            // 3.�ŧichar int...���ܼ� 
            index.stats = 1;
            index = Rest_of_declarators( index ); // rest_of_declarators

            if ( index.result == true ) {  // add IDENT to defined-list
              if ( glob_cmpnd_statmnt == 1 && Find_compnd_IDENT( glob_codes[ident_i].token ) == false ) {
                glob_codes[ident_i].type = "IDENT";

                Defined element;
                element.variable = glob_codes[ident_i].token;
                element.data = "unknown";
                element.type = "Function";
                element.code = "";
                element.times = 1;
                glob_compnd_defined_list.push_back( element );
              } // end if
              else if ( Find_IDENT( glob_codes[ident_i].token ) == false ) {
                glob_codes[ident_i].type = "IDENT";

                Defined element;
                element.variable = glob_codes[ident_i].token;
                element.data = "unknown";
                element.type = "Variable";
                element.code = "";
                element.times = 1;
                glob_defined_list.push_back( element );
              } // end else if 
              else {
                Defined tmp;
                tmp = Get_IDENT_value( glob_codes[ident_i].token );
                if ( tmp.type == "Variable" ) {
                  Overwrite_IDENT( tmp );
                } // end if 
              } // end else 
            } // end if 
            else {
              farthest.push_back( index.i );
              index.i = FindFarthest( farthest, index.i );
              index.result = false;
            } // end else 
          } // end if 
          else { // add IDENT to defined-list 
            if ( Find_IDENT( glob_codes[ident_i].token ) == false ) {
              glob_codes[ident_i].type = "IDENT";

              Defined element;
              element.variable = glob_codes[ident_i].token;
              element.data = "unknown";
              element.type = "Function";
              element.code = "";
              element.times = 1;
              glob_defined_list.push_back( element );
            } // end else if 
            else {
              Defined tmp;
              tmp = Get_IDENT_value( glob_codes[ident_i].token );
              if ( tmp.type == "Function" ) {
                Overwrite_IDENT( tmp );
              } // end if 
            } // end else 
          } // end else 
        } // end else 
      } // end else if 
      else {
        index.i = token_i;
        index.result = false;
      } // end else 

      if ( index.result == false ) { // �N��type�����L �̫ᤴ�� ���N�Ptype�O�_��L��  
        index.undefined = -1;
        glob_type_wrong = -1;
      } // end if 
      else if ( index.error == 2 ) { // if waiting remaining input 
        index.undefined = -1;
      } // end else if 
      else if ( index.undefined != -1 ) { // String a, b, ... ����del  
        for ( int i = 0; i <= index.i ; i++ ) {
          if ( glob_codes[i].type != "string" && glob_codes[i].type != "char" ) {
            if ( glob_codes[i].type != "DefType" && glob_codes[i].type != "Condition" ) {
              if ( glob_codes[i].type != "Return") {
                if ( Find_IDENT( glob_codes[i].token ) == true ) {
                  Del_IDENT( glob_codes[i].token );
                } // end if 
              } // end if 
            } // end if
          } // end if 
        } // end for 
      } // end else if 
    } // end if 
  } // end else 

  farthest.clear();
  return index;
} // Definition() 

ReturnResult Type_specifier( ReturnResult index ) { // ���Fvoid�H�~�����O  
  // int, char, float, string, bool
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true; 
  } // end if 
  else if ( glob_codes[token_i].token == "int" || glob_codes[token_i].token == "char" ) {
    index.i = token_i;
    index.result = true;
  } // end else if 
  else if ( glob_codes[token_i].token == "float" || glob_codes[token_i].token == "string" ) {
    index.i = token_i;
    index.result = true;
  } // end else if
  else if ( glob_codes[token_i].token == "bool" ) {
    index.i = token_i;
    index.result = true;
  } // end else if 
  else if ( glob_codes[token_i].token == "cin" || glob_codes[token_i].token == "cout" ) {
    index.i = token_i;
    index.result = false;
  } // end else if
  else if ( glob_codes[token_i].token == "void" || glob_codes[token_i].token == "if" ) {
    index.i = token_i;
    index.result = false;
  } // end else if
  else if ( glob_codes[token_i].token == "else" || glob_codes[token_i].token == "while" ) {
    index.i = token_i;
    index.result = false;
  } // end else if
  else if ( glob_codes[token_i].token == "do" || glob_codes[token_i].token == "return" ) {
    index.i = token_i;
    index.result = false;
  } // end else if
  else if ( glob_codes[token_i].token == "Done" ) {
    index.i = token_i;
    index.result = false;
  } // end else if
  else {
    if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // ����@�Otrue  
      index.i = token_i;
      index.result = true;
      glob_type_wrong = token_i;
    } // end if 
    else {
      index.i = token_i;
      index.result = false;
    } // end else 
  } // end else 

  return index;  
} // Type_specifier()  

ReturnResult Rest_of_declarators( ReturnResult index ) { // rest_of_declarators, stats 1 2 3 
  int save_i, ident_i;
  int token_i = FindToken( index.i );

  if ( index.stats == 1 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      save_i = index.i;
      if ( glob_codes[token_i].token == "[" ) { // �i���i�L [ '[' Constant ']' ]  
        index.i = token_i;
        token_i++;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else if ( Is_CONST( glob_codes[token_i].type ) == true ) {
          index.i = token_i;
          token_i++;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else if ( glob_codes[token_i].token == "]" ) {
            index.i = token_i + 1;
            index.result = true;
            index.stats = 2;
            index = Rest_of_declarators( index ); // �~�򩹤U����  
          }  // end else if 
          else { // �߱󦹥i���i�L ���U����  
            index.i = save_i;
            index.stats = 2;
            index = Rest_of_declarators( index );
          } // end else 
        } // end else if 
        else { // �߱󦹥i���i�L ���U����  
          index.i = save_i;
          index.stats = 2;
          index = Rest_of_declarators( index );
        } // end else 
      } // end if 
      else { // �߱󦹥i���i�L ���U����   
        index.i = token_i;
        index.stats = 2;
        index = Rest_of_declarators( index );
      } // end else 
    } // end else 
  } // end if 
  else if ( index.stats == 2 ) { // 0��h��  
    save_i = index.i;
    if ( glob_codes[token_i].token == "," ) {
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if 
      else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func ) 
        ident_i = token_i;
        index.i = token_i;
        save_i = index.i; 

        token_i++;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // �S����h�F  
          index.i++;
          index.result = true;
          index.stats = 3;
          index = Rest_of_declarators( index ); // �~�򩹤U����  
        } // end if
        else if ( glob_codes[token_i].token == "[" ) { // �i���i�L 
          index.i = token_i;
          token_i++;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else if ( Is_CONST( glob_codes[token_i].type ) == true ) {
            index.i = token_i;
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if 
            else if ( glob_codes[token_i].token == "]" ) {
              index.i = token_i + 1;
              token_i++;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // �S����h�F 
                index.i++; 
                index.result = true;
                index.stats = 3;
                index = Rest_of_declarators( index ); // �~�򩹤U����  
              } // end if
              else {
                index.result = true;
                index.stats = 2;
                index = Rest_of_declarators( index ); // �~��0��h�� 
              } // end else   
            } // end else if 
            else {
              index.result = true;
              index.i = save_i + 1; // �h�^ident���U��token  
              index.stats = 3;
              index = Rest_of_declarators( index ); // �~�򩹤U����  
            } // end else 
          } // end else if 
          else {
            index.result = true;
            index.i = save_i + 1; // �h�^ident���U��token  
            index.stats = 3;
            index = Rest_of_declarators( index ); // �~�򩹤U����  
          } // end else 
        } // end else if 
        else {
          index.result = true;
          index.i = token_i; // �h�^ident���U�@��token  
          index.stats = 2;
          index = Rest_of_declarators( index ); // �~��0��h��   
        } // end else 

        if ( index.result == true ) {  // add IDENT to defined-list 
          if ( glob_cmpnd_statmnt == 1 && Find_compnd_IDENT( glob_codes[ident_i].token ) == false ) {
            glob_codes[ident_i].type = "IDENT";

            Defined element;
            element.variable = glob_codes[ident_i].token;
            element.data = "unknown";
            element.type = "Function";
            element.code = "";
            element.times = 1;
            glob_compnd_defined_list.push_back( element );
          } // end if
          else if ( Find_IDENT( glob_codes[ident_i].token ) == false ) {
            glob_codes[ident_i].type = "IDENT";

            Defined element;
            element.variable = glob_codes[ident_i].token;
            element.data = "unknown";
            element.type = "Variable";
            element.code = "";
            element.times = 1;
            glob_defined_list.push_back( element );
            glob_multi_declaration.push_back( element );
          } // end else if 
          else {
            Defined tmp;
            tmp = Get_IDENT_value( glob_codes[ident_i].token );
            if ( tmp.type == "Variable" ) {
              tmp = Overwrite_IDENT( tmp );
              glob_multi_declaration.push_back( tmp );
            } // end if 
          } // end else 
        } // end if
      } // end else if
      else { // not ident 
        index.i = token_i;
        index.result = false;
      } // end else 
    } // end if 
    else {
      index.result = true;
      index.i = token_i;
      index.stats = 3;
      index = Rest_of_declarators( index ); // �~�򩹤U����  
    } // end else 
  } // end else if 
  else if ( index.stats == 3 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      if ( glob_codes[token_i].token == ";" ) {
        index.i = token_i;
        index.result = true;
      } // end if
      else {
        index.i = token_i;
        index.result = false;
      } // end else  
    } // end else 
  } // end else if 

  return index;
} // Rest_of_declarators() 

ReturnResult Function_definition( ReturnResult index, int last_i ) { 
// �w�qfunc,  function_definition_without_ID 
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( glob_codes[token_i].token == "(" ) {
    index.i = token_i;
    token_i++;
    token_i = FindToken( token_i );

    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else if ( glob_codes[token_i].token == ")" ) { // '()' 
      index.i = token_i;
      index.result = true;

      index.i++;
      index.stats = 1;
      index = Compound_statement( index, index.i ); // compound_statement 
    } // end else if 
    else if ( glob_codes[token_i].token == "void" ) { // �i���i�L void  
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == ")" ) {
        index.i = token_i;
        index.result = true;

        index.i++;
        index.stats = 1;
        index = Compound_statement( index, index.i ); // compound_statement 
      } // end else if 
      else {
        index.i = token_i;
        index.result = false;
      } // end else 
    } // end else if
    else { // �i���i�L formal_parameter_list  
      index.i = token_i;
      index.stats = 1;
      index = Formal_parameter_list( index, last_i ); // formal_parameter_list 

      if ( index.result == true ) {
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == ")" ) {
          index.i = token_i;
          index.result = true;

          index.i++;
          index.stats = 1;
          index = Compound_statement( index, index.i ); // compound_statement 
        } // end else if 
        else {
          index.i = token_i;
          index.result = false;
        } // end else 
      } // end if 
    } // end else 
  } // end else if 
  else {
    index.i = token_i;
    index.result = false;
  } // end else 

  return index;
} // Function_definition() 

ReturnResult Formal_parameter_list( ReturnResult index, int last_i ) { 
// formal_parameter_list , stats 1 2, last_i 
  int save_i, last_correct_i, ident_i;
  int token_i = FindToken( index.i );

  if ( index.stats == 1 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index = Type_specifier( index );

      if ( index.result == true ) {
        if ( glob_type_wrong == index.i ) {
          index.undefined = index.i;
        } // end if 

        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == "&" ) { // �i���i�L'&'  
          index.i = token_i;
          token_i++;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.result = true;
            index.stats = 2;
          } // end if 
          else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func ) 
            ident_i = token_i;
            index.i = token_i;
            save_i = index.i; // �x�sident��m  
            last_correct_i = index.i;

            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // �S����h�F 
              index.result = true;
              index.stats = 2;
              index.i++;
              index = Formal_parameter_list( index, last_correct_i );
            } // end if 
            else if ( glob_codes[token_i].token == "[" ) { // �i���i�L  
              index.i = token_i;
              token_i++;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // waiting for remaining input 
                index.error = 2;
                index.result = true;
              } // end if 
              else if ( Is_CONST( glob_codes[token_i].type ) == true ) { // const 
                index.i = token_i;
                token_i++;
                token_i = FindToken( token_i );

                if ( token_i == -1 ) { // waiting for remaining input 
                  index.error = 2;
                  index.result = true; 
                } // end if 
                else if ( glob_codes[token_i].token == "]" ) {
                  index.result = true;
                  index.i = token_i;
                  last_correct_i = index.i;
                  index.stats = 2;
                  index.i++;
                  index = Formal_parameter_list( index, last_correct_i ); // �~�򩹤U���� 
                } // end else if 
                else { // �h�^ident�U�@��token  
                  last_correct_i = save_i; // ident 
                  index.i = save_i + 1; // ident�U�@��  
                  index.result = true;
                  index.stats = 2;
                  index = Formal_parameter_list( index, last_correct_i ); // �~�򩹤U���� 
                } // end else 
              } // end else if 
              else { // �h�^ident�U�@��token 
                last_correct_i = save_i; 
                index.i = save_i + 1;
                index.result = true;
                index.stats = 2;
                index = Formal_parameter_list( index, last_correct_i ); // �~�򩹤U���� 
              } // end else 
            } // end else if  
            else {
              index.result = true;
              index.stats = 2;
              index.i++; // ident�U�@��  
              index = Formal_parameter_list( index, last_correct_i ); // �~�򩹤U���� 
            } // end else 

            if ( index.result == true ) {  // add IDENT to defined-list 
              if ( Find_compnd_IDENT( glob_codes[ident_i].token ) == false ) {
                glob_codes[ident_i].type = "IDENT";

                Defined element;
                element.variable = glob_codes[ident_i].token;
                element.data = "unknown";
                element.type = "Variable";
                element.code = "";
                element.times = 1;
                glob_compnd_defined_list.push_back( element );
              } // end else if
//              else {
//                Defined tmp;
//                tmp = Get_IDENT_value( glob_codes[ident_i].token );
//                if ( tmp.type == "Variable" ) {
//                  Overwrite_IDENT( tmp );
//                } // end if 
//              } // end else 
            } // end if
          } // end else if 
          else {
            index.i = token_i;
            index.result = false;
          } // end else 
        } // end else if 
        else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func ) 
          ident_i = token_i;
          index.i = token_i;
          save_i = index.i; // �x�sident��m  
          last_correct_i = index.i;   

          token_i++;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // �S����h�F 
            index.result = true;
            index.stats = 2;
            index.i++;
            index = Formal_parameter_list( index, last_correct_i );
          } // end if 
          else if ( glob_codes[token_i].token == "[" ) { // �i���i�L  
            index.i = token_i;
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if 
            else if ( Is_CONST( glob_codes[token_i].type ) == true ) { // const 
              index.i = token_i;
              token_i++;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // waiting for remaining input 
                index.error = 2;
                index.result = true;
              } // end if 
              else if ( glob_codes[token_i].token == "]" ) {
                index.result = true;
                index.i = token_i;
                last_correct_i = index.i;
                index.stats = 2;
                index.i++;
                index = Formal_parameter_list( index, last_correct_i ); // �~�򩹤U���� 
              } // end else if 
              else { // �h�^ident�U�@��token 
                last_correct_i = save_i; // ident 
                index.i = save_i + 1; // ident�U�@��  
                index.result = true;
                index.stats = 2;
                index = Formal_parameter_list( index, last_correct_i ); // �~�򩹤U���� 
              } // end else 
            } // end else if 
            else { // �h�^ident�U�@��token 
              last_correct_i = save_i;  
              index.i = save_i + 1;
              index.result = true;
              index.stats = 2;
              index = Formal_parameter_list( index, last_correct_i ); // �~�򩹤U���� 
            } // end else 
          } // end else if  
          else {
            index.result = true;
            index.stats = 2;
            index.i++;
            index = Formal_parameter_list( index, last_correct_i ); // �~�򩹤U���� 
          } // end else 

          if ( index.result == true ) {  // add IDENT to defined-list 
            if ( Find_compnd_IDENT( glob_codes[ident_i].token ) == false ) {
              glob_codes[ident_i].type = "IDENT";

              Defined element;
              element.variable = glob_codes[ident_i].token;
              element.data = "unknown";
              element.type = "Variable";
              element.code = "";
              element.times = 1;
              glob_compnd_defined_list.push_back( element );
            } // end else if
//          else {
//             Defined tmp;
//             tmp = Get_IDENT_value( glob_codes[ident_i].token );
//                if ( tmp.type == "Variable" ) {
//                  Overwrite_IDENT( tmp );
//                } // end if 
//              } // end else 
            } // end if
        } // end else if 
        else {
          index.i = token_i;
          index.result = false;
        } // end else 

        if ( index.result == false ) { // �N��type�����L �̫ᤴ�� ���N�Ptype�O�_��L��  
          index.undefined = -1;
          glob_type_wrong = -1;
        } // end if 
        else if ( index.error == 2 ) { // if waiting remaining input 
          index.undefined = -1;
        } // end else if 
      } // end if 
    } // end else 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // �S����h�F  
      index.result = true;
    } // end if
    else {
      if ( glob_codes[token_i].token == "," ) { // 0��h��  
        index.i = token_i;
        last_correct_i = index.i;
        token_i++;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else {
          index.i = token_i;
          index.stats = 1;
          index = Formal_parameter_list( index, last_correct_i ); // �~�򩹤U���� 

          if ( index.result == false ) {
            // index.i = last_i; // �h�^0�� 
            // index.result = true;
          } // end if 
          else if ( index.error == 2 ) { // ����H���ٻݭninput 
            index.error = 0;
          } // end else if 
        } // end else 
      } // end if 
      else { // 0��  
        index.i = last_i;
        index.result = true;
      } // end else 
    } // end else 
  } // end else if 

  return index;
} // Formal_parameter_list() 

ReturnResult Compound_statement( ReturnResult index, int last_i ) { // Compound_statement, stats 1 2 
  // '{' + statement or declaration + '}'
  int token_i = FindToken( index.i );
  int save_i, last_correct_i;
  vector<int> farthest;

  if ( index.stats == 1 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else if ( glob_codes[token_i].token == "{" ) {
      index.i = token_i;
      save_i = index.i; // �x�s'{'��m  
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "}" ) { // �u��'{}'  
        index.i = token_i;
        index.result = true;
      } // end else if 
      else {
        last_correct_i = index.i;
        index.i = token_i;
        index.stats = 2;
        index = Compound_statement( index, last_correct_i ); 
      } // end else 
    } // end else if 
    else {
      index.i = token_i;
      index.result = false;
    } // end else
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      save_i = index.i; // �x�stoken��m  
      index = Statement( index ); // statement  

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        index.i++;
        index.stats = 2;
        index = Compound_statement( index, last_correct_i ); // compound_statement�~�����    
      } // end if 
      else {
        farthest.push_back( index.i );
        index.i = save_i;
        index = Declaration( index );

        if ( index.result == true ) {
          last_correct_i = index.i;
          index.i++;
          index.stats = 2;
          index = Compound_statement( index, last_correct_i ); // compound_statement�~�����   
        } // end if 
        else {
          farthest.push_back( index.i );
          index.i = save_i;
          if ( glob_codes[index.i].token == "}" ) {
            index.result = true;
          } // end if 
          else {
            index.i = FindFarthest( farthest, index.i );
            index.result = false;
          } // end else 
        } // end else 
      } // end else    
    } // end else 
  } // end else if

  if ( index.result == true ) {
    glob_cmpnd_statmnt = 1;
  } // end if 

  farthest.clear();
  return index;
} // Compound_statement() 

ReturnResult Declaration( ReturnResult index ) {   // �ŧi�ܼ�  
  int token_i = FindToken( index.i );
  int save_i, ident_i;
  
  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else {
    index.i = token_i;
    index = Type_specifier( index );

    if ( index.result == true ) {
      if ( glob_type_wrong == index.i ) {
        index.undefined = index.i;
      } // end if 

      token_i = index.i + 1;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if 
      else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func ) 
        ident_i = token_i;
        index.i = token_i;
        save_i = index.i;   

        token_i++;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else {
          index.i = token_i;
          index.stats = 1;
          index = Rest_of_declarators( index );
        } // end else if 

        if ( index.result == true ) {  // add IDENT to defined-list
          if ( Find_compnd_IDENT( glob_codes[ident_i].token ) == false ) {
            glob_codes[ident_i].type = "IDENT";

            Defined element;
            element.variable = glob_codes[ident_i].token;
            element.data = "unknown";
            element.type = "Variable";
            element.code = "";
            element.times = 1;
            glob_compnd_defined_list.push_back( element );
          } // end if 
//          else {
//            Defined tmp;
//            tmp = Get_IDENT_value( glob_codes[ident_i].token );
//            if ( tmp.type == "Variable" ) {
//              Overwrite_IDENT( tmp );
//            } // end if 
//          } // end else 
        } // end if
      } // end else if 
      else {
        index.i = token_i;
        index.result = false;
      } // end else 

      if ( index.result == false ) { // �N��type�����L �̫ᤴ�� ���N�Ptype�O�_��L��  
        index.undefined = -1;
        glob_type_wrong = -1;
      } // end if 
      else if ( index.error == 2 ) { // if waiting remaining input 
        index.undefined = -1;
      } // end else if 
    } // end if 
  } // end else 

  return index;
} // Declaration() 



// ********************end of definition******************** 
// ####################start of statement####################

ReturnResult Statement( ReturnResult index ) { // statement 
  int save_i;
  int token_i = FindToken( index.i );
  vector <int> farthest;

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else {
    index.i = token_i;
    save_i = index.i;
    if ( glob_codes[token_i].token == "if" ) {
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "(" ) {
        index.i = token_i + 1;
        index.stats = 1;
        index = Expression( index, token_i ); // expression 

        if ( index.result == true ) {
          token_i = index.i + 1;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else if ( glob_codes[token_i].token == ")" ) {
            index.i = token_i + 1;
            index = Statement( index ); // statement 

            if ( index.result == true ) {
              save_i = index.i;
              token_i = index.i + 1;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // waiting for remaining input 
                index.error = 2;
                index.result = true;
              } // end if 
              else if ( glob_codes[token_i].token == "else" ) { // �i���i�Lelse  
                index.i = token_i + 1;
                index = Statement( index ); // statement 

                if ( index.result == false ) { // �h�^ if statement  
                  index.i = save_i;
                  index.result = true;  
                } // end if 
              } // end else if 
              else { // �S����h�F  
                index.result = true; 
              } // end else 
            } // end if 
          } // end if 
          else {
            index.i = token_i;
            index.result = false;
          } // end else 
        } // end if 
      } // end else if 
      else {
        index.i = token_i;
        index.result = false;
      } // end else 
    } // end if 
    else if ( glob_codes[token_i].token == "do" ) {
      index.i = token_i + 1;
      index = Statement( index ); // statement 

      if ( index.result == true ) {
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == "while" ) {
          index.i = token_i;
          token_i++;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else if ( glob_codes[token_i].token == "(" ) {
            index.i = token_i + 1;
            index.stats = 1;
            index = Expression( index, token_i ); // expression 

            if ( index.result == true ) {
              token_i = index.i + 1;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // waiting for remaining input 
                index.error = 2;
                index.result = true;
              } // end if 
              else if ( glob_codes[token_i].token == ")" ) {
                index.i = token_i;
                token_i++;
                token_i = FindToken( token_i );

                if ( token_i == -1 ) { // waiting for remaining input 
                  index.error = 2;
                  index.result = true;
                } // end if 
                else if ( glob_codes[token_i].token == ";" ) {
                  index.i = token_i;
                  index.result = true;
                } // end else if 
                else {
                  index.i = token_i;
                  index.result = false;
                } // end else 
              } // end else if 
              else {
                index.i = token_i;
                index.result = false;
              } // end else 
            } // end if 
          } // end else if 
          else {
            index.i = token_i;
            index.result = false;
          } // end else 
        } // end else if 
        else {
          index.i = token_i;
          index.result = false;
        } // end else 
      } // end if      
    } // end else if 
    else if ( glob_codes[token_i].token == "while" ) {
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "(" ) {
        index.i = token_i + 1;
        index.stats = 1;
        index = Expression( index, token_i ); // expression 

        if ( index.result == true ) {
          token_i = index.i + 1;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else if ( glob_codes[token_i].token == ")" ) {
            index.i = token_i + 1;
            index = Statement( index ); // statement 
          } // end if 
          else {
            index.i = token_i;
            index.result = false;
          } // end else 
        } // end if 
      } // end else if 
      else {
        index.i = token_i;
        index.result = false;
      } // end else 
    } // end else if 
    else if ( glob_codes[token_i].token == "return" ) {
      save_i = token_i;
      index.i = token_i + 1;
      index.stats = 1;
      index = Expression( index, token_i ); // expression 

      if ( index.result == true ) { // �i���i�L expression 
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == ";" ) {
          index.i = token_i;
          index.result = true;
        } // end else if 
        else {
          index.i = token_i;
          index.result = false;
        } // end else 
      } // end if  
      else {
        index.i = save_i; // �b'return' token  
        token_i++;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == ";" ) {
          index.i = token_i;
          index.result = true;
        } // end else if 
        else {
          index.i = token_i;
          index.result = false;
        } // end else 
      } // end else 
    } // end else if 
    else if ( glob_codes[token_i].token == ";" ) { // null statement 
      index.i = token_i;
      index.result = true;
    } // end else if 
    else {
      index.i = save_i;

      index.stats = 1;
      index = Expression( index, index.i ); // expression 

      if ( index.result == true ) {
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == ";" ) {
          index.i = token_i;
          index.result = true;
        } // end else if 
        else {
          index.i = token_i;
          index.result = false;
        } // end else 
      } // end if 
      else {
        farthest.push_back( index.i );
        index.i = save_i;
        index.stats = 1;
        index = Compound_statement( index, index.i ); // compound_statement 

        if ( index.result == false ) {
          farthest.push_back( index.i );
          index.i = FindFarthest( farthest, index.i );
          index.result = false;
        } // end if 
      } // end else 
    } // end else
  } // end else 

  farthest.clear();
  return index;
} // Statement() 

ReturnResult Expression( ReturnResult index, int last_i ) { // expression, stats 1 2, last_i 
//  �@�}�llast_i�ǤJԣ�������n, ��var���j�Ǹ�ƥ�  
  int token_i = FindToken( index.i );
  int save_i, last_correct_i;
 
  if ( index.stats == 1 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if
    else {
      index.i = token_i;
      index.stats = 1;
      index = Basic_expression( index );

      if ( index.result == true ) {
        last_correct_i = index.i;
        index.i++;
        index.stats = 2;
        index = Expression( index, last_correct_i ); // �A�դ@��
      } // end if 
    } // end else 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // �S����h�F  
      index.result = true;
    } // end if 
    else if ( glob_codes[token_i].token == "," ) {
      index.i = token_i + 1;
      index.stats = 1;
      index = Basic_expression( index );   

      if ( index.result == false ) {
        // index.i = last_i;
        // index.result = true;
      } // end if
      else {
        last_correct_i = index.i;
        index.i++;
        index.stats = 2;
        index = Expression( index, last_correct_i ); // �A�դ@��
      } // end else 
    } // end else if
    else { // 0��  
      index.i = last_i;
      index.result = true;
    } // end else 
  } // end else if 

  return index;
} // Expression() 

ReturnResult Basic_expression( ReturnResult index ) { // basic_expression, stats 1 2 
  int last_correct_i, save_i;
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if
  else if ( index.stats == 1 ) {
    if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func )
      index.i = token_i;

      if ( Find_compnd_IDENT( glob_codes[token_i].token ) == true ) {
        index.result = true; // ���򻼰j  
        // tmp.error = 4; // undefined
      } // end if
      else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // �|���Q�w�q  
        index.result = true; // ���򻼰j  
        if ( index.undefined == -1 && Is_CONST( glob_codes[token_i].type ) == false ) { // �����Ĥ@��undefined  
          index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
        } // end if 
        // tmp.error = 4; // undefined
      } // end else if 
      else {
        if ( glob_codes[token_i].type == "unknown" ) {
          index.result = true;
          glob_codes[token_i].type = "IDENT"; // ��{int x;}��statement executed  
        } // end if 
      } // end else    

      if ( glob_codes[token_i].token != "else" ) {
        token_i++;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if  
        else {
          last_correct_i = index.i;
          index.i = token_i;
          index.stats = 1;
          index = Roisbe( index, last_correct_i ); // rest_of_Identifier_started_basic_exp 
        } // end else 
      } // end if 
      else {
        index.i = token_i;
        index.result = false;
      } // end else 
    } // end if 
    else if ( glob_codes[token_i].token == "+" ) { //  PP 
      save_i = token_i;
      if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
        index.i = token_i;
        index.error = 2;
        index.result = true; 
      } // end else if 
      else if ( glob_codes[token_i + 1].token == "+" ) {
        index.i = token_i + 1;
        token_i = token_i + 2;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func )
          index.i = token_i;

          if ( Find_compnd_IDENT( glob_codes[token_i].token ) == true ) {
            index.result = true; // ���򻼰j  
          } // end if
          else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // �|���Q�w�q  
            index.result = true; // ���򻼰j  
            string type = glob_codes[token_i].type;
            if ( index.undefined == -1 && Is_CONST( type ) == false ) { // �����Ĥ@��undefined  
              index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
            } // end if 
            // tmp.error = 4; // undefined
          } // end else if 
          else {
            if ( glob_codes[token_i].type == "unknown" ) {
              index.result = true;
              glob_codes[token_i].type = "IDENT"; // ��{int x;}��statement executed 
            } // end if  
          } // end else    

          if ( glob_codes[token_i].token != "else" ) {
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if  
            else {
              last_correct_i = index.i;
              index.i = token_i;
              index.stats = 1;
              index = Ropisbe( index, last_correct_i ); // rest_of_PPMM_Identifier_started_basic_exp
            } // end else 
          } // end if 
        else {
            index.i = token_i;
            index.result = false;
          } // end else
        } // end else if 
        else {
          index.i = token_i;
          index.result = false;
        } // end else 
      } // end else if 
      else {
        index.i = save_i;
        index.stats = 2;
        index = Basic_expression( index ); // �~�򩹤U����  
      } // end else 
    } // end else if
    else if ( glob_codes[token_i].token == "-" ) { //  MM 
      save_i = token_i;
      if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
        index.i = token_i;
        index.error = 2;
        index.result = true; 
      } // end else if 
      else if ( glob_codes[token_i + 1].token == "-" ) {
        index.i = token_i + 1;
        token_i = token_i + 2;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func )
          index.i = token_i;

          if ( Find_compnd_IDENT( glob_codes[token_i].token ) == true ) {
            index.result = true; // ���򻼰j  
          } // end if
          else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // �|���Q�w�q  
            index.result = true; // ���򻼰j  
            string type = glob_codes[token_i].type;
            if ( index.undefined == -1 && Is_CONST( type ) == false ) { // �����Ĥ@��undefined  
              index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
            } // end if 
            // tmp.error = 4; // undefined
          } // end else if 
          else {
            if ( glob_codes[token_i].type == "unknown" ) {
              index.result = true;
              glob_codes[token_i].type = "IDENT"; // ��{int x;}��statement executed 
            } // end if 
          } // end else    

          if ( glob_codes[token_i].token != "else" ) {
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if  
            else {
              last_correct_i = index.i;
              index.i = token_i;
              index.stats = 1;
              index = Ropisbe( index, last_correct_i ); // rest_of_PPMM_Identifier_started_basic_exp 
            } // end else 
          } // end if 
          else {
            index.i = token_i;
            index.result = false;
          } // end else
        } // end else if 
        else {
          index.i = token_i;
          index.result = false;
        } // end else 
      } // end else if 
      else {
        index.i = save_i;
        index.stats = 2;
        index = Basic_expression( index ); // �~�򩹤U����  
      } // end else 
    } // end else if
    else {
      index.stats = 2;
      index = Basic_expression( index ); // �~�򩹤U����  
    } // end else  
  } // end else if 
  else if ( index.stats == 2 ) {
    if ( Is_sign( glob_codes[token_i].token ) == true ) {
      while ( token_i != -1 && Is_sign( glob_codes[token_i].token ) == true ) {
        index.i = token_i;
        token_i++;
        token_i = FindToken( token_i );
      } // end while 

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if 
      else {
        index.i = token_i;
        index = Signed_unary_exp( index ); // signed_unary_exp
      
        if ( index.result == true ) {
          token_i = index.i + 1;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else {
            last_correct_i = index.i;
            index.i = token_i;
            index.stats = 1;
            index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 
          } // end else 
        } // end if 
      } // end else 
    } // end if 
    else if ( Is_CONST( glob_codes[token_i].type ) == true ) {
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if
      else {
        last_correct_i = index.i;
        index.i = token_i;
        index.stats = 1;
        index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 
      } // end else 
    } // end else if 
    else if ( glob_codes[token_i].token == "(" ) {
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if 
      else {
        index.i = token_i;
        index.stats = 1;
        index = Expression( index, index.i ); // expression 

        if ( index.result == true ) {
          token_i = index.i + 1 ;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if
          else if ( glob_codes[token_i].token == ")" ) {
            index.i = token_i;
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if 
            else {
              last_correct_i = index.i;
              index.i = token_i;
              index.stats = 1;
              index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 
            } // end else 
          } // end else if 
          else {
            index.i = token_i;
            index.result = false;
          } // end else 
        } // end if 
      } // end else 
    } // else if 
    else {
      index.i = token_i;
      index.result = false;
    } // end else
  } // end else if 

  return index;
} // Basic_expression() 

ReturnResult Roisbe( ReturnResult index, int last_i ) { // rest_of_Identifier_started_basic_exp, stats 1 2 
  int save_i, last_correct_i, start;
  int token_i = FindToken( index.i );
  vector<int> farthest;

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if
  else if ( index.stats == 1 ) {
    start = token_i;
    if ( glob_codes[token_i].token == "[" ) { // �i���i�L 
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if 
      else {
        index.i = token_i;
        index.stats = 1;
        index = Expression( index, index.i ); // expression 

        if ( index.result == true ) {
          token_i = index.i + 1 ;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if
          else if ( glob_codes[token_i].token == "]" ) {
            index.i = token_i;
            index.result = true;
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input  
              index.error = 2;
              index.result = true;
            } // end if 
            else {
              save_i = index.i;
              index.i = token_i;
              index = Assignment_operator( index );

              if ( index.result == true ) {
                token_i = index.i + 1;
                token_i = FindToken( token_i );
              
                if ( token_i == -1 ) { // waiting for remaining input 
                  index.error = 2;
                  index.result = true;
                } // end if 
                else { 
                  index.i = token_i;
                  index.stats = 1;
                  index = Basic_expression( index ); // basic_expression 
                } // end else 
              } // end if 
              else {
                farthest.push_back( index.i );
                index.i = save_i; // �h�^']' 
                if ( glob_codes[token_i].token == "+" ) { // �i���i�L PP
                  if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
                    index.i = token_i;
                    index.error = 2;
                    index.result = true; 
                  } // end else if 
                  else if ( glob_codes[token_i + 1].token == "+" ) {
                    index.i = token_i + 1;
                    token_i = token_i + 2;
                    token_i = FindToken( token_i );

                    if ( token_i == -1 ) { // waiting for remaining input 
                      index.error = 2;
                      index.result = true;
                    } // end if 
                    else {
                      last_correct_i = index.i;
                      index.i = token_i;
                      index.stats = 1;
                      index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

                      if ( index.result == false ) { // �ճѤU�i���  
                        farthest.push_back( index.i );
                        index.i = start;
                        last_correct_i = last_i;
                        index.stats = 1;
                        index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

                        if ( index.result == false ) {
                          farthest.push_back( index.i );
                        } // end if 
                      } // end if 
                    } // end else 
                  } // end else if 
                  else { // �ճѤU�i���  
                    farthest.push_back( token_i + 1 );
                    index.i = start;
                    last_correct_i = last_i;
                    index.stats = 1;
                    index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

                    if ( index.result == false ) {
                      farthest.push_back( index.i );
                    } // end if 
                  } // end else 
                } // end if 
                else if ( glob_codes[token_i].token == "-" ) { // �i���i�L MM 
                  if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
                    index.i = token_i;
                    index.error = 2;
                    index.result = true; 
                  } // end else if 
                  else if ( glob_codes[token_i + 1].token == "-" ) {
                    index.i = token_i + 1;
                    token_i = token_i + 2;
                    token_i = FindToken( token_i );

                    if ( token_i == -1 ) { // waiting for remaining input 
                      index.error = 2;
                      index.result = true;
                    } // end if 
                    else {
                      last_correct_i = index.i;
                      index.i = token_i;
                      index.stats = 1;
                      index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe

                      if ( index.result == false ) { // �ճѤU���i��� 
                        farthest.push_back( index.i );
                        index.i = start;
                        last_correct_i = last_i;
                        index.stats = 1;
                        index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

                        if ( index.result == false ) {
                          farthest.push_back( index.i );
                        } // end if  
                      } // end if 
                    } // end else 
                  } // end else if 
                  else { // �ճѤU�i���
                    farthest.push_back( token_i + 1 );
                    index.i = start;
                    last_correct_i = last_i;
                    index.stats = 1;
                    index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

                    if ( index.result == false ) {
                      farthest.push_back( index.i );
                    } // end if 
                  } // end else 
                } // end else if 
                else { // �u��romce_and_romloe
                  last_correct_i = last_i; // ']'  
                  index.i = token_i;
                  index.stats = 1;
                  index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

                  if ( index.result == false ) {
                    farthest.push_back( index.i );
                  } // end if 
                } // end else 
              } // end else 
            } // end else 
          } // end else if 
          else {
            farthest.push_back( token_i );
            index.i = start;
            last_correct_i = last_i;
            index.stats = 1;
            index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

            if ( index.result == false ) {
              farthest.push_back( index.i );
            } // end if 
          } // end else 
        } // end if 
      } // end else 
    } // end if 
    else if ( glob_codes[token_i].token == "(" ) {
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if
      else {
        index.i = token_i;
        index.stats = 1;
        index = Actual_parameter_list( index, index.i );

        if ( index.result == true ) {
          token_i = index.i + 1;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else if ( glob_codes[token_i].token == ")" ) {
            index.i = token_i;
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if
            else {
              last_correct_i = index.i;
              index.i = token_i;
              index.stats = 1;
              index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

              if ( index.result == false ) {
                farthest.push_back( index.i );
              } // end if 
            } // end else 
          } // end else if 
          else {
            farthest.push_back( token_i );
            index.i = start;
            last_correct_i = last_i;
            index.stats = 1;
            index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

            if ( index.result == false ) {
              farthest.push_back( index.i );
            } // end if 
          } // end else 
        } // end if 
        else {
          farthest.push_back( index.i );
          if ( glob_codes[token_i].token == ")" ) { // �u��'()' 
            index.i = token_i;
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if
            else {
              last_correct_i = index.i;
              index.i = token_i;
              index.stats = 1;
              index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

              if ( index.result == false ) {
                farthest.push_back( index.i );
              } // end if 
            } // end else 
          } // end else if 
          else {
            farthest.push_back( token_i );
            index.i = start;
            last_correct_i = last_i;
            index.stats = 1;
            index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

            if ( index.result == false ) {
              farthest.push_back( index.i );
            } // end if 
          } // end else 
        } // end else 
      } // end else   	
    } // end else if 
    else {
      save_i = index.i;
      index.i = token_i;
      index = Assignment_operator( index );

      if ( index.result == true ) {
        token_i = index.i + 1;
        token_i = FindToken( token_i );
              
        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else { 
          index.i = token_i;
          index.stats = 1;
          index = Basic_expression( index ); // basic_expression 

          if ( index.result == false ) {
            farthest.push_back( index.i );
          } // end if 
        } // end else 
      } // end if 
      else {
        farthest.push_back( index.i );
        index.i = save_i; // �h�^  
        if ( glob_codes[token_i].token == "+" ) { // �i���i�L PP
          if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
            index.i = token_i;
            index.error = 2;
            index.result = true; 
          } // end else if 
          else if ( glob_codes[token_i + 1].token == "+" ) {
            index.i = token_i + 1;
            token_i = token_i + 2;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if 
            else {
              last_correct_i = index.i;
              index.i = token_i;
              index.stats = 1;
              index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

              if ( index.result == false ) { // �ճѤU���i��� 
                farthest.push_back( index.i );
                index.i = start;
                last_correct_i = last_i;
                index.stats = 1;
                index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

                if ( index.result == false ) {
                  farthest.push_back( index.i );
                } // end if 
              } // end if 
            } // end else 
          } // end else if 
          else { // �ճѤU���i��� 
            farthest.push_back( token_i + 1 );
            index.i = start;
            last_correct_i = last_i;
            index.stats = 1;
            index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

            if ( index.result == false ) {
              farthest.push_back( index.i );
            } // end if 
          } // end else 
        } // end if 
        else if ( glob_codes[token_i].token == "-" ) { // �i���i�L MM 
          if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
            index.i = token_i;
            index.error = 2;
            index.result = true; 
          } // end else if 
          else if ( glob_codes[token_i + 1].token == "-" ) {
            index.i = token_i + 1;
            token_i = token_i + 2;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if 
            else {
              last_correct_i = index.i;
              index.i = token_i;
              index.stats = 1;
              index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe

              if ( index.result == false ) { // �ճѤU���i��� 
                farthest.push_back( index.i );
                index.i = start;
                last_correct_i = last_i;
                index.stats = 1;
                index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe

                if ( index.result == false ) {
                  farthest.push_back( index.i );
                } // end if 
              } // end if 
            } // end else 
          } // end else if 
          else { // �ճѤU���i��� 
            farthest.push_back( token_i + 1 );
            index.i = start;
            last_correct_i = last_i;
            index.stats = 1;
            index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

            if ( index.result == false ) {
              farthest.push_back( index.i );
            } // end if 
          } // end else 
        } // end else if 
        else { // �ճѤU���i��� 
          farthest.push_back( token_i );
          index.i = start;
          last_correct_i = last_i;
          index.stats = 1;
          index = Romce_and_romloe( index, last_correct_i ); // romce_and_romloe 

          if ( index.result == false ) {
            farthest.push_back( index.i );
          } // end if 
        } // end else 
      } // end else  
    } // end else 

    if ( index.result == false ) {
      index.i = FindFarthest( farthest, index.i );
      index.result = false;
    } // end if
  } // end else if 

  farthest.clear();
  return index;
} // Roisbe()  

ReturnResult Ropisbe( ReturnResult index, int last_i ) { // rest_of_PPMM_Identifier_started_basic_exp 
  int last_correct_i;
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if
  else if ( glob_codes[token_i].token == "[" ) { // �i���i�L 
    index.i = token_i;
    token_i++;
    token_i = FindToken( token_i );

    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = Expression( index, index.i ); // expression 

      if ( index.result == true ) {
        token_i = index.i + 1 ;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input  
          index.error = 2;
          index.result = true;
        } // end if
        else if ( glob_codes[token_i].token == "]" ) {
          index.i = token_i;
          index.result = true;

          token_i++;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input  
            index.error = 2;
            index.result = true;
          } // end if 
          else { 
            last_correct_i = index.i;
            index.i = token_i;
            index.stats = 1;
            index = Romce_and_romloe( index, last_correct_i ); // Romce_and_romloe 
          } // end else 
        } // end else if 
        else {
          index.i = token_i;
          index.result = false;
        } // end else
      } // end if 
    } // end else 
  } // end else if 
  else {
    index.i = token_i;
    index.stats = 1;
    index = Romce_and_romloe( index, last_i ); // Romce_and_romloe 
  } // end else 

  return index;
} // Ropisbe()  

bool Is_sign( string str ) {
  bool result = false;

  if ( str == "+" || str == "-" || str == "!" ) {
    result = true;
  } // end if 

  return result;
} // Is_sign() 

ReturnResult Actual_parameter_list( ReturnResult index, int last_i ) {
// actual_parameter_list, stats 1 2, last_i 
//  �@�}�llast_i�ǤJԣ�������n, ��var���j�Ǹ�ƥ�  
  int token_i = FindToken( index.i );
  int save_i, last_correct_i;
 
  if ( index.stats == 1 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if
    else {
      index.i = token_i;
      index.stats = 1;
      index = Basic_expression( index );

      if ( index.result == true ) {
        last_correct_i = index.i;
        index.i++;
        index.stats = 2;
        index = Actual_parameter_list( index, last_correct_i ); // �A�դ@��
      } // end if 
    } // end else 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // �S����h�F  
      index.result = true;
    } // end if 
    else if ( glob_codes[token_i].token == "," ) {
      index.i = token_i + 1;
      index.stats = 1;
      index = Basic_expression( index );   

      if ( index.result == false ) {
        // index.i = last_i;
        // index.result = true;
      } // end if
      else {
        last_correct_i = index.i;
        index.i++;
        index.stats = 2;
        index = Actual_parameter_list( index, last_correct_i ); // �A�դ@��
      } // end else 
    } // end else if
    else { // 0��  
      index.i = last_i;
      index.result = true;
    } // end else 
  } // end else if 

  return index;
} // Actual_parameter_list() 

ReturnResult Assignment_operator( ReturnResult index ) {
  int token_i = FindToken( index.i );
  
  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( glob_codes[token_i].token == "+" ) {
    index.i = token_i;
    if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else if ( glob_codes[token_i + 1].token == "=" ) {
      index.i = token_i + 1;
      index.result = true;
    } // end else if 
    else {
      index.i = token_i + 1;
      index.result = false;
    } // end else 
  } // end else if 
  else if ( glob_codes[token_i].token == "-" ) {
    index.i = token_i;
    if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else if ( glob_codes[token_i + 1].token == "=" ) {
      index.i = token_i + 1;
      index.result = true;
    } // end else if 
    else {
      index.i = token_i + 1;
      index.result = false;
    } // end else 
  } // end else if 
  else if ( glob_codes[token_i].token == "*" ) {
    index.i = token_i;
    if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else if ( glob_codes[token_i + 1].token == "=" ) {
      index.i = token_i + 1;
      index.result = true;
    } // end else if 
    else {
      index.i = token_i + 1;
      index.result = false;
    } // end else 
  } // end else if 
  else if ( glob_codes[token_i].token == "/" ) {
    index.i = token_i;
    if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else if ( glob_codes[token_i + 1].token == "=" ) {
      index.i = token_i + 1;
      index.result = true;
    } // end else if 
    else {
      index.i = token_i + 1;
      index.result = false;
    } // end else 
  } // end else if 
  else if ( glob_codes[token_i].token == "%" ) {
    index.i = token_i;
    if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else if ( glob_codes[token_i + 1].token == "=" ) {
      index.i = token_i + 1;
      index.result = true;
    } // end else if 
    else {
      index.i = token_i + 1;
      index.result = false;
    } // end else 
  } // end else if 
  else if ( glob_codes[token_i].token == "=" ) {
    index.i = token_i;
    index.result = true;
  } // end else if 
  else {
    index.i = token_i;
    index.result = false;
  } // end else 

  return index;
} // Assignment_operator() 

// ------start of Unary_exp------ 

ReturnResult Unary_exp( ReturnResult index ) { // ����++ --�Osign �٬O PP/MM 
  int token_i = FindToken( index.i );
  vector<int> farthest;

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if
  else {
    index.i = token_i;
    index = Unsigned_unary_exp( index ); // 1.unsigned_unary

    if ( index.result == false ) { // 2.PP/MM, 3.sign { sign } signed_unary_exp 
      farthest.push_back( index.i );
      index.i = token_i;

      if ( glob_codes[token_i].token == "+" ) { // PP 
        if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
          index.i = token_i;
          index.error = 2;
          index.result = true; 
        } // end else if 
        else if ( glob_codes[token_i + 1].token == "+" ) {
          index.i = token_i + 1;
          token_i = token_i + 2;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func )
            index.i = token_i;

            if ( Find_compnd_IDENT( glob_codes[token_i].token ) == true ) {
              index.result = true; // ���򻼰j  
              string type = glob_codes[token_i].type;
              if ( index.undefined == -1 && Is_CONST( type ) == false ) { // �����Ĥ@��undefined  
                index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
              } // end if 
              // tmp.error = 4; // undefined
            } // end if
            else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // �|���Q�w�q  
              index.result = true; // ���򻼰j  
              string type = glob_codes[token_i].type;
              if ( index.undefined == -1 && Is_CONST( type ) == false ) { // �����Ĥ@��undefined  
                index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
              } // end if 
              // tmp.error = 4; // undefined
            } // end else if 
            else {
              index.result = true;

              if ( glob_codes[token_i].type == "unknown" ) {
                index.result = true;
                glob_codes[token_i].type = "IDENT"; // ��{int x;}��statement executed 
              } // end if 
            } // end else    

            int save_i = index.i; 
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // �S����h�F  
              index.result = true;
            } // end if 
            else if ( glob_codes[token_i].token == "[" ) { // �i���i�L 
              token_i++;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // waiting for remaining input 
                index.error = 2;
                index.result = true;
              } // end if 
              else {
                index.i = token_i;
                index.stats = 1;
                index = Expression( index, index.i ); // expression 
                if ( index.result == true ) {
                  token_i = index.i + 1 ;
                  token_i = FindToken( token_i );

                  if ( token_i == -1 ) { // waiting for remaining input  
                    index.error = 2;
                    index.result = true;
                  } // end if
                  else if ( glob_codes[token_i].token == "]" ) {
                    index.i = token_i;
                    index.result = true;
                  } // end else if 
                  else { // �h�^( PP | MM )Identifier  
                    index.i = save_i;
                    index.result = true;
                  } // end else
                } // end if 
                else { // �h�^( PP | MM )Identifier 
                  index.i = save_i; 
                  index.result = true;
                } // end else
              } // end else 
            } // end else if 
            else { // �u��IDENT  
              index.result = true;
            } // end else 
          } // end else if 
          else {
            farthest.push_back( token_i );
            index.i = token_i;
            index.result = false;
          } // end else 
        } // end else if 
        else {
          farthest.push_back( token_i + 1 );
          index.i = token_i + 1;
          index.result = false;
        } // end else 
      } // end else if
      else if ( glob_codes[token_i].token == "-" ) { // MM 
        if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
          index.i = token_i;
          index.error = 2;
          index.result = true; 
        } // end else if 
        else if ( glob_codes[token_i + 1].token == "-" ) {
          index.i = token_i + 1;
          token_i = token_i + 2;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func )
            index.i = token_i;

            if ( Find_compnd_IDENT( glob_codes[token_i].token ) == true ) {
              index.result = true; // ���򻼰j  
            } // end if
            else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // �|���Q�w�q  
              index.result = true; // ���򻼰j  
              string type;
              type = glob_codes[token_i].type;
              if ( index.undefined == -1 && Is_CONST( type ) == false ) { // �����Ĥ@��undefined  
                index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
              } // end if 
              // tmp.error = 4; // undefined
            } // end else if 
            else {
              if ( glob_codes[token_i].type == "unknown" ) {
                index.result = true;
                glob_codes[token_i].type = "IDENT"; // ��{int x;}��statement executed 
              } // end if  
            } // end else    

            int save_i = index.i; 
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // �S����h�F  
              index.result = true;
            } // end if 
            else if ( glob_codes[token_i].token == "[" ) { // �i���i�L 
              token_i++;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // waiting for remaining input 
                index.error = 2;
                index.result = true;
              } // end if 
              else {
                index.i = token_i;
                index.stats = 1;
                index = Expression( index, index.i ); // expression 
                if ( index.result == true ) {
                  token_i = index.i + 1 ;
                  token_i = FindToken( token_i );

                  if ( token_i == -1 ) { // waiting for remaining input  
                    index.error = 2;
                    index.result = true;
                  } // end if
                  else if ( glob_codes[token_i].token == "]" ) {
                    index.i = token_i;
                    index.result = true;
                  } // end else if 
                  else { // �h�^( PP | MM )Identifier  
                    index.i = save_i;
                    index.result = true;
                  } // end else 
                } // end if 
                else { // �h�^( PP | MM )Identifier 
                  index.i = save_i; 
                  index.result = true;
                } // end else
              } // end else 
            } // end else if 
            else { // �u��IDENT  
              index.result = true;
            } // end else 
          } // end else if 
          else {
            farthest.push_back( token_i );
            index.i = token_i;
            index.result = false;
          } // end else 
        } // end else if 
        else {
          farthest.push_back( token_i + 1 );
          index.i = token_i + 1;
          index.result = false;
        } // end else 
      } // end else if
      else { // 3.sign { sign } signed_unary_exp 
        string tmp_token = glob_codes[token_i].token;
        if ( tmp_token == "+" || tmp_token == "-" || tmp_token == "!" ) {
          index.i = token_i;
          token_i++;
          token_i = FindToken( token_i );

          if ( token_i != -1 ) {
            tmp_token = glob_codes[token_i].token;
          } // end if 

          while ( token_i != -1 && ( tmp_token == "+" || tmp_token == "-" || tmp_token == "!" ) ) {
          // 0��h��sign  
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i != -1 ) {
              tmp_token = glob_codes[token_i].token;
            } // end if 
          } // end while

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else {
            index.i = token_i;
            index = Signed_unary_exp( index ); // signed_unary_exp 
          } // end else  	
        } // end if 
        else {
          farthest.push_back( token_i );
          index.result = false;
        } // end else 
      } // end else     
    } // end if

    if ( index.result == false ) {
      index.i = FindFarthest( farthest, index.i );
      index.result = false;
    } // end if
  } // end else 

  farthest.clear();
  return index;
} // Unary_exp()

ReturnResult Signed_unary_exp( ReturnResult index ) {
// abc[2][i] -> array
// Func(a+b) -> function call
// ( a+=b ) -> �A���̧�hexpression 

  int token_i = FindToken( index.i );
  
  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func )
    index.i = token_i;
    int save_i = index.i;

    if ( Find_compnd_IDENT( glob_codes[token_i].token ) == true ) {
      index.result = true; // ���򻼰j  
    } // end if
    else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // �|���Q�w�q  
      index.result = true; // ���򻼰j  
      if ( index.undefined == -1 && Is_CONST( glob_codes[token_i].type ) == false ) { // �����Ĥ@��undefined  
        index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
      } // end if 
      // tmp.error = 4; // undefined
    } // end else if 
    else {
      if ( glob_codes[token_i].type == "unknown" ) {
        index.result = true;
        glob_codes[token_i].type = "IDENT"; // ��{int x;}��statement executed 
      } // end if 
    } // end else    
    
    token_i++;
    token_i = FindToken( token_i );

    if ( token_i == -1 ) { // �S����h�F 
      index.result = true;
    } // end if
    else if ( glob_codes[token_i].token == "(" ) { // �i���i�L  
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input  
        index.error = 2;
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == ")" ) { // Foo(), function call  
        index.i = token_i;
        index.result = true;
      } // end else if 
      else {
        index.i = token_i;
        index.stats = 1;
        index = Actual_parameter_list( index, index.i ); // Actual_parameter_list
        if ( index.result == true ) {
          token_i = index.i + 1 ;
          token_i = FindToken( token_i );
          
          if ( token_i == -1 ) { // waiting for remaining input  
            index.error = 2;
            index.result = true;
          } // end if
          else if ( glob_codes[token_i].token == ")" ) {
            index.i = token_i;
            index.result = true;
          } // end else if 
          else { // �h�^Identifier  
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if
        else { // �h�^Identifier
          index.i = save_i;
          index.result = true;
        } // end else 
      } // end else 
    } // end else if 
    else if ( glob_codes[token_i].token == "[" ) { // �i���i�L  array[i][j] 
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input  
        index.error = 2;
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "]" ) {
        index.i = token_i;
        index.result = true;
      } // end else if 
      else {
        index.i = token_i;
        index.stats = 1;
        index = Expression( index, index.i ); // expression 
        if ( index.result == true ) {
          token_i = index.i + 1 ;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input  
            index.error = 2;
            index.result = true;
          } // end if
          else if ( glob_codes[token_i].token == "]" ) {
            index.i = token_i;
            index.result = true;
          } // end else if 
          else { // �h�^Identifier
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if 
        else { // �h�^Identifier 
          index.i = save_i;
          index.result = true;
        } // end else
      } // end else 
    } // end else if
    else { // �u��IDENT 
      index.result = true;
    } // end else 
  } // end else if 
  else if ( Is_CONST( glob_codes[token_i].type ) == true ) { // is CONST 
    index.i = token_i;
    index.result = true;
  } // end else if 
  else if ( glob_codes[token_i].token == "(" ) { // '(' + expression + ')' 
    index.i = token_i;
    token_i++;
    token_i = FindToken( token_i );

    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = Expression( index, index.i ); // expression
      if ( index.result == true ) {
        token_i = index.i + 1 ;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input  
          index.error = 2;
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == ")" ) {
          index.i = token_i;
          index.result = true;
        } // end else if 
        else {
          index.i = token_i;
          index.result = false;
        } // end else 
      } // end if 
      else {
        index.result = false; 
      } // end else 
    } // end else 
  } // end else if 
  else {
    index.i = token_i;
    index.result = false;
  } // end else 

  return index;
} // Signed_unary_exp() 

ReturnResult Unsigned_unary_exp( ReturnResult index ) {
// abc[2][i] -> array
// Func(a+b) -> function call
// ( a+=b ) -> �A���̧�hexpression 

  int token_i = FindToken( index.i );
  
  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func )
    index.i = token_i;
    int save_i = index.i;

    if ( Find_compnd_IDENT( glob_codes[token_i].token ) == true ) {
      index.result = true; // ���򻼰j  
    } // end if
    else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // �|���Q�w�q  
      index.result = true; // ���򻼰j  
      if ( index.undefined == -1 && Is_CONST( glob_codes[token_i].type ) == false ) { // �����Ĥ@��undefined  
        index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
      } // end if 
      // tmp.error = 4; // undefined
    } // end else if 
    else {
      if ( glob_codes[token_i].type == "unknown" ) {
        index.result = true;
        glob_codes[token_i].type = "IDENT"; // ��{int x;}��statement executed 
      } // end if 
    } // end else    
    
    token_i++;
    token_i = FindToken( token_i );

    if ( token_i == -1 ) { // �S����h�F  
      index.result = true;
    } // end if
    else if ( glob_codes[token_i].token == "(" ) { // �i���i�L  
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input  
        index.error = 2;
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == ")" ) { // Foo(), function call  
        index.i = token_i;
        index.result = true;
      } // end else if 
      else {
        index.i = token_i;
        index.stats = 1;
        index = Actual_parameter_list( index, index.i ); // Actual_parameter_list
        if ( index.result == true ) {
          token_i = index.i + 1 ;
          token_i = FindToken( token_i );
          
          if ( token_i == -1 ) { // waiting for remaining input  
            index.error = 2;
            index.result = true;
          } // end if
          else if (  glob_codes[token_i].token == ")" ) {
            index.i = token_i;
            index.result = true;
          } // end else if 
          else { // �h�^Identifier
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if
        else { // �h�^Identifier 
          index.i = save_i;
          index.result = true;
        } // end else 
      } // end else 
    } // end else if 
    else if ( glob_codes[token_i].token == "[" ) { // �i���i�L  array[i][j] 
      index.i = token_i;
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input  
        index.error = 2;
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "]" ) {
        index.i = token_i;
        index.result = true;
      } // end else if 
      else {
        index.i = token_i;
        index.stats = 1;
        index = Expression( index, index.i ); // expression 
        if ( index.result == true ) {
          token_i = index.i + 1 ;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input  
            index.error = 2;
            index.result = true;
          } // end if
          else if ( glob_codes[token_i].token == "]" ) {
            index.i = token_i;
            token_i = index.i + 1 ;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input  
              index.error = 2;
              index.result = true;
            } // end if
            else if ( glob_codes[token_i].token == "+" ) { // �i���i�L PP 
              if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
                index.i = token_i;
                index.error = 2;
              } // end else if 
              else if ( glob_codes[token_i + 1].token == "+" ) {
                index.i = token_i + 1;
              } // end else if
              else { // �h�^Identifier'[' expression ']'  

              } // end else 

              index.result = true;
            } // end else if
            else if ( glob_codes[token_i].token == "-" ) { // �i���i�L MM 
              if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
                index.i = token_i;
                index.error = 2;
              } // end else if 
              else if ( glob_codes[token_i + 1].token == "-" ) {
                index.i = token_i + 1;
              } // end else if 
              else { // �h�^Identifier'[' expression ']'  

              } // end else 

              index.result = true;
            } // end else if
            else { // �u��Identifier'[' expression ']'  
              index.result = true;
            } // end else 
          } // end else if 
          else { // �h�^Identifier 
            index.i = save_i;
            index.result = true;
          } // end else
        } // end if 
        else { // �h�^Identifier  
          index.i = save_i;
          index.result = true;
        } // end else
      } // end else 
    } // end else if
    else if ( glob_codes[token_i].token == "+" ) { // �i���i�L PP 
      if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
        index.i = token_i;
        index.error = 2;
      } // end else if 
      else if ( glob_codes[token_i + 1].token == "+" ) {
        index.i = token_i + 1;
      } // end else if 
      else { // �h�^Identifier  

      } // end else 

      index.result = true;
    } // end else if
    else if ( glob_codes[token_i].token == "-" ) { // �i���i�L PP 
      if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
        index.i = token_i;
        index.error = 2;
      } // end else if 
      else if ( glob_codes[token_i + 1].token == "-" ) {
        index.i = token_i + 1;
      } // end else if 
      else { // �h�^Identifier  

      } // end else 

      index.result = true;
    } // end else if
    else { // �u��IDENT 
      index.result = true;
    } // end else 
  } // end else if 
  else if ( Is_CONST( glob_codes[token_i].type ) == true ) { // is CONST 
    index.i = token_i;
    index.result = true;
  } // end else if 
  else if ( glob_codes[token_i].token == "(" ) { // '(' + expression + ')' 
    index.i = token_i;
    token_i++;
    token_i = FindToken( token_i );

    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = Expression( index, index.i ); // expression
      if ( index.result == true ) {
        token_i = index.i + 1 ;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input  
          index.error = 2;
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == ")" ) {
          index.i = token_i;
          index.result = true;
        } // end else if 
        else {
          index.i = token_i;
          index.result = false;
        } // end else 
      } // end if 
      else {
        index.result = false; 
      } // end else 
    } // end else 
  } // end else if 
  else {
    index.i = token_i;
    index.result = false;
  } // end else 

  return index;
} // Unsigned_unary_exp()  

// ------end of Unary_exp------ 
// ------start of romce_and_romloe------

ReturnResult Romce_and_romloe( ReturnResult index, int last_i ) { // Romce_and_romloe, stats 1 
  int last_correct_i, save_i;
  int token_i = FindToken( index.i );
  
  //  if ( token_i == -1 ) { // waiting for remaining input 
  //    index.error = 2;
  //    index.result = true;
  //  } // end if 
  if ( index.stats == 1 ) {
    last_correct_i = last_i;
    index.i = token_i;
    index.stats = 1;
    index = Romloe( index, last_correct_i ); // romloe 
    save_i = index.i; 

    if ( index.result == true ) {
      last_correct_i = index.i;
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h�F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "?" ) { // �i���i�L 
        index.i = token_i;
        token_i++;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if 
        else {
          index.i = token_i;
          index.stats = 1;
          index = Basic_expression( index );

          if ( index.result == true ) {
            token_i = index.i + 1;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if 
            else if ( glob_codes[token_i].token == ":" ) {
              index.i = token_i;
              token_i++;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // waiting for remaining input 
                index.error = 2;
                index.result = true;
              } // end if 
              else {
                index.i = token_i;
                index.stats = 1;
                index = Basic_expression( index );

                if ( index.result == false ) { // �h�^romloe 
                  index.i = save_i;
                  index.result = true;
                } // end if 
              } // end else 
            } // end else if 
            else { // �h�^romloe  
              index.i = save_i;
              index.result = true;
            } // end else 
          } // end if 
          else { // �h�^romloe  
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end else 
      } // end else if 
    } // end if 
  } // end if 

  return index;
} // Romce_and_romloe() 

ReturnResult Romloe( ReturnResult index, int last_i ) { // romloe, stats 1 2 
  int last_correct_i, save_i, save_i2;
  int token_i = FindToken( index.i );
  
  //  if ( token_i == -1 ) { // waiting for remaining input 
  //    index.error = 2;
  //    index.result = true;
  //  } // end if 
  if ( index.stats == 1 ) {
    last_correct_i = last_i;
    index.i = token_i;
    index.stats = 1;
    index = Romlae( index, last_correct_i ); // romlae 
    save_i = index.i; 

    if ( index.result == true ) {
      last_correct_i = index.i;
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h���F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "|" ) {
        index.i = token_i;

        if ( token_i + 1 < glob_codes.size() ) { // ����'||'  
          if ( glob_codes[token_i + 1].token == "|" ) {
            index.i = token_i + 1;
            index.result = true;

            token_i = index.i + 1;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if
            else {
              index.i = token_i;
              index.stats = 2;
              index = Romloe( index, last_correct_i ); // �A���դ@�� 
            } // end else 
          } // end if 
          else { // �h�^romlae 
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if 
        else { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end else 
      } // end else if
    } // end if 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = M_lae( index ); // maybe_logical_AND_exp 
      save_i2 = index.i;

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // �S����h���F 
          index.result = true;
        } // end if
        else if ( glob_codes[token_i].token == "|" ) {
          index.i = token_i;

          if ( token_i + 1 < glob_codes.size() ) { // ����'||'  
            if ( glob_codes[token_i + 1].token == "|" ) {
              index.i = token_i + 1;
              index.result = true;

              token_i = index.i + 1;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // waiting for remaining input 
                index.error = 2;
                index.result = true;
              } // end if
              else {
                index.i = token_i;
                index.stats = 2;
                index = Romloe( index, last_correct_i ); // �A���դ@�� 
              } // end else 
            } // end if 
            else { // �h�^maybe_logical_AND_exp 
              index.i = save_i2;
              index.result = true;
            } // end else 
          } // end if 
          else { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end else 
        } // end else if
      } // end if 
      else { // �h�^  
        index.i = last_i;
        index.result = true;
      } // end else
    } // end else  
  } // end else if 

  return index;
} // Romloe() 

ReturnResult Romlae( ReturnResult index, int last_i ) { // romlae, stats 1 2
  int last_correct_i, save_i, save_i2;
  int token_i = FindToken( index.i );
  
  //  if ( token_i == -1 ) { // waiting for remaining input 
  //    index.error = 2;
  //    index.result = true;
  //  } // end if 
  if ( index.stats == 1 ) {
    last_correct_i = last_i;
    index.i = token_i;
    index.stats = 1;
    index = Romboe( index, last_correct_i ); // romboe 
    save_i = index.i;

    if ( index.result == true ) {
      last_correct_i = index.i;
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h���F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "&" ) {
        index.i = token_i;

        if ( token_i + 1 < glob_codes.size() ) { // ����'&&'  
          if ( glob_codes[token_i + 1].token == "&" ) {
            index.i = token_i + 1;
            index.result = true;

            token_i = index.i + 1;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if
            else {
              index.i = token_i;
              index.stats = 2;
              index = Romlae( index, last_correct_i ); // �A���դ@�� 
            } // end else 
          } // end if 
          else { // �h�^romboe 
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if 
        else { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end else 
      } // end else if 
    } // end if 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = M_boe( index ); // maybe_bit_OR_exp 
      save_i2 = index.i;

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // �S����h���F 
          index.result = true;
        } // end if
        else if ( glob_codes[token_i].token == "&" ) {
          index.i = token_i;

          if ( token_i + 1 < glob_codes.size() ) { // ����'&&'  
            if ( glob_codes[token_i + 1].token == "&" ) {
              index.i = token_i + 1;
              index.result = true;

              token_i = index.i + 1;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // waiting for remaining input 
                index.error = 2;
                index.result = true;
              } // end if
              else {
                index.i = token_i;
                index.stats = 2;
                index = Romlae( index, last_correct_i ); // �A���դ@�� 
              } // end else 
            } // end if 
            else { // �h�^maybe_bit_OR_exp 
              index.i = save_i2;
              index.result = true;
            } // end else 
          } // end if 
          else { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end else 
        } // end else if
      } // end if 
      else { // �h�^  
        index.i = last_i;
        index.result = true; 
      } // end else 
    } // end else 
  } // end else if 

  return index;
} // Romlae() 

ReturnResult Romboe( ReturnResult index, int last_i ) { // romboe, stats 1 2 
  int last_correct_i; 
  int token_i = FindToken( index.i );
  
  //  if ( token_i == -1 ) { // waiting for remaining input 
  //    index.error = 2;
  //    index.result = true;
  //  } // end if 
  if ( index.stats == 1 ) {
    last_correct_i = last_i;
    index.i = token_i;
    index.stats = 1;
    index = Rombeoe( index, last_correct_i ); // rombeoe 

    if ( index.result == true ) {
      last_correct_i = index.i;
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h���F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "|" ) {
        index.i = token_i;
        index.result = true;
        
        token_i++;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if
        else {
          index.i = token_i;
          index.stats = 2;
          index = Romboe( index, last_correct_i ); // �A���դ@�� 
        } // end else
      }  // end else if 
    } // end if 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = M_beoe( index ); // maybe_bit_ex_OR_exp 

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // �S����h���F 
          index.result = true;
        } // end if
        else if ( glob_codes[token_i].token == "|" ) {
          index.i = token_i;
          index.result = true;
        
          token_i++;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if
          else {
            index.i = token_i;
            index.stats = 2;
            index = Romboe( index, last_correct_i ); // �A���դ@�� 
          } // end else
        } // end else if 
      } // end if 
      else { // �h�^  
        index.i = last_i;
        index.result = true; 
      } // end else 
    } // end else 
  } // end else if 

  return index;
} // Romboe() 

ReturnResult Rombeoe( ReturnResult index, int last_i ) { // rombeoe, stats 1 2 
  int last_correct_i;
  int token_i = FindToken( index.i );
  
  //  if ( token_i == -1 ) { // waiting for remaining input 
  //    index.error = 2;
  //    index.result = true;
  //  } // end if 
  if ( index.stats == 1 ) {
    last_correct_i = last_i;
    index.i = token_i;
    index.stats = 1;
    index = Rombae( index, last_correct_i ); // rombae 

    if ( index.result == true ) {
      last_correct_i = index.i;
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h���F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "^" ) {
        index.i = token_i;
        index.result = true;
        
        token_i++;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if
        else {
          index.i = token_i;
          index.stats = 2;
          index = Rombeoe( index, last_correct_i ); // �A���դ@�� 
        } // end else
      }  // end else if 
    } // end if 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = M_bae( index ); // maybe_bit_AND_exp 

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // �S����h���F 
          index.result = true;
        } // end if
        else if ( glob_codes[token_i].token == "^" ) {
          index.i = token_i;
          index.result = true;
        
          token_i++;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if
          else {
            index.i = token_i;
            index.stats = 2;
            index = Rombeoe( index, last_correct_i ); // �A���դ@�� 
          } // end else
        } // end else if 
      } // end if 
      else { // �h�^  
        index.i = last_i;
        index.result = true; 
      } // end else 
    } // end else 
  } // end else if 

  return index;
} // Rombeoe() 

ReturnResult Rombae( ReturnResult index, int last_i ) { // rombae, stats 1 2 
  int last_correct_i;
  int token_i = FindToken( index.i );
  
  //  if ( token_i == -1 ) { // waiting for remaining input 
  //    index.error = 2;
  //    index.result = true;
  //  } // end if 
  if ( index.stats == 1 ) {
    last_correct_i = last_i;
    index.i = token_i;
    index.stats = 1;
    index = Romee( index, last_correct_i ); // romee 

    if ( index.result == true ) {
      last_correct_i = index.i;
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h���F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "&" ) {
        index.i = token_i;
        index.result = true;
        
        token_i++;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if
        else {
          index.i = token_i;
          index.stats = 2;
          index = Rombae( index, last_correct_i ); // �A���դ@�� 
        } // end else
      }  // end else if 
    } // end if 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = M_ee( index ); // maybe_equality_exp 

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // �S����h���F 
          index.result = true;
        } // end if
        else if ( glob_codes[token_i].token == "&" ) {
          index.i = token_i;
          index.result = true;
        
          token_i++;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if
          else {
            index.i = token_i;
            index.stats = 2;
            index = Rombae( index, last_correct_i ); // �A���դ@�� 
          } // end else
        } // end else if 
      } // end if 
      else { // �h�^  
        index.i = last_i;
        index.result = true; 
      } // end else 
    } // end else 
  } // end else if 

  return index;
} // Rombae() 

ReturnResult Romee( ReturnResult index, int last_i ) { // romee, stats 1 2 
  int last_correct_i, save_i, save_i2;
  int token_i = FindToken( index.i );
  
  //  if ( token_i == -1 ) { // waiting for remaining input 
  //    index.error = 2;
  //    index.result = true;
  //  } // end if 
  if ( index.stats == 1 ) {
    last_correct_i = last_i;
    index.i = token_i;
    index.stats = 1;
    index = Romre( index, last_correct_i ); // rest_of_maybe_relational_exp 
    save_i = index.i; // ����romre��m  

    if ( index.result == true ) {
      last_correct_i = index.i;
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h���F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "!" ) {
        index.i = token_i;
        if ( token_i + 1 < glob_codes.size() ) { // ����'!='  
          if ( glob_codes[token_i + 1].token == "=" ) {
            index.i = token_i + 1;
            index.result = true;

            token_i = index.i + 1;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if
            else {
              index.i = token_i;
              index.stats = 2;
              index = Romee( index, last_correct_i ); // �A���դ@�� 
            } // end else 
          } // end if 
          else { // �h�^rest_of_maybe_relational_exp 
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if 
        else { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end else 
      } // end else if 
      else if ( glob_codes[token_i].token == "=" ) {
        index.i = token_i;
        if ( token_i + 1 < glob_codes.size() ) { // ����'=='  
          if ( glob_codes[token_i + 1].token == "=" ) {
            index.i = token_i + 1;
            index.result = true;

            token_i = index.i + 1;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if
            else {
              index.i = token_i;
              index.stats = 2;
              index = Romee( index, last_correct_i ); // �A���դ@�� 
            } // end else 
          } // end if 
          else { // �h�^rest_of_maybe_relational_exp 
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if 
        else { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end else 
      } // end else if 
      else { // �u��rest_of_maybe_relational_exp 
        index.result = true; 
      } // end else 
    } // end if 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = M_re( index ); // maybe_relational_exp 
      save_i2 = index.i; // �x�s��m  

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        token_i = FindToken( token_i );
      
        if ( token_i == -1 ) { // �S����h���F 
          index.result = true;
        } // end if
        else if ( glob_codes[token_i].token == "!" ) {
          index.i = token_i;
          if ( token_i + 1 < glob_codes.size() ) { // ����'!='  
            if ( glob_codes[token_i + 1].token == "=" ) {
              index.i = token_i + 1;
              index.result = true;

              token_i = index.i + 1;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // waiting for remaining input 
                index.error = 2;
                index.result = true;
              } // end if
              else {
                index.i = token_i;
                index.stats = 2;
                index = Romee( index, last_correct_i ); // �A���դ@�� 
              } // end else 
            } // end if 
            else { // �h�^maybe_relational_exp 
              index.i = save_i2;
              index.result = true;
            } // end else 
          } // end if 
          else { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end else 
        } // end else if 
        else if ( glob_codes[token_i].token == "=" ) {
          index.i = token_i;
          if ( token_i + 1 < glob_codes.size() ) { // ����'=='  
            if ( glob_codes[token_i + 1].token == "=" ) {
              index.i = token_i + 1;
              index.result = true;

              token_i = index.i + 1;
              token_i = FindToken( token_i );

              if ( token_i == -1 ) { // waiting for remaining input 
                index.error = 2;
                index.result = true;
              } // end if
              else {
                index.i = token_i;
                index.stats = 2;
                index = Romee( index, last_correct_i ); // �A���դ@�� 
              } // end else 
            } // end if 
            else { // �h�^maybe_relational_exp 
              index.i = save_i2;
              index.result = true;
            } // end else 
          } // end if 
          else { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end else 
        } // end else if 
        else { // �u��maybe_relational_exp  
          index.result = true; 
        } // end else 
      } // end if 
      else { // �h�^  
        index.i = last_i;
        index.result = true; 
      } // end else 
    } // end else 
  } // end else if

  return index;
} // Romee() 

ReturnResult Romre( ReturnResult index, int last_i ) { // rest_of_maybe_relational_exp, stats 1 2 
  int last_correct_i;
  int token_i = FindToken( index.i );
  
  //  if ( token_i == -1 ) { // waiting for remaining input 
  //    index.error = 2;
  //    index.result = true;
  //  } // end if
  if ( index.stats == 1 ) {
    last_correct_i = last_i;
    index.i = token_i;
    index.stats = 1;
    index = Romse( index, last_correct_i ); // rest_of_maybe_shift_exp 

    if ( index.result == true ) {
      last_correct_i = index.i;
      token_i = index.i + 1;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // �S����h���F 
        index.result = true;
      } // end if 
      else {
        if ( glob_codes[token_i].token == "<" ) {
          index.i = token_i;
          if ( token_i + 1 < glob_codes.size() ) { // ����'<=' 
            if ( glob_codes[token_i + 1].token == "=" ) {
              index.i = token_i + 1;
              index.result = true;
            } // end if 
          } // end if 

          token_i = index.i + 1;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if
          else {
            index.i = token_i;
            index.stats = 2;
            index = Romre( index, last_correct_i ); // �A���դ@�� 
          } // end else 
        } // end if
        else if ( glob_codes[token_i].token == ">" ) {
          index.i = token_i;
          if ( token_i + 1 < glob_codes.size() ) { // ����'>=' 
            if ( glob_codes[token_i + 1].token == "=" ) {
              index.i = token_i + 1;
              index.result = true;
            } // end if 
          } // end if 

          token_i = index.i + 1;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if
          else {
            index.i = token_i;
            index.stats = 2;
            index = Romre( index, last_correct_i ); // �A���դ@�� 
          } // end else 
        } // end else if 
        else { // �u��rest_of_maybe_shift_exp 
          index.result = true;
        } // end else 
      } // end else 
    } // end if 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = M_se( index ); // maybe_shift_exp 

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // �S����h���F 
          index.result = true;
        } // end if 
        else {
          if ( glob_codes[token_i].token == "<" ) {
            index.i = token_i;
            if ( token_i + 1 < glob_codes.size() ) { // ����'<=' 
              if ( glob_codes[token_i + 1].token == "=" ) {
                index.i = token_i + 1;
                index.result = true;
              } // end if 
            } // end if 

            token_i = index.i + 1;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if
            else {
              index.i = token_i;
              index.stats = 2;
              index = Romre( index, last_correct_i ); // �A���դ@�� 
            } // end else 
          } // end if
          else if ( glob_codes[token_i].token == ">" ) {
            index.i = token_i;
            if ( token_i + 1 < glob_codes.size() ) { // ����'>=' 
              if ( glob_codes[token_i + 1].token == "=" ) {
                index.i = token_i + 1;
                index.result = true;
              } // end if 
            } // end if 

            token_i = index.i + 1;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if
            else {
              index.i = token_i;
              index.stats = 2;
              index = Romre( index, last_correct_i ); // �A���դ@�� 
            } // end else 
          } // end else if 
          else { // �u��rest_of_maybe_shift_exp 
            index.result = true;
          } // end else 
        } // end else 
      } // end if 
      else { // �h�^  
        index.i = last_i;
        index.result = true; 
      } // end else
    } // end else 
  } // end else if 

  return index;
} // Romre() 

ReturnResult Romse( ReturnResult index, int last_i ) { // rest_of_maybe_shift_exp, stats 1 2 
  int last_correct_i;
  int token_i = FindToken( index.i );

  //  if ( token_i == -1 ) { // waiting for remaining input 
  //    index.error = 2;
  //    index.result = true;
  //  } // end if 
  if ( index.stats == 1 ) {
    last_correct_i = last_i;
    index.i = token_i;
    index.stats = 1;
    index = Romae( index, last_correct_i ); // rest_of_maybe_additive_exp 

    if ( index.result == true ) {
      last_correct_i = index.i;
      token_i = index.i + 1;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // �u��romae  
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "<" ) { // 0��h�� 
        if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
          index.i = token_i;
          index.error = 2;
          index.result = true;
        } // end else if 
        else if ( glob_codes[token_i + 1].token == "<" ) {
          index.i = token_i + 1;
          token_i = token_i + 2;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else {
            index.i = token_i;
            index.stats = 2; 
            index = Romse( index, last_correct_i ); // �~�򩹤U���� 
          } // end else 
        } // end else if 
        else { // �u���@��'<', �h�^rest_of_maybe_additive_exp  
          index.result = true;
        } // end else 
      } // end else if 
      else if ( glob_codes[token_i].token == ">" ) { // 0��h�� 
        if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
          index.i = token_i;
          index.error = 2;
          index.result = true;
        } // end else if 
        else if ( glob_codes[token_i + 1].token == ">" ) {
          index.i = token_i + 1;
          token_i = token_i + 2;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else {
            index.i = token_i;
            index.stats = 2; 
            index = Romse( index, last_correct_i ); // �~�򩹤U���� 
          } // end else 
        } // end else if 
        else { // �u���@��'>', �h�^rest_of_maybe_additive_exp 
          index.result = true;
        } // end else 
      } // end else if 
      else { // �u��rest_of_maybe_additive_exp 
        index.result = true;
      } // end else 
    } // end if 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = M_ae( index ); // maybe_additive_exp 

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // �S����h���F 
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == "<" ) { // 0��h�� 
          if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
            index.i = token_i;
            index.error = 2;
            index.result = true;
          } // end else if 
          else if ( glob_codes[token_i + 1].token == "<" ) {
            index.i = token_i + 1;
            token_i = token_i + 2;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if 
            else {
              index.i = token_i;
              index.stats = 2; 
              index = Romse( index, last_correct_i ); // �A���դ@�� 
            } // end else 
          } // end else if 
          else { // �u���@��'<', �h�^maybe_additive_exp 
            index.result = true;
          } // end else 
        } // end else if 
        else if ( glob_codes[token_i].token == ">" ) { // 0��h�� 
          if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
            index.i = token_i;
            index.error = 2;
            index.result = true;
          } // end else if 
          else if ( glob_codes[token_i + 1].token == ">" ) {
            index.i = token_i + 1;
            token_i = token_i + 2;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if 
            else {
              index.i = token_i;
              index.stats = 2; 
              index = Romse( index, last_correct_i ); // �A���դ@�� 
            } // end else 
          } // end else if 
          else { // �u���@��'>', �h�^maybe_additive_exp 
            index.result = true;
          } // end else 
        } // end else if
        else { // �u��maybe_additive_exp 
          index.result = true;
        } // end else 
      } // end if 
      else { // �h�^  
        index.i = last_i;
        index.result = true; 
      } // end else
    } // end else 
  } // end else if 

  return index;
} // Romse() 

ReturnResult Romae( ReturnResult index, int last_i ) { // rest_of_maybe_additive_exp, stats 1 2 
  int last_correct_i;
  int token_i = FindToken( index.i );
  
  //  if ( token_i == -1 ) { // waiting for remaining input 
  //    index.error = 2;
  //    index.result = true;
  //  } // end if 
  if ( index.stats == 1 ) {
    last_correct_i = last_i;
    index.i = token_i;
    index.stats = 1;
    index = Romme( index, last_correct_i ); // rest_of_maybe_mult_exp 

    if ( index.result == true ) {
      last_correct_i = index.i;
      token_i = index.i + 1;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // �u��Romme 
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "+" || glob_codes[token_i].token == "-" ) { // �A���դ@��  
        index.i = token_i + 1;
        index.stats = 2;
        index = Romae( index, last_correct_i );
      } // end else if 
      else { // �h�^rest_of_maybe_mult_exp 
        index.result = true;
      } // end else 
    } // end if 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // waiting for remaining input 
      index.error = 2;
      index.result = true;
    } // end if 
    else {
      index.i = token_i;
      index.stats = 1;
      index = M_me( index ); // maybe_mult_exp 

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // �S����h���F 
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == "+" || glob_codes[token_i].token == "-" ) { // �A���դ@��  
          index.i = token_i + 1;
          index.stats = 2;
          index = Romae( index, last_correct_i );
        } // end else if 
        else { // �h�^maybe_mult_exp 
          index.result = true;
        } // end else 
      } // end if
      else { // �h�^  
        index.i = last_i;
        index.result = true; 
      } // end else
    } // end else 
  } // end else if 

  return index;
} // Romae() 

ReturnResult Romme( ReturnResult index, int last_i ) { // rest_of_maybe_mult_exp, stats 1 2 
// �u��Unary_exp�ണ��false 
  int last_correct_i; // �e��rom function��last_correct_i�Φb���B  
  int token_i = FindToken( index.i );
  
  if ( index.stats == 1 ) {
    if ( token_i == -1 ) { // could be empty
      index.i = last_i;
      index.result = true;
    } // end if
    else if ( glob_codes[token_i].token == "*" ) {
      last_correct_i = index.i;
      index.i = token_i + 1;
      index.stats = 2;
      index = Romme( index, last_correct_i );
    } // end if 
    else if ( glob_codes[token_i].token == "/" ) {
      last_correct_i = index.i;
      index.i = token_i + 1;
      index.stats = 2;
      index = Romme( index, last_correct_i );
    } // end if 
    else if ( glob_codes[token_i].token == "%" ) {
      last_correct_i = index.i;
      index.i = token_i + 1;
      index.stats = 2;
      index = Romme( index, last_correct_i );
    } // end if 
    else { // �N�ⳣ��, ���N��empty 
      index.i = last_i; // �h�^�W�����T����m  
      index.result = true;
    } // end else 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // waiting for remaining input  
      index.error = 2;
      index.result = true;
    } // end if
    else {
      index.i = token_i;
      index.stats = 1;
      index = Unary_exp( index ); // unary_exp 

      if ( index.result == true ) {
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // �S����h���F  
          index.result = true;
        } // end if 
        else {
          last_correct_i = index.i;
          index.i = token_i;
          index.stats = 1;
          index = Romme( index, last_correct_i );
        } // end else 
      } // end if 
      else { // �h�^  
        index.i = last_i;
        index.result = true; 
      } // end else
    } // end else 
  } // end else if 

  return index;
} // Romme()  

ReturnResult M_lae( ReturnResult index ) { // maybe_logical_AND_exp, stats 1 
  int token_i = FindToken( index.i );
  int save_i;
  
  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( index.stats == 1 ) {
    index.i = token_i;
    index.stats = 1;
    index = M_boe( index ); // maybe_bit_OR_exp 
    save_i = index.i; // �x�s��m  

    if ( index.result == true ) {
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h�F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "&" ) {
        index.i = token_i;

        if ( token_i + 1 < glob_codes.size() ) { // ����'&&'  
          if ( glob_codes[token_i + 1].token == "&" ) {
            index.i = token_i + 1;
            index.result = true;

            token_i = index.i + 1;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if
            else {
              index.i = token_i;
              index.stats = 1;
              index = M_lae( index ); // �A���դ@�� 

              if ( index.result == false ) {
                index.i = save_i; // �h�^maybe_bit_OR_exp 
                index.result = true; 
              } // end if 
              else if ( index.error == 2 ) { // ����H���ٻݭninput  
                index.error = 0;
              } // end else if 
            } // end else 
          } // end if 
          else { // �h�^maybe_bit_OR_exp 
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if 
        else { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end else 
      } // end else if 
    } // end if 
  } // end else if 

  return index;
} // M_lae() 

ReturnResult M_boe( ReturnResult index ) { // maybe_bit_OR_exp, stats 1 
  int token_i = FindToken( index.i );
  int save_i;  

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( index.stats == 1 ) {
    index.i = token_i;
    index.stats = 1;
    index = M_beoe( index ); // maybe_bit_ex_OR_exp 
    save_i = index.i;

    if ( index.result == true ) {
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h�F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "|" ) {
        index.i = token_i;
        index.result = true;
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if
        else {
          index.i = token_i;
          index.stats = 1;
          index = M_boe( index ); // �A���դ@�� 

          if ( index.result == false ) {
            index.i = save_i; // �h�^maybe_bit_ex_OR_exp 
            index.result = true; 
          } // end if 
          else if ( index.error == 2 ) { // ����H���ٻݭninput 
            index.error = 0;
          } // end else if 
        } // end else 
      } // end else if 
    } // end if 
  } // end else if 

  return index;
} // M_boe() 

ReturnResult M_beoe( ReturnResult index ) { // maybe_bit_ex_OR_exp, stats 1 
  int token_i = FindToken( index.i );
  int save_i;

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if
  else if ( index.stats == 1 ) {
    index.i = token_i;
    index.stats = 1;
    index = M_bae( index ); // maybe_bit_AND_exp 
    save_i = index.i;

    if ( index.result == true ) {
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h�F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "^" ) {
        index.i = token_i;
        index.result = true;
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if
        else {
          index.i = token_i;
          index.stats = 1;
          index = M_beoe( index ); // �A���դ@�� 

          if ( index.result == false ) {
            index.i = save_i; // �h�^maybe_bit_AND_exp 
            index.result = true; 
          } // end if 
          else if ( index.error == 2 ) { // ����H���ٻݭninput 
            index.error = 0;
          } // end else if 
        } // end else 
      } // end else if 
    } // end if 
  } // end else if  

  return index;
} // M_beoe() 

ReturnResult M_bae( ReturnResult index ) { // maybe_bit_AND_exp, stats 1  
  int token_i = FindToken( index.i );
  int save_i;

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( index.stats == 1 ) {
    index.i = token_i;
    index.stats = 1;
    index = M_ee( index ); // maybe_equality_exp 
    save_i = index.i;

    if ( index.result == true ) {
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h�F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "&" ) {
        index.i = token_i;
        index.result = true;
        token_i = index.i + 1;
        token_i = FindToken( token_i );

        if ( token_i == -1 ) { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end if
        else {
          index.i = token_i;
          index.stats = 1;
          index = M_bae( index ); // �A���դ@�� 

          if ( index.result == false ) {
            index.i = save_i; // �h�^maybe_equality_exp 
            index.result = true; 
          } // end if 
          else if ( index.error == 2 ) { // ����H���ٻݭninput 
            index.error = 0;
          } // end else if 
        } // end else 
      } // end else if 
    } // end if 
  } // end else if  

  return index;
} // M_bae() 

ReturnResult M_ee( ReturnResult index ) { // maybe_equality_exp, stats 1 
  int token_i = FindToken( index.i );
  int save_i;

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( index.stats == 1 ) {
    index.i = token_i;
    index.stats = 1;
    index = M_re( index ); // maybe_relational_exp 
    save_i = index.i;

    if ( index.result == true ) {
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // �S����h�F 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "!" ) {
        index.i = token_i;
        if ( token_i + 1 < glob_codes.size() ) { // ����'!='  
          if ( glob_codes[token_i + 1].token == "=" ) {
            index.i = token_i + 1;
            index.result = true;
            token_i = index.i + 1;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if
            else {
              index.i = token_i;
              index.stats = 1;
              index = M_ee( index ); // �A���դ@�� 

              if ( index.result == false ) {
                index.i = save_i; // �h�^maybe_relational_exp 
                index.result = true; 
              } // end if 
              else if ( index.error == 2 ) { // ����H���ٻݭninput 
                index.error = 0;
              } // end else if 
            } // end else 
          } // end if 
          else { // �h�^maybe_relational_exp 
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if 
        else { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end else 
      } // end else if 
      else if ( glob_codes[token_i].token == "=" ) {
        index.i = token_i;
        if ( token_i + 1 < glob_codes.size() ) { // ����'=='  
          if ( glob_codes[token_i + 1].token == "=" ) {
            index.i = token_i + 1;
            index.result = true;
            token_i = index.i + 1;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // waiting for remaining input 
              index.error = 2;
              index.result = true;
            } // end if
            else {
              index.i = token_i;
              index.stats = 1;
              index = M_ee( index ); // �A���դ@�� 

              if ( index.result == false ) {
                index.i = save_i; // �h�^maybe_relational_exp 
                index.result = true; 
              } // end if
              else if ( index.error == 2 ) { // ����H���ٻݭninput 
                index.error = 0;
              } // end else if 
            } // end else 
          } // end if 
          else { // �h�^maybe_relational_exp 
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if 
        else { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end else 
      } // end else if 
      else { // �u��maybe_relational_exp  
        index.result = true; 
      } // end else 
    } // end if 
  } // end else if  
  
  return index;
} // M_ee() 

ReturnResult M_re( ReturnResult index ) { // maybe_relational_exp, stats 1 
  int token_i = FindToken( index.i );
  int save_i;

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if
  else if ( index.stats == 1 ) {
    index.i = token_i;
    index.stats = 1;
    index = M_se( index ); // maybe_shift_exp 
    save_i = index.i;

    if ( index.result == true ) {
      token_i = index.i + 1;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // �S����h�F 
        index.result = true;
      } // end if 
      else {
        if ( glob_codes[token_i].token == "<" ) {
          index.i = token_i;
          if ( token_i + 1 < glob_codes.size() ) { // ����'<=' 
            if ( glob_codes[token_i + 1].token == "=" ) {
              index.i = token_i + 1;
              index.result = true;
            } // end if 
          } // end if 

          token_i = index.i + 1;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if
          else {
            index.i = token_i;
            index.stats = 1;
            index = M_re( index ); // �A���դ@�� 

            if ( index.result == false ) {
              index.i = save_i; // �h�^maybe_shift_exp 
              index.result = true; 
            } // end if 
            else if ( index.error == 2 ) { // ����H���ٻݭninput 
              index.error = 0;
            } // end else if 
          } // end else 
        } // end if
        else if ( glob_codes[token_i].token == ">" ) {
          index.i = token_i;
          if ( token_i + 1 < glob_codes.size() ) { // ����'>=' 
            if ( glob_codes[token_i + 1].token == "=" ) {
              index.i = token_i + 1;
              index.result = true;
            } // end if 
          } // end if 

          token_i = index.i + 1;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if
          else {
            index.i = token_i;
            index.stats = 1;
            index = M_re( index ); // �A���դ@�� 

            if ( index.result == false ) {
              index.i = save_i; // �h�^maybe_shift_exp 
              index.result = true; 
            } // end if 
            else if ( index.error == 2 ) { // ����H���ٻݭninput 
              index.error = 0;
            } // end else if 
          } // end else 
        } // end else if 
        else { // �u��maybe_shift_exp   
          index.result = true;
        } // end else 
      } // end else 
    } // end if 
  } // end else if  

  return index;
} // M_re() 

ReturnResult M_se( ReturnResult index ) { // maybe_shift_exp, stats 1 
  int token_i = FindToken( index.i );
  int save_i;

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( index.stats == 1 ) {
    index.i = token_i;
    index.stats = 1;
    index = M_ae( index ); // maybe_additive_exp 
    save_i = index.i;

    if ( index.result == true ) {
      token_i = index.i + 1;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // �S����h�F  
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "<" ) { // 0��h�� 
        if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
          index.i = token_i;
          index.error = 2;
          index.result = true;
        } // end else if 
        else if ( glob_codes[token_i + 1].token == "<" ) {
          index.i = token_i + 1;
          token_i = token_i + 2;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else {
            index.i = token_i;
            index.stats = 1; 
            index = M_se( index ); // �A���դ@��  

            if ( index.result == false ) {
              index.i = save_i; // maybe_additive_exp 
              index.result = true; 
            } // end if 
            else if ( index.error == 2 ) { // ����H���ٻݭninput 
              index.error = 0;
            } // end else if 
          } // end else 
        } // end else if 
        else { // �u���@��'<', �h�^maybe_additive_exp 
          index.result = true;
        } // end else 
      } // end else if 
      else if ( glob_codes[token_i].token == ">" ) { // 0��h�� 
        if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
          index.i = token_i;
          index.error = 2;
          index.result = true;
        } // end else if 
        else if ( glob_codes[token_i + 1].token == ">" ) {
          index.i = token_i + 1;
          token_i = token_i + 2;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end if 
          else {
            index.i = token_i;
            index.stats = 1; 
            index = M_se( index ); // �A���դ@��  

            if ( index.result == false ) {
              index.i = save_i; // maybe_additive_exp 
              index.result = true; 
            } // end if 
            else if ( index.error == 2 ) { // ����H���ٻݭninput 
              index.error = 0;
            } // end else if 
          } // end else  
        } // end else if 
        else { // �u���@��'>', �h�^maybe_additive_exp 
          index.result = true;
        } // end else 
      } // end else if 
      else { // �u��maybe_additive_exp 
        index.result = true;
      } // end else 
    } // end if 
  } // else if 

  return index;
} // M_se() 

ReturnResult M_ae( ReturnResult index ) { // maybe_additive_exp, stats 1 
  int token_i = FindToken( index.i );
  int save_i;
  
  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( index.stats == 1 ) {
    index.i = token_i;
    index.stats = 1;
    index = M_me( index ); // maybe_mult_exp 
    save_i = index.i;

    if ( index.result == true ) {
      token_i = index.i + 1;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // �S����h�F 
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "+" || glob_codes[token_i].token == "-" ) { // �A���դ@��  
        index.i = token_i + 1;
        index.stats = 1;
        index = M_ae( index );

        if ( index.result == false ) {
          index.i = save_i; // maybe_mult_exp 
          index.result = true; 
        } // end if 
        else if ( index.error == 2 ) { // ����H���ٻݭninput 
          index.error = 0;
        } // end else if 
      } // end else if 
      else { // �h�^maybe_mult_exp 
        index.result = true;
      } // end else 
    } // end if 
  } // else if 

  return index;
} // M_ae() 

ReturnResult M_me( ReturnResult index ) { // maybe_mult_exp, stats 1 
  int token_i = FindToken( index.i );
  int save_i;
  
  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( index.stats == 1 ) {
    index.i = token_i;
    index.stats = 1;
    index = Unary_exp( index ); // unary_exp 

    if ( index.result == true ) {
      save_i = index.i;
      index.i++;
      index.stats = 1;
      index = Romme( index, save_i ); // rest_of_maybe_mult_exp 
    } // end if 
  } // end else if 

  return index;
} // M_me() 

int main() {
  char ch;
  int x_axis = 1, y_axis = 1;
  int brk = 0;
  int uTestNum;
  string str, token_buffer, peek;
  bool quit;
  // cin >> uTestNum;
  Cin_cout();
  Set_type();
  cout << "Our-C running ..." << endl;
  cout << "> ";
  while ( brk == 0 ) {
    if ( cin.peek() ) {
      ch = cin.get();

      stringstream char_to_string;

      char_to_string << ch;
      str = char_to_string.str(); // str = ch  

      if ( Is_White_Space( str ) == true ) { // is whitespace, cutting token  
        Lines element;
        if ( token_buffer.size() == 0 ) { // token buffer���S�F�� �[�Jwhitespace 
          element.row = x_axis;
          element.column = y_axis;
          y_axis = y_axis + str.size();
          element.token = str;
          element.type = "whitespace";
          glob_codes.push_back( element );
        } // end if 
        else { // token buffer�����F�� 
          element.row = x_axis;
          element.column = y_axis;
          y_axis = y_axis + token_buffer.size();
          element.token = token_buffer;
          if ( Is_NUM( token_buffer ) == "integer" || Is_NUM( token_buffer ) == "float" ) {
            element.type = Is_NUM( token_buffer );
          } // end if 
          else {
            element.type = "unknown";
          } // end else
 
          glob_codes.push_back( element );

          element.row = x_axis; // �[�Jwhitespace 
          element.column = y_axis;
          y_axis = y_axis + str.size();
          element.token = str;
          element.type = "whitespace";
          glob_codes.push_back( element );
        } // end else 

        token_buffer.clear(); // token buffer�M��  
        if ( str == "\n" ) {
          // cout << "start analyzing statement" << endl; 
          Line_Comment();
          Num_split_with_unrecog();
          Examine_float();
          Char_String_tab_newline();
          Update_type();
          // PrintCodes();
          quit = Syntax_check( 0 );


          if ( quit == true ) {
            brk = 1; // break 
          } // end if 
          else {
            glob_line++;
            x_axis++;
            y_axis = 1;
          } // end else 
        } // end if, row+1 if token is newline 
      } // end if 
      else if ( Is_SP_Token( str ) == true ) { // �p�G�O�S�O��token 
        if ( cin.peek() != '\n' && cin.peek() != ' ' && cin.peek() != '\t' ) { // ���ݫ᭱��char 
          ch = cin.get();
          stringstream ss;
          ss << ch;
          peek = ss.str(); // peek = ch  
          Lines element;
          if ( ( peek == str && peek == "\\" ) || ( peek == str && peek == "/" ) ) { 
          // line-comment, '/', '\\' 
            if ( token_buffer.size() == 0 ) { // token buffer���S�F�� �����[�J  
              element.row = x_axis;
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end if 
            else { // token buffer�����F�� 
              element.row = x_axis;
              element.column = y_axis;
              y_axis = y_axis + token_buffer.size();
              element.token = token_buffer;
              if ( Is_NUM( token_buffer ) == "integer" || Is_NUM( token_buffer ) == "float" ) {
                element.type = Is_NUM( token_buffer );
              } // end if 
              else {
                element.type = "unknown";
              } // end else 

              glob_codes.push_back( element );

              element.row = x_axis; // �����[�J 
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end else 

            token_buffer.clear(); // token buffer�M��  
          } // end if 
          else if ( ( str == "\\" && peek == "\'" ) || ( str == "\\" && peek == "\"" ) ) {
          // line-comment, '\'', '\"' , �]�N�O�n�L�X " or '  
            if ( token_buffer.size() == 0 ) { // token buffer���S�F�� �����[�J  
              element.row = x_axis;
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end if 
            else { // token buffer�����F�� 
              element.row = x_axis;
              element.column = y_axis;
              y_axis = y_axis + token_buffer.size();
              element.token = token_buffer;
              if ( Is_NUM( token_buffer ) == "integer" || Is_NUM( token_buffer ) == "float" ) {
                element.type = Is_NUM( token_buffer );
              } // end if 
              else {
                element.type = "unknown";
              } // end else 

              glob_codes.push_back( element );

              element.row = x_axis; // �����[�J 
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end else 

            token_buffer.clear(); // token buffer�M�� 
          } // end else if 
          else if ( ( str == "\\" && peek == "n" ) || ( str == "\\" && peek == "t" ) ) {
          // tab�Pnewline   
            if ( token_buffer.size() == 0 ) { // token buffer���S�F�� �����[�J  
              element.row = x_axis;
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end if 
            else { // token buffer�����F�� 
              element.row = x_axis;
              element.column = y_axis;
              y_axis = y_axis + token_buffer.size();
              element.token = token_buffer;
              if ( Is_NUM( token_buffer ) == "integer" || Is_NUM( token_buffer ) == "float" ) {
                element.type = Is_NUM( token_buffer );
              } // end if 
              else {
                element.type = "unknown";
              } // end else 

              glob_codes.push_back( element );

              element.row = x_axis; // �����[�J 
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end else 

            token_buffer.clear(); // token buffer�M�� 
          } // end else if 
          else { // str �P peek����Ӥ��Ptoken�B�z 
            if ( Is_Token( peek ) == true ) { // peek��token  
              if ( token_buffer.size() == 0 ) { // token buffer���S�F�� �����[�J  
                element.row = x_axis;
                element.column = y_axis;
                y_axis = y_axis + str.size();
                element.token = str;
                element.type = "token";
                glob_codes.push_back( element );

                element.row = x_axis;
                element.column = y_axis;
                y_axis = y_axis + peek.size();
                element.token = peek;
                element.type = "token";
                glob_codes.push_back( element );
              } // end if 
              else { // token buffer�����F��  
                element.row = x_axis;
                element.column = y_axis;
                y_axis = y_axis + token_buffer.size();
                element.token = token_buffer;
                if ( Is_NUM( token_buffer ) == "integer" || Is_NUM( token_buffer ) == "float" ) {
                  element.type = Is_NUM( token_buffer );
                } // end if 
                else {
                  element.type = "unknown";
                } // end else 

                glob_codes.push_back( element );

                element.row = x_axis; // �����[�J
                element.column = y_axis;
                y_axis = y_axis + str.size();
                element.token = str;
                element.type = "token";
                glob_codes.push_back( element );

                element.row = x_axis;
                element.column = y_axis;
                y_axis = y_axis + peek.size();
                element.token = peek;
                element.type = "token";
                glob_codes.push_back( element );
              } // end else 

              token_buffer.clear(); // token buffer�M��  
            } // end if 
            else { // peek����token  
              if ( token_buffer.size() == 0 ) { // token buffer���S�F�� �����[�J  
                element.row = x_axis;
                element.column = y_axis;
                y_axis = y_axis + str.size();
                element.token = str;
                element.type = "token";
                glob_codes.push_back( element );

                token_buffer = token_buffer + peek;
              } // end if 
              else { // token buffer�����F��  
                element.row = x_axis;
                element.column = y_axis;
                y_axis = y_axis + token_buffer.size();
                element.token = token_buffer;
                if ( Is_NUM( token_buffer ) == "integer" || Is_NUM( token_buffer ) == "float" ) {
                  element.type = Is_NUM( token_buffer );
                } // end if 
                else {
                  element.type = "unknown";
                } // end else 

                glob_codes.push_back( element );

                element.row = x_axis; // �����[�J
                element.column = y_axis;
                y_axis = y_axis + str.size();
                element.token = str;
                element.type = "token";
                glob_codes.push_back( element );

                token_buffer.clear(); // token buffer�M��
                token_buffer = token_buffer + peek;
              } // end else 
            } // end else 
          } // end else 
        } // end if 
        else { // �᭱�S��char�F  ��ܥu����@��   
          Lines element;
          if ( token_buffer.size() == 0 ) { // token buffer���S�F�� �����[�J  
            element.row = x_axis;
            element.column = y_axis;
            y_axis = y_axis + str.size();
            element.token = str;
            element.type = "token";
            glob_codes.push_back( element );
          } // end if 
          else { // token buffer�����F�� 
            element.row = x_axis;
            element.column = y_axis;
            y_axis = y_axis + token_buffer.size();
            element.token = token_buffer;
            if ( Is_NUM( token_buffer ) == "integer" || Is_NUM( token_buffer ) == "float" ) {
              element.type = Is_NUM( token_buffer );
            } // end if 
            else {
              element.type = "unknown";
            } // end else 

            glob_codes.push_back( element );

            element.row = x_axis; // �����[�J  
            element.column = y_axis;
            y_axis = y_axis + str.size();
            element.token = str;
            element.type = "token";
            glob_codes.push_back( element );
          } // end else 

          token_buffer.clear(); // token buffer�M�� 
        } // end else 
      } // end else if 
      else if ( Is_Token( str ) == true ) { // �p�G�O���q��token 
        Lines element;
        if ( token_buffer.size() == 0 ) { // token buffer���S�F�� �����[�J  
          element.row = x_axis;
          element.column = y_axis;
          y_axis = y_axis + str.size();
          element.token = str;
          element.type = "token";
          glob_codes.push_back( element );
        } // end if 
        else { // token buffer�����F�� 
          element.row = x_axis;
          element.column = y_axis;
          y_axis = y_axis + token_buffer.size();
          element.token = token_buffer;
          if ( Is_NUM( token_buffer ) == "integer" || Is_NUM( token_buffer ) == "float" ) {
            element.type = Is_NUM( token_buffer );
          } // end if 
          else {
            element.type = "unknown";
          } // end else 

          glob_codes.push_back( element );

          element.row = x_axis; // �����[�J  
          element.column = y_axis;
          y_axis = y_axis + str.size();
          element.token = str;
          element.type = "token";
          glob_codes.push_back( element );
        } // end else 

        token_buffer.clear(); // token buffer�M�� 	
      } // end else if 
      else { // keep getting token
        token_buffer = token_buffer + str;
      } // end else 
    } // end if 
  } // end while 

  return 0;
} // main() 
