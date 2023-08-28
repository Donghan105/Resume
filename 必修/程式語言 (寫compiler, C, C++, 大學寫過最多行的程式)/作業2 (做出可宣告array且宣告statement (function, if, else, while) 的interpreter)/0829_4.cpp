// StyleCheckType vector
# include <iostream>
# include <stdio.h>
# include <string> // string型別, strcpy, assign, substr, erase, find, erase, strstr 
# include <vector>  // 動態陣列 
# include <stdlib.h> // atoi 
# include <sstream> // char to string  
using namespace std;

struct Lines{ 
  int row;
  int column;
  string token;
  string type;
}; // 使用者輸入的code 

struct ReturnResult{
  int i;
  bool result;
  int stats;
  int error;
  int unrecognized;
  int unexpected;
  int undefined;
  int defining;
}; // 檢查文法時的return  

// error 1: Error 
// error 2: Waiting for remaining input 
// error 3: Quit 

struct Defined{
  string variable; // 變數名稱  
  string data; // 內容  
  string type; // 型別 
  string code;
  int times; // 第幾次被定義  
};

struct CalStack{
  string data; // 內容  
  string type; // 型別 
  int error;
};

// error 1: Error 
// error 2: Waiting for remaining input 
// error 3: Quit 


vector<Lines> glob_codes; // 使用者輸入的code  
vector<Defined> glob_defined_list; // 已宣告的variable 
vector<Defined> glob_last_defined_list; // 紀錄上一次已宣告的variable 
vector<Defined> glob_compnd_defined_list; // 紀錄{int a, b... } 
vector<Defined> glob_multi_declaration; // 紀錄int a, b...  

// OurC feature variables 
int glob_type_wrong = -1, glob_line = 1;
int glob_cmpnd_statmnt = 0;

int FindToken( int i ) ; // 跳過whitespace 找出下個token 
bool Type_list( string str ) ;
bool Token_list( string str ) ; // 判斷文法時 合法的token 
bool Is_Token( string str ) ; // 切token時 所需要辨識出的token  
bool Is_SP_Token( string str ) ; // special token, line-comment, '/', '\\' 
bool Is_White_Space( string TMP ) ; // whitespace 
string Is_NUM( string str ) ; // 文字處理, ascii code判定是否為數字  
bool Is_CONST( string str ) ;
bool Is_zero( string str ) ;
bool Is_IDENT( string str, int index ) ; // identifier 
string Not_IDENT( string token ) ; // 含有範圍外字元, 不是ident, 將ident轉為該範圍外字元 @bc -> @ 
int FindFarthest( vector<int>& arr, int index ) ;

void Line_Comment() ; // if found line-comment, delete words after line-comment 
void Num_split_with_unrecog() ; // 20& -> 20 &, 35.5@ -> 35.5 @, 40.2.2@ -> 40.2 .2 @ 
void Examine_float() ; // 檢查是不是浮點數 
void Double_decimal_3( double d ) ; // printf(%1.3f) 
void Unrecognized_or_Unexpected( string str, int index, int line ) ; // 檢查是否所有ident都已經define過  
void Print_Unrecognized_or_Unexpected( ReturnResult tmp ) ;
bool Find_IDENT( string str ) ; // make sure that this IDENT is defined or not
bool Find_compnd_IDENT( string str ) ;
bool Find_IDENT_in_last_version( string str ) ;
Defined Get_IDENT_value( string str ) ; // make sure that this IDENT is defined or not
Defined Overwrite_IDENT( Defined new_element ) ;
void Del_IDENT( string var ) ;
int Check_divide_zero() ; // 回傳0: 此行沒有token or 沒錯, 回傳-1: 有undefined token, 回傳1: 發生除以0 
int Check_all_defined( int start, int end ) ; // 檢查是兩種中哪一種錯誤  
int Define_new_or_not() ;

void ClearTakenToken( int j ) ; // del the token that already gets 
int Syntax_check( int i ) ; // 檢查有無文法錯誤  

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
ReturnResult Definition( ReturnResult index ) ; // 定義func or 宣告變數  ok 
ReturnResult Type_specifier( ReturnResult index ) ; // 除了void以外的型別  ok 
ReturnResult Rest_of_declarators( ReturnResult index ) ; // rest_of_declarators, stats 1 2 3 ok 
ReturnResult Function_definition( ReturnResult index, int last_i ) ; // 定義func, last_i ok 
ReturnResult Formal_parameter_list( ReturnResult index, int last_i ) ; // stats 1 2, last_i ok 
ReturnResult Compound_statement( ReturnResult index, int last_i ) ; // Compound_statement, stats 1 2 
ReturnResult Declaration( ReturnResult index ) ;   // 宣告變數 ok 
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

ReturnResult Unary_exp( ReturnResult index ) ; // 分辨++ --是sign 還是 PP/MM 
ReturnResult Signed_unary_exp( ReturnResult index ) ;
ReturnResult Unsigned_unary_exp( ReturnResult index ) ;

int FindToken( int i ) { // 跳過whitespace 找出下個token  
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

bool Token_list( string str ) { // 判斷文法時 合法的token 
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

bool Is_Token( string str ) { // 切token時 所需要辨識出的token  
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

string Is_NUM( string str ) { // 文字處理, ascii code判定是否為數字  
  bool result = true;
  int dotcount = 0;
  for ( int i = 0;  i < str.size() ; i++ ) { // 一個字元一個字元 判斷 
    if ( ( int ) str[i] == 46 ) {
      dotcount++;
      if ( dotcount > 1 ) {
        result = false;
        i = str.size();
      } // end if 
    } // end if 
    else if ( i == 0 && str.size() > 1 && ( ( int ) str[i] == 45 || ( int ) str[i] == 43 ) ) {
      // +, - sign (不能只有 + or -, 整個str需有其他數字 )
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

  for ( int i = 1; i < str.size() ; i++ ) { // 剩餘的char  
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
  //      if ( Is_White_Space( glob_codes[i].token ) == true ) { // 若此quit前面只有whitespace(此為第一個token) 
  //        result = false;
  //      } // end if 
  //      else { // quit := 3; 是ident  
  //        result = true;
  //        i = index; // break
  //      } // end else 
  //    } // end for 
  //  } // end if 

  return result;
} // Is_IDENT() 

string Not_IDENT( string token ) { // 含有範圍外字元, 不是ident, 將ident轉為該範圍外字元 @bc -> @ 
  string str;
  str = token;
  if ( ( ( int ) token[0] < 65 || ( int ) token[0] > 90 ) ) {  // 1st char not letters
    if ( ( ( int ) token[0] < 97 || ( int ) token[0] > 122 ) ) {
      str = token[0];
    } // end if
  } // end if

  if ( str == token ) { // 若通過第一個char的考驗 ( 後續可有字母, degits, 底線 )  
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
  //  if ( token.find( "quit" ) != -1 ) { // 若quit@ 轉為 quit  
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
            else { // 出現2nd dot, 開始切token   
              Lines newtoken;
              newtoken.token = str.substr( end + 1, str.size() - end - 1 ); // 被切成在後段的token 
              newtoken.type = "unknown";
              newtoken.column = glob_codes[i].column + 1;
              newtoken.row = glob_codes[i].row + 1;
              glob_codes.insert( glob_codes.begin() + i + 1, newtoken ); // 插入在i+1 

              glob_codes[i].token = str.substr( 0, end + 1 ); // 被切成在前段的token 
              glob_codes[i].type = "float";

              j = str.size(); // break for 
            } // end else 
          } // end else if  
          else { // 分割成2個token 
            Lines newtoken;
            newtoken.token = str.substr( end + 1, str.size() - end - 1 ); // 被切成在後段的token 
            newtoken.type = "unknown";
            newtoken.column = glob_codes[i].column + 1;
            newtoken.row = glob_codes[i].row + 1;
            glob_codes.insert( glob_codes.begin() + i + 1, newtoken ); // 插入在i+1 

            glob_codes[i].token = str.substr( 0, end + 1 ); // 被切成在前段的token 

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
          else { // 切不出任何數字 
            j = str.size(); // break for 
          } // end else
        } // end else if 
      } // end for 
    } // end if 
  } // end for
} // Num_split_with_unrecog() 

void Examine_float() { // 檢查是不是浮點數 
  int flag = 0, start, end;
  int dotcount = 0;
  string temp = "NULL";
  for ( int i = 0; i < glob_codes.size() ; i++ ) { // 使用者輸入的code的token  
    if ( glob_codes[i].type == "integer" ) { // 此token為整數  
      if ( flag == 0 ) { // 前面還不認為是小數  
        temp = glob_codes[i].token;
        start = i;
        flag = 1; // 從現在起開始將tokens組合為小數 
      } // end if 
      else if ( flag == 1 ) { // 已認定是小數  
        temp += glob_codes[i].token; // 開始組合小數  
      } // end if 
    } // end if 
    else if ( glob_codes[i].token == "." ) { // 此token為'.' 
      if ( flag == 0 ) { // 前面還未有其他數字 此小數能為.23  
        flag = 1; // 從現在起開始將tokens組合為小數 
        start = i;
        dotcount++; // 紀錄碰到的'.'數量 
        temp = "."; // 小數buffer 
      } // end if 
      else if ( flag == 1 ) { // 從現在起開始將tokens組合為小數 
        if ( dotcount == 0 ) {
          dotcount++; // 紀錄碰到的'.'數量 
          temp += "."; // 小數buffer 
        } // end if 
        else { // 遇到第二個'.' 2..1-> 2. .1兩小數  
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

void Unrecognized_or_Unexpected( string str, int index, int line ) { // 檢查是否所有ident都已經define過  
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
  else { // 肯定是拿到不要的東西 需驗證是哪種錯誤 (ex: 此token是Unrecognized, Unexpected)
    Unrecognized_or_Unexpected( glob_codes[tmp.i].token, tmp.i, glob_line );
  } // end else 

  //      int new_var = Define_new_or_not();
  //      while ( new_var != -1 ) { // 此行有error, 若有新宣告變數則需移除  
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

int Check_divide_zero() { // 回傳0: 此行沒有token or 沒錯, 回傳-1: 有undefined token, 回傳1: 發生除以0  
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

int Check_all_defined( int start, int end ) { // 檢查是兩種中哪一種錯誤  
  int result = -1;
  if ( end == -1 ) { end = glob_codes.size() - 1; } // end for, special case, 待使用者補上input  

  for ( int i = end; i >= start ; i-- ) { // 倒著走 就能得到最早的錯誤index 
    if ( glob_codes[i].type == "IDENT" ) {
      if ( Find_IDENT( glob_codes[i].token ) == false ) {
        result = i;
      } // end if 
      else { // 有找到, 先確定是否有給予初始值
        Defined element;
        int index = i + 1;
        element = Get_IDENT_value( glob_codes[i].token );
        index = FindToken( index );
        if ( element.data == "unknown" && glob_codes[index].token != ":=" ) {
          result = i;
          for ( int j = 0; j < glob_defined_list.size() ; j++ ) {
            if ( glob_defined_list[j].variable == glob_codes[i].token ) {
              glob_defined_list.erase( glob_defined_list.begin() + j );
              j = glob_defined_list.size(); // break此for 
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

int Syntax_check( int i ) { // 檢查有無文法錯誤  
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
  glob_last_defined_list = glob_defined_list; // 保存最初版  

  while ( tmp.error == 0 && tmp.result == true && glob_codes.size() > 0 && dvd_0 == 0 ) {
    Set_compnd_defined_list();
    tmp.result = false;
    tmp = User_input( tmp, i );
    if ( tmp.result == false ) {
      Print_Unrecognized_or_Unexpected( tmp );
      ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
    } // end if
    else {
      index = FindToken( 0 );
      if ( FindToken( 0 ) == -1 ) { // 沒有可用token 只有whitespace  
        ClearTakenToken( glob_codes.size() - 1 ); // 清除整行	
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
            ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
          } // end else
        } // end if 
        else {
          tmp.result = false;
          tmp.i = done;
          Print_Unrecognized_or_Unexpected( tmp );
          ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
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
              ClearTakenToken( tmp.i ); // 清除已拿過 (不缺任何待下次輸入補上的資料 確定文法已完整解析 ) 
              tmp.i = i; // 從index 0 重新開始分析下一批文法  
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
              ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
            } // end else 
          } // end else if 
          else {
            tmp.result = false;
            tmp.i = done;
            tmp.undefined = -1;
            tmp.unexpected = -1;
            tmp.unrecognized = -1;
            Print_Unrecognized_or_Unexpected( tmp );
            ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
          } // end else
        } // end if 
        else {
          tmp.result = false;
          tmp.i = done;
          Print_Unrecognized_or_Unexpected( tmp );
          ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
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
                // 清除已拿過 (不缺任何待下次輸入補上的資料 確定文法已完整解析 ) 
                tmp.i = i; // 從index 0 重新開始分析下一批文法  
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
                ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
              } // end else 
            } // end if 
            else {
              tmp.result = false;
              tmp.i = done;
              tmp.undefined = -1;
              tmp.unexpected = -1;
              tmp.unrecognized = -1;
              Print_Unrecognized_or_Unexpected( tmp );
              ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
            } // end else 
          } // end else if 
          else {
            tmp.result = false;
            tmp.i = done;
            tmp.undefined = -1;
            tmp.unexpected = -1;
            tmp.unrecognized = -1;
            Print_Unrecognized_or_Unexpected( tmp );
            ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
          } // end else 
        } // end if 
        else {
          tmp.result = false;
          tmp.i = done;
          Print_Unrecognized_or_Unexpected( tmp );
          ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
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
              ClearTakenToken( tmp.i ); // 清除已拿過 (不缺任何待下次輸入補上的資料 確定文法已完整解析 ) 
              tmp.i = i; // 從index 0 重新開始分析下一批文法  
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
              ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
            } // end else 
          } // end else if 
          else {
            tmp.result = false;
            tmp.i = done;
            tmp.undefined = -1;
            tmp.unexpected = -1;
            tmp.unrecognized = -1;
            Print_Unrecognized_or_Unexpected( tmp );
            ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
          } // end else
        } // end if 
        else {
          tmp.result = false;
          tmp.i = done;
          Print_Unrecognized_or_Unexpected( tmp );
          ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
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
                // 清除已拿過 (不缺任何待下次輸入補上的資料 確定文法已完整解析 ) 
                tmp.i = i; // 從index 0 重新開始分析下一批文法  
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
                ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
              } // end else 
            } // end if 
            else {
              tmp.result = false;
              tmp.i = done;
              tmp.undefined = -1;
              tmp.unexpected = -1;
              tmp.unrecognized = -1;
              Print_Unrecognized_or_Unexpected( tmp );
              ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
            } // end else 
          } // end else if 
          else {
            tmp.result = false;
            tmp.i = done;
            tmp.undefined = -1;
            tmp.unexpected = -1;
            tmp.unrecognized = -1;
            Print_Unrecognized_or_Unexpected( tmp );
            ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
          } // end else 
        } // end if 
        else {
          tmp.result = false;
          tmp.i = done;
          Print_Unrecognized_or_Unexpected( tmp );
          ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
        } // end else 
      } // end if
      else if ( tmp.undefined != -1 ) { // // error 4: Undefined 
        string token = glob_codes[tmp.undefined].token;
        cout << "Line " << glob_line << " : undefined identifier : \'" << token << "\'" << endl;
        tmp.result = false;
        
        //        int new_var = Define_new_or_not();
        //        while ( new_var != -1 ) { // 此行有error, 若有新宣告變數則需移除  
        //          Del_IDENT( glob_codes[new_var].token );
        //          glob_codes.erase( glob_codes.begin() + new_var );
        //          new_var = Define_new_or_not();		
        //       } end while 

        ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
      } // end else if 
      else if ( tmp.error == 2 ) {  // && tmp.i == 0 
      // waiting for remaining input (ex: abc\n) 
        for ( int i = 0; i < glob_codes.size() ; i++ ) {
          if ( glob_codes[i].type != "string" && glob_codes[i].type != "char" ) {
            if ( glob_codes[i].type != "DefType" && glob_codes[i].type != "Condition" ) {
              if ( glob_codes[i].type != "Return") {
                if ( Find_IDENT_in_last_version( glob_codes[i].token ) == false ) {
                  Del_IDENT( glob_codes[i].token ); // 先刪除 待指令完全再定義  
                } // end if 
              } // end if 
            } // end if
          } // end if 
        } // end for
      } // end else if 
//      else if ( tmp.error == 2 && tmp.defining != -1 ) { 
//      // 有 ':=', waiting for remaining input ( ex: abc\n :=\n ) 
//      } // end else if 
      else { // 開始確認除以0 
        // dvd_0 = Check_divide_zero(); // 回傳0: 此行沒有token or 沒錯, 回傳-1: 有undefined token, 回傳1: 發生除以0 
        if ( dvd_0 == -1 ) {
          string token = glob_codes[defined_or_not].token;
          defined_or_not = Check_all_defined( 0, glob_codes.size() - 1 ) ;
          cout << "Line " << glob_line << " : undefined identifier : \'" << token << "\'" << endl;

          //          int new_var = Define_new_or_not();
          //          while ( new_var != -1 ) { // 此行有error, 若有新宣告變數則需移除  
          //            Del_IDENT( glob_codes[new_var].token );
          //            glob_codes.erase( glob_codes.begin() + new_var );
          //            new_var = Define_new_or_not();		
          //          } 

          tmp.result = false;
          // tmp.error = 4;
          tmp.undefined = defined_or_not;
          ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
        } // end if 
        else if ( dvd_0 == 1 ) {
          tmp.error = 1;
          tmp.result = false;
          ClearTakenToken( glob_codes.size() - 1 ); // 清除整行
        } // end else if 
        else if ( tmp.error == 2 ) {
        // // waiting for remaining input ( ex: abc + \n ) 
        } // end else if 
        else { // 開始進入運算 
          // result = Calculation( tmp.i, glob_codes ); // 開始計算 
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
            // cout << result.data << endl; // 印出計算結果  
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
          ClearTakenToken( tmp.i ); // 清除已拿過 (不缺任何待下次輸入補上的資料 確定文法已完整解析 ) 
          tmp.i = i; // 從index 0 重新開始分析下一批文法  
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
    glob_last_defined_list = glob_defined_list; // 保存上一版  
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
  else if ( tmp.error == 2 ) { // 待下次使用者輸入補上資料  
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
    // ClearTakenToken( tmp.i ); // 清除整行 
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

  for ( int i = 0; i < glob_codes.size() ; i++ ) { // 先處理string  
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
      for ( int j = start + 1; j < end ; j++ ) { // 兩個/" 之間  
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

      for ( int j = start + 1; j <= end ; j++ ) { // 清除已拿token  
        glob_codes.erase( glob_codes.begin() + start + 1 );
        i--;
      } // end for

      start = -1;
      end = -1;
    } // end if 
    else if ( start != -1 && i == glob_codes.size() - 1 ) { // 沒有第二個/"  
      
    } // end else if 
  } // end for 

  for ( int i = 0; i < glob_codes.size() ; i++ ) { // 處理char  
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
      for ( int j = start + 1; j < end ; j++ ) { // 兩個/' 之間  
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

      for ( int j = start + 1; j <= end ; j++ ) { // 清除已拿token  
        glob_codes.erase( glob_codes.begin() + start + 1 );
        i--;
      } // end for

      start = -1;
      end = -1;
    } // end if 
    else if ( start != -1 && i == glob_codes.size() - 1 ) { // 沒有第二個/'  
      
    } // end else if 
  } // end for 
} // Char_String_tab_newline() 

int Executed_or_Entered() { // 回傳 -1:executed, else: 回傳defined id的index  
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
          indentation = indentation + " "; // 一次縮排縮一格  
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
          if ( if_else_while == 1 ) { // if之後只有一行  
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
        //          index = User_input( index, last_correct_i ); // 繼續嘗試   
        //        } // end if
      } // end if 
      //      else {
      //        last_correct_i = index.i;
      //        index.i++;
      //        index.stats = 2;
      //        index = User_input( index, last_correct_i ); // 繼續嘗試   
      //      } // end else 
    } // end else 
  } // end if 
  //  else if ( index.stats == 2 ) { // 可有可無  
  //    if ( token_i == -1 ) { // 沒有更多了  
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
  //          index = User_input( index, last_correct_i ); // 繼續嘗試   
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
  //        index = User_input( index, last_correct_i ); // 繼續嘗試   
  //      } // end else 
  //    } // end else 
  //
  //    if ( index.result == false ) {
  //      index.i = last_i;
  //      index.result = true;
  //    } // end if 
  //    else if ( index.error == 2 ) { // 防止以為還需要input 
  //      index.error = 0;
  //   } // end else if 
  // } // end else if

  farthest.clear();
  return index;
} // User_input() 

ReturnResult Definition( ReturnResult index ) { // 定義func or 宣告變數  
  int save_i, last_correct_i, ident_i;
  int token_i = FindToken( index.i );
  vector <int> farthest;

  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( glob_codes[token_i].token == "void" ) {
    // 1.定義void func 
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
          // 2.定義char int...等 func 
          index = Function_definition( index, last_correct_i ); // function_definition_without_ID 

          if ( index.result == false ) {
            farthest.push_back( index.i );
            index.i = save_i;
            // 3.宣告char int...等變數 
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

      if ( index.result == false ) { // 就算type錯給過 最後仍錯 那就與type是否對無關  
        index.undefined = -1;
        glob_type_wrong = -1;
      } // end if 
      else if ( index.error == 2 ) { // if waiting remaining input 
        index.undefined = -1;
      } // end else if 
      else if ( index.undefined != -1 ) { // String a, b, ... 全部del  
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

ReturnResult Type_specifier( ReturnResult index ) { // 除了void以外的型別  
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
    if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // 先當作是true  
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
      if ( glob_codes[token_i].token == "[" ) { // 可有可無 [ '[' Constant ']' ]  
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
            index = Rest_of_declarators( index ); // 繼續往下嘗試  
          }  // end else if 
          else { // 拋棄此可有可無 往下嘗試  
            index.i = save_i;
            index.stats = 2;
            index = Rest_of_declarators( index );
          } // end else 
        } // end else if 
        else { // 拋棄此可有可無 往下嘗試  
          index.i = save_i;
          index.stats = 2;
          index = Rest_of_declarators( index );
        } // end else 
      } // end if 
      else { // 拋棄此可有可無 往下嘗試   
        index.i = token_i;
        index.stats = 2;
        index = Rest_of_declarators( index );
      } // end else 
    } // end else 
  } // end if 
  else if ( index.stats == 2 ) { // 0到多次  
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

        if ( token_i == -1 ) { // 沒有更多了  
          index.i++;
          index.result = true;
          index.stats = 3;
          index = Rest_of_declarators( index ); // 繼續往下嘗試  
        } // end if
        else if ( glob_codes[token_i].token == "[" ) { // 可有可無 
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

              if ( token_i == -1 ) { // 沒有更多了 
                index.i++; 
                index.result = true;
                index.stats = 3;
                index = Rest_of_declarators( index ); // 繼續往下嘗試  
              } // end if
              else {
                index.result = true;
                index.stats = 2;
                index = Rest_of_declarators( index ); // 繼續0到多次 
              } // end else   
            } // end else if 
            else {
              index.result = true;
              index.i = save_i + 1; // 退回ident的下個token  
              index.stats = 3;
              index = Rest_of_declarators( index ); // 繼續往下嘗試  
            } // end else 
          } // end else if 
          else {
            index.result = true;
            index.i = save_i + 1; // 退回ident的下個token  
            index.stats = 3;
            index = Rest_of_declarators( index ); // 繼續往下嘗試  
          } // end else 
        } // end else if 
        else {
          index.result = true;
          index.i = token_i; // 退回ident的下一個token  
          index.stats = 2;
          index = Rest_of_declarators( index ); // 繼續0到多次   
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
      index = Rest_of_declarators( index ); // 繼續往下嘗試  
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
// 定義func,  function_definition_without_ID 
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
    else if ( glob_codes[token_i].token == "void" ) { // 可有可無 void  
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
    else { // 可有可無 formal_parameter_list  
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
        else if ( glob_codes[token_i].token == "&" ) { // 可有可無'&'  
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
            save_i = index.i; // 儲存ident位置  
            last_correct_i = index.i;

            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // 沒有更多了 
              index.result = true;
              index.stats = 2;
              index.i++;
              index = Formal_parameter_list( index, last_correct_i );
            } // end if 
            else if ( glob_codes[token_i].token == "[" ) { // 可有可無  
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
                  index = Formal_parameter_list( index, last_correct_i ); // 繼續往下嘗試 
                } // end else if 
                else { // 退回ident下一個token  
                  last_correct_i = save_i; // ident 
                  index.i = save_i + 1; // ident下一個  
                  index.result = true;
                  index.stats = 2;
                  index = Formal_parameter_list( index, last_correct_i ); // 繼續往下嘗試 
                } // end else 
              } // end else if 
              else { // 退回ident下一個token 
                last_correct_i = save_i; 
                index.i = save_i + 1;
                index.result = true;
                index.stats = 2;
                index = Formal_parameter_list( index, last_correct_i ); // 繼續往下嘗試 
              } // end else 
            } // end else if  
            else {
              index.result = true;
              index.stats = 2;
              index.i++; // ident下一個  
              index = Formal_parameter_list( index, last_correct_i ); // 繼續往下嘗試 
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
          save_i = index.i; // 儲存ident位置  
          last_correct_i = index.i;   

          token_i++;
          token_i = FindToken( token_i );

          if ( token_i == -1 ) { // 沒有更多了 
            index.result = true;
            index.stats = 2;
            index.i++;
            index = Formal_parameter_list( index, last_correct_i );
          } // end if 
          else if ( glob_codes[token_i].token == "[" ) { // 可有可無  
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
                index = Formal_parameter_list( index, last_correct_i ); // 繼續往下嘗試 
              } // end else if 
              else { // 退回ident下一個token 
                last_correct_i = save_i; // ident 
                index.i = save_i + 1; // ident下一個  
                index.result = true;
                index.stats = 2;
                index = Formal_parameter_list( index, last_correct_i ); // 繼續往下嘗試 
              } // end else 
            } // end else if 
            else { // 退回ident下一個token 
              last_correct_i = save_i;  
              index.i = save_i + 1;
              index.result = true;
              index.stats = 2;
              index = Formal_parameter_list( index, last_correct_i ); // 繼續往下嘗試 
            } // end else 
          } // end else if  
          else {
            index.result = true;
            index.stats = 2;
            index.i++;
            index = Formal_parameter_list( index, last_correct_i ); // 繼續往下嘗試 
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

        if ( index.result == false ) { // 就算type錯給過 最後仍錯 那就與type是否對無關  
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
    if ( token_i == -1 ) { // 沒有更多了  
      index.result = true;
    } // end if
    else {
      if ( glob_codes[token_i].token == "," ) { // 0到多次  
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
          index = Formal_parameter_list( index, last_correct_i ); // 繼續往下嘗試 

          if ( index.result == false ) {
            // index.i = last_i; // 退回0次 
            // index.result = true;
          } // end if 
          else if ( index.error == 2 ) { // 防止以為還需要input 
            index.error = 0;
          } // end else if 
        } // end else 
      } // end if 
      else { // 0次  
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
      save_i = index.i; // 儲存'{'位置  
      token_i++;
      token_i = FindToken( token_i );

      if ( token_i == -1 ) { // waiting for remaining input 
        index.error = 2;
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "}" ) { // 只有'{}'  
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
      save_i = index.i; // 儲存token位置  
      index = Statement( index ); // statement  

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        index.i++;
        index.stats = 2;
        index = Compound_statement( index, last_correct_i ); // compound_statement繼續嘗試    
      } // end if 
      else {
        farthest.push_back( index.i );
        index.i = save_i;
        index = Declaration( index );

        if ( index.result == true ) {
          last_correct_i = index.i;
          index.i++;
          index.stats = 2;
          index = Compound_statement( index, last_correct_i ); // compound_statement繼續嘗試   
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

ReturnResult Declaration( ReturnResult index ) {   // 宣告變數  
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

      if ( index.result == false ) { // 就算type錯給過 最後仍錯 那就與type是否對無關  
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
              else if ( glob_codes[token_i].token == "else" ) { // 可有可無else  
                index.i = token_i + 1;
                index = Statement( index ); // statement 

                if ( index.result == false ) { // 退回 if statement  
                  index.i = save_i;
                  index.result = true;  
                } // end if 
              } // end else if 
              else { // 沒有更多了  
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

      if ( index.result == true ) { // 可有可無 expression 
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
        index.i = save_i; // 在'return' token  
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
//  一開始last_i傳入啥都不重要, 此var遞迴傳資料用  
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
        index = Expression( index, last_correct_i ); // 再試一次
      } // end if 
    } // end else 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // 沒有更多了  
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
        index = Expression( index, last_correct_i ); // 再試一次
      } // end else 
    } // end else if
    else { // 0次  
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
        index.result = true; // 持續遞迴  
        // tmp.error = 4; // undefined
      } // end if
      else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // 尚未被定義  
        index.result = true; // 持續遞迴  
        if ( index.undefined == -1 && Is_CONST( glob_codes[token_i].type ) == false ) { // 紀錄第一個undefined  
          index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
        } // end if 
        // tmp.error = 4; // undefined
      } // end else if 
      else {
        if ( glob_codes[token_i].type == "unknown" ) {
          index.result = true;
          glob_codes[token_i].type = "IDENT"; // 使{int x;}為statement executed  
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
            index.result = true; // 持續遞迴  
          } // end if
          else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // 尚未被定義  
            index.result = true; // 持續遞迴  
            string type = glob_codes[token_i].type;
            if ( index.undefined == -1 && Is_CONST( type ) == false ) { // 紀錄第一個undefined  
              index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
            } // end if 
            // tmp.error = 4; // undefined
          } // end else if 
          else {
            if ( glob_codes[token_i].type == "unknown" ) {
              index.result = true;
              glob_codes[token_i].type = "IDENT"; // 使{int x;}為statement executed 
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
        index = Basic_expression( index ); // 繼續往下嘗試  
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
            index.result = true; // 持續遞迴  
          } // end if
          else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // 尚未被定義  
            index.result = true; // 持續遞迴  
            string type = glob_codes[token_i].type;
            if ( index.undefined == -1 && Is_CONST( type ) == false ) { // 紀錄第一個undefined  
              index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
            } // end if 
            // tmp.error = 4; // undefined
          } // end else if 
          else {
            if ( glob_codes[token_i].type == "unknown" ) {
              index.result = true;
              glob_codes[token_i].type = "IDENT"; // 使{int x;}為statement executed 
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
        index = Basic_expression( index ); // 繼續往下嘗試  
      } // end else 
    } // end else if
    else {
      index.stats = 2;
      index = Basic_expression( index ); // 繼續往下嘗試  
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
    if ( glob_codes[token_i].token == "[" ) { // 可有可無 
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
                index.i = save_i; // 退回']' 
                if ( glob_codes[token_i].token == "+" ) { // 可有可無 PP
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

                      if ( index.result == false ) { // 試剩下可能性  
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
                  else { // 試剩下可能性  
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
                else if ( glob_codes[token_i].token == "-" ) { // 可有可無 MM 
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

                      if ( index.result == false ) { // 試剩下的可能性 
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
                  else { // 試剩下可能性
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
                else { // 只有romce_and_romloe
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
          if ( glob_codes[token_i].token == ")" ) { // 只有'()' 
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
        index.i = save_i; // 退回  
        if ( glob_codes[token_i].token == "+" ) { // 可有可無 PP
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

              if ( index.result == false ) { // 試剩下的可能性 
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
          else { // 試剩下的可能性 
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
        else if ( glob_codes[token_i].token == "-" ) { // 可有可無 MM 
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

              if ( index.result == false ) { // 試剩下的可能性 
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
          else { // 試剩下的可能性 
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
        else { // 試剩下的可能性 
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
  else if ( glob_codes[token_i].token == "[" ) { // 可有可無 
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
//  一開始last_i傳入啥都不重要, 此var遞迴傳資料用  
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
        index = Actual_parameter_list( index, last_correct_i ); // 再試一次
      } // end if 
    } // end else 
  } // end if 
  else if ( index.stats == 2 ) {
    if ( token_i == -1 ) { // 沒有更多了  
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
        index = Actual_parameter_list( index, last_correct_i ); // 再試一次
      } // end else 
    } // end else if
    else { // 0次  
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

ReturnResult Unary_exp( ReturnResult index ) { // 分辨++ --是sign 還是 PP/MM 
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
              index.result = true; // 持續遞迴  
              string type = glob_codes[token_i].type;
              if ( index.undefined == -1 && Is_CONST( type ) == false ) { // 紀錄第一個undefined  
                index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
              } // end if 
              // tmp.error = 4; // undefined
            } // end if
            else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // 尚未被定義  
              index.result = true; // 持續遞迴  
              string type = glob_codes[token_i].type;
              if ( index.undefined == -1 && Is_CONST( type ) == false ) { // 紀錄第一個undefined  
                index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
              } // end if 
              // tmp.error = 4; // undefined
            } // end else if 
            else {
              index.result = true;

              if ( glob_codes[token_i].type == "unknown" ) {
                index.result = true;
                glob_codes[token_i].type = "IDENT"; // 使{int x;}為statement executed 
              } // end if 
            } // end else    

            int save_i = index.i; 
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // 沒有更多了  
              index.result = true;
            } // end if 
            else if ( glob_codes[token_i].token == "[" ) { // 可有可無 
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
                  else { // 退回( PP | MM )Identifier  
                    index.i = save_i;
                    index.result = true;
                  } // end else
                } // end if 
                else { // 退回( PP | MM )Identifier 
                  index.i = save_i; 
                  index.result = true;
                } // end else
              } // end else 
            } // end else if 
            else { // 只有IDENT  
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
              index.result = true; // 持續遞迴  
            } // end if
            else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // 尚未被定義  
              index.result = true; // 持續遞迴  
              string type;
              type = glob_codes[token_i].type;
              if ( index.undefined == -1 && Is_CONST( type ) == false ) { // 紀錄第一個undefined  
                index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
              } // end if 
              // tmp.error = 4; // undefined
            } // end else if 
            else {
              if ( glob_codes[token_i].type == "unknown" ) {
                index.result = true;
                glob_codes[token_i].type = "IDENT"; // 使{int x;}為statement executed 
              } // end if  
            } // end else    

            int save_i = index.i; 
            token_i++;
            token_i = FindToken( token_i );

            if ( token_i == -1 ) { // 沒有更多了  
              index.result = true;
            } // end if 
            else if ( glob_codes[token_i].token == "[" ) { // 可有可無 
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
                  else { // 退回( PP | MM )Identifier  
                    index.i = save_i;
                    index.result = true;
                  } // end else 
                } // end if 
                else { // 退回( PP | MM )Identifier 
                  index.i = save_i; 
                  index.result = true;
                } // end else
              } // end else 
            } // end else if 
            else { // 只有IDENT  
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
          // 0到多個sign  
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
// ( a+=b ) -> 括號裡更多expression 

  int token_i = FindToken( index.i );
  
  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func )
    index.i = token_i;
    int save_i = index.i;

    if ( Find_compnd_IDENT( glob_codes[token_i].token ) == true ) {
      index.result = true; // 持續遞迴  
    } // end if
    else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // 尚未被定義  
      index.result = true; // 持續遞迴  
      if ( index.undefined == -1 && Is_CONST( glob_codes[token_i].type ) == false ) { // 紀錄第一個undefined  
        index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
      } // end if 
      // tmp.error = 4; // undefined
    } // end else if 
    else {
      if ( glob_codes[token_i].type == "unknown" ) {
        index.result = true;
        glob_codes[token_i].type = "IDENT"; // 使{int x;}為statement executed 
      } // end if 
    } // end else    
    
    token_i++;
    token_i = FindToken( token_i );

    if ( token_i == -1 ) { // 沒有更多了 
      index.result = true;
    } // end if
    else if ( glob_codes[token_i].token == "(" ) { // 可有可無  
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
          else { // 退回Identifier  
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if
        else { // 退回Identifier
          index.i = save_i;
          index.result = true;
        } // end else 
      } // end else 
    } // end else if 
    else if ( glob_codes[token_i].token == "[" ) { // 可有可無  array[i][j] 
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
          else { // 退回Identifier
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if 
        else { // 退回Identifier 
          index.i = save_i;
          index.result = true;
        } // end else
      } // end else 
    } // end else if
    else { // 只有IDENT 
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
// ( a+=b ) -> 括號裡更多expression 

  int token_i = FindToken( index.i );
  
  if ( token_i == -1 ) { // waiting for remaining input 
    index.error = 2;
    index.result = true;
  } // end if 
  else if ( Is_IDENT( glob_codes[token_i].token, token_i ) == true ) { // is IDENT ( var or func )
    index.i = token_i;
    int save_i = index.i;

    if ( Find_compnd_IDENT( glob_codes[token_i].token ) == true ) {
      index.result = true; // 持續遞迴  
    } // end if
    else if ( Find_IDENT( glob_codes[token_i].token ) == false ) { // 尚未被定義  
      index.result = true; // 持續遞迴  
      if ( index.undefined == -1 && Is_CONST( glob_codes[token_i].type ) == false ) { // 紀錄第一個undefined  
        index.undefined = token_i; // // glob_codes[tmp.i].type = "unknown" 
      } // end if 
      // tmp.error = 4; // undefined
    } // end else if 
    else {
      if ( glob_codes[token_i].type == "unknown" ) {
        index.result = true;
        glob_codes[token_i].type = "IDENT"; // 使{int x;}為statement executed 
      } // end if 
    } // end else    
    
    token_i++;
    token_i = FindToken( token_i );

    if ( token_i == -1 ) { // 沒有更多了  
      index.result = true;
    } // end if
    else if ( glob_codes[token_i].token == "(" ) { // 可有可無  
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
          else { // 退回Identifier
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if
        else { // 退回Identifier 
          index.i = save_i;
          index.result = true;
        } // end else 
      } // end else 
    } // end else if 
    else if ( glob_codes[token_i].token == "[" ) { // 可有可無  array[i][j] 
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
            else if ( glob_codes[token_i].token == "+" ) { // 可有可無 PP 
              if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
                index.i = token_i;
                index.error = 2;
              } // end else if 
              else if ( glob_codes[token_i + 1].token == "+" ) {
                index.i = token_i + 1;
              } // end else if
              else { // 退回Identifier'[' expression ']'  

              } // end else 

              index.result = true;
            } // end else if
            else if ( glob_codes[token_i].token == "-" ) { // 可有可無 MM 
              if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
                index.i = token_i;
                index.error = 2;
              } // end else if 
              else if ( glob_codes[token_i + 1].token == "-" ) {
                index.i = token_i + 1;
              } // end else if 
              else { // 退回Identifier'[' expression ']'  

              } // end else 

              index.result = true;
            } // end else if
            else { // 只有Identifier'[' expression ']'  
              index.result = true;
            } // end else 
          } // end else if 
          else { // 退回Identifier 
            index.i = save_i;
            index.result = true;
          } // end else
        } // end if 
        else { // 退回Identifier  
          index.i = save_i;
          index.result = true;
        } // end else
      } // end else 
    } // end else if
    else if ( glob_codes[token_i].token == "+" ) { // 可有可無 PP 
      if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
        index.i = token_i;
        index.error = 2;
      } // end else if 
      else if ( glob_codes[token_i + 1].token == "+" ) {
        index.i = token_i + 1;
      } // end else if 
      else { // 退回Identifier  

      } // end else 

      index.result = true;
    } // end else if
    else if ( glob_codes[token_i].token == "-" ) { // 可有可無 PP 
      if ( token_i + 1 >= glob_codes.size() ) { // waiting for remaining input 
        index.i = token_i;
        index.error = 2;
      } // end else if 
      else if ( glob_codes[token_i + 1].token == "-" ) {
        index.i = token_i + 1;
      } // end else if 
      else { // 退回Identifier  

      } // end else 

      index.result = true;
    } // end else if
    else { // 只有IDENT 
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
      
      if ( token_i == -1 ) { // 沒有更多了 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "?" ) { // 可有可無 
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

                if ( index.result == false ) { // 退回romloe 
                  index.i = save_i;
                  index.result = true;
                } // end if 
              } // end else 
            } // end else if 
            else { // 退回romloe  
              index.i = save_i;
              index.result = true;
            } // end else 
          } // end if 
          else { // 退回romloe  
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
      
      if ( token_i == -1 ) { // 沒有更多次了 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "|" ) {
        index.i = token_i;

        if ( token_i + 1 < glob_codes.size() ) { // 嘗試'||'  
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
              index = Romloe( index, last_correct_i ); // 再嘗試一次 
            } // end else 
          } // end if 
          else { // 退回romlae 
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

        if ( token_i == -1 ) { // 沒有更多次了 
          index.result = true;
        } // end if
        else if ( glob_codes[token_i].token == "|" ) {
          index.i = token_i;

          if ( token_i + 1 < glob_codes.size() ) { // 嘗試'||'  
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
                index = Romloe( index, last_correct_i ); // 再嘗試一次 
              } // end else 
            } // end if 
            else { // 退回maybe_logical_AND_exp 
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
      else { // 退回  
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
      
      if ( token_i == -1 ) { // 沒有更多次了 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "&" ) {
        index.i = token_i;

        if ( token_i + 1 < glob_codes.size() ) { // 嘗試'&&'  
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
              index = Romlae( index, last_correct_i ); // 再嘗試一次 
            } // end else 
          } // end if 
          else { // 退回romboe 
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

        if ( token_i == -1 ) { // 沒有更多次了 
          index.result = true;
        } // end if
        else if ( glob_codes[token_i].token == "&" ) {
          index.i = token_i;

          if ( token_i + 1 < glob_codes.size() ) { // 嘗試'&&'  
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
                index = Romlae( index, last_correct_i ); // 再嘗試一次 
              } // end else 
            } // end if 
            else { // 退回maybe_bit_OR_exp 
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
      else { // 退回  
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
      
      if ( token_i == -1 ) { // 沒有更多次了 
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
          index = Romboe( index, last_correct_i ); // 再嘗試一次 
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

        if ( token_i == -1 ) { // 沒有更多次了 
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
            index = Romboe( index, last_correct_i ); // 再嘗試一次 
          } // end else
        } // end else if 
      } // end if 
      else { // 退回  
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
      
      if ( token_i == -1 ) { // 沒有更多次了 
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
          index = Rombeoe( index, last_correct_i ); // 再嘗試一次 
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

        if ( token_i == -1 ) { // 沒有更多次了 
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
            index = Rombeoe( index, last_correct_i ); // 再嘗試一次 
          } // end else
        } // end else if 
      } // end if 
      else { // 退回  
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
      
      if ( token_i == -1 ) { // 沒有更多次了 
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
          index = Rombae( index, last_correct_i ); // 再嘗試一次 
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

        if ( token_i == -1 ) { // 沒有更多次了 
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
            index = Rombae( index, last_correct_i ); // 再嘗試一次 
          } // end else
        } // end else if 
      } // end if 
      else { // 退回  
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
    save_i = index.i; // 紀錄romre位置  

    if ( index.result == true ) {
      last_correct_i = index.i;
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // 沒有更多次了 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "!" ) {
        index.i = token_i;
        if ( token_i + 1 < glob_codes.size() ) { // 嘗試'!='  
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
              index = Romee( index, last_correct_i ); // 再嘗試一次 
            } // end else 
          } // end if 
          else { // 退回rest_of_maybe_relational_exp 
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
        if ( token_i + 1 < glob_codes.size() ) { // 嘗試'=='  
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
              index = Romee( index, last_correct_i ); // 再嘗試一次 
            } // end else 
          } // end if 
          else { // 退回rest_of_maybe_relational_exp 
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if 
        else { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end else 
      } // end else if 
      else { // 只有rest_of_maybe_relational_exp 
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
      save_i2 = index.i; // 儲存位置  

      if ( index.result == true ) {
        last_correct_i = index.i;
        token_i = index.i + 1;
        token_i = FindToken( token_i );
      
        if ( token_i == -1 ) { // 沒有更多次了 
          index.result = true;
        } // end if
        else if ( glob_codes[token_i].token == "!" ) {
          index.i = token_i;
          if ( token_i + 1 < glob_codes.size() ) { // 嘗試'!='  
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
                index = Romee( index, last_correct_i ); // 再嘗試一次 
              } // end else 
            } // end if 
            else { // 退回maybe_relational_exp 
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
          if ( token_i + 1 < glob_codes.size() ) { // 嘗試'=='  
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
                index = Romee( index, last_correct_i ); // 再嘗試一次 
              } // end else 
            } // end if 
            else { // 退回maybe_relational_exp 
              index.i = save_i2;
              index.result = true;
            } // end else 
          } // end if 
          else { // waiting for remaining input 
            index.error = 2;
            index.result = true;
          } // end else 
        } // end else if 
        else { // 只有maybe_relational_exp  
          index.result = true; 
        } // end else 
      } // end if 
      else { // 退回  
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

      if ( token_i == -1 ) { // 沒有更多次了 
        index.result = true;
      } // end if 
      else {
        if ( glob_codes[token_i].token == "<" ) {
          index.i = token_i;
          if ( token_i + 1 < glob_codes.size() ) { // 嘗試'<=' 
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
            index = Romre( index, last_correct_i ); // 再嘗試一次 
          } // end else 
        } // end if
        else if ( glob_codes[token_i].token == ">" ) {
          index.i = token_i;
          if ( token_i + 1 < glob_codes.size() ) { // 嘗試'>=' 
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
            index = Romre( index, last_correct_i ); // 再嘗試一次 
          } // end else 
        } // end else if 
        else { // 只有rest_of_maybe_shift_exp 
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

        if ( token_i == -1 ) { // 沒有更多次了 
          index.result = true;
        } // end if 
        else {
          if ( glob_codes[token_i].token == "<" ) {
            index.i = token_i;
            if ( token_i + 1 < glob_codes.size() ) { // 嘗試'<=' 
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
              index = Romre( index, last_correct_i ); // 再嘗試一次 
            } // end else 
          } // end if
          else if ( glob_codes[token_i].token == ">" ) {
            index.i = token_i;
            if ( token_i + 1 < glob_codes.size() ) { // 嘗試'>=' 
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
              index = Romre( index, last_correct_i ); // 再嘗試一次 
            } // end else 
          } // end else if 
          else { // 只有rest_of_maybe_shift_exp 
            index.result = true;
          } // end else 
        } // end else 
      } // end if 
      else { // 退回  
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

      if ( token_i == -1 ) { // 只有romae  
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "<" ) { // 0到多次 
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
            index = Romse( index, last_correct_i ); // 繼續往下嘗試 
          } // end else 
        } // end else if 
        else { // 只有一個'<', 退回rest_of_maybe_additive_exp  
          index.result = true;
        } // end else 
      } // end else if 
      else if ( glob_codes[token_i].token == ">" ) { // 0到多次 
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
            index = Romse( index, last_correct_i ); // 繼續往下嘗試 
          } // end else 
        } // end else if 
        else { // 只有一個'>', 退回rest_of_maybe_additive_exp 
          index.result = true;
        } // end else 
      } // end else if 
      else { // 只有rest_of_maybe_additive_exp 
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

        if ( token_i == -1 ) { // 沒有更多次了 
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == "<" ) { // 0到多次 
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
              index = Romse( index, last_correct_i ); // 再嘗試一次 
            } // end else 
          } // end else if 
          else { // 只有一個'<', 退回maybe_additive_exp 
            index.result = true;
          } // end else 
        } // end else if 
        else if ( glob_codes[token_i].token == ">" ) { // 0到多次 
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
              index = Romse( index, last_correct_i ); // 再嘗試一次 
            } // end else 
          } // end else if 
          else { // 只有一個'>', 退回maybe_additive_exp 
            index.result = true;
          } // end else 
        } // end else if
        else { // 只有maybe_additive_exp 
          index.result = true;
        } // end else 
      } // end if 
      else { // 退回  
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

      if ( token_i == -1 ) { // 只有Romme 
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "+" || glob_codes[token_i].token == "-" ) { // 再嘗試一次  
        index.i = token_i + 1;
        index.stats = 2;
        index = Romae( index, last_correct_i );
      } // end else if 
      else { // 退回rest_of_maybe_mult_exp 
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

        if ( token_i == -1 ) { // 沒有更多次了 
          index.result = true;
        } // end if 
        else if ( glob_codes[token_i].token == "+" || glob_codes[token_i].token == "-" ) { // 再嘗試一次  
          index.i = token_i + 1;
          index.stats = 2;
          index = Romae( index, last_correct_i );
        } // end else if 
        else { // 退回maybe_mult_exp 
          index.result = true;
        } // end else 
      } // end if
      else { // 退回  
        index.i = last_i;
        index.result = true; 
      } // end else
    } // end else 
  } // end else if 

  return index;
} // Romae() 

ReturnResult Romme( ReturnResult index, int last_i ) { // rest_of_maybe_mult_exp, stats 1 2 
// 只有Unary_exp能提供false 
  int last_correct_i; // 前面rom function的last_correct_i用在此處  
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
    else { // 就算都錯, 那就當empty 
      index.i = last_i; // 退回上次正確的位置  
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

        if ( token_i == -1 ) { // 沒有更多次了  
          index.result = true;
        } // end if 
        else {
          last_correct_i = index.i;
          index.i = token_i;
          index.stats = 1;
          index = Romme( index, last_correct_i );
        } // end else 
      } // end if 
      else { // 退回  
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
    save_i = index.i; // 儲存位置  

    if ( index.result == true ) {
      token_i = index.i + 1;
      token_i = FindToken( token_i );
      
      if ( token_i == -1 ) { // 沒有更多了 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "&" ) {
        index.i = token_i;

        if ( token_i + 1 < glob_codes.size() ) { // 嘗試'&&'  
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
              index = M_lae( index ); // 再嘗試一次 

              if ( index.result == false ) {
                index.i = save_i; // 退回maybe_bit_OR_exp 
                index.result = true; 
              } // end if 
              else if ( index.error == 2 ) { // 防止以為還需要input  
                index.error = 0;
              } // end else if 
            } // end else 
          } // end if 
          else { // 退回maybe_bit_OR_exp 
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
      
      if ( token_i == -1 ) { // 沒有更多了 
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
          index = M_boe( index ); // 再嘗試一次 

          if ( index.result == false ) {
            index.i = save_i; // 退回maybe_bit_ex_OR_exp 
            index.result = true; 
          } // end if 
          else if ( index.error == 2 ) { // 防止以為還需要input 
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
      
      if ( token_i == -1 ) { // 沒有更多了 
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
          index = M_beoe( index ); // 再嘗試一次 

          if ( index.result == false ) {
            index.i = save_i; // 退回maybe_bit_AND_exp 
            index.result = true; 
          } // end if 
          else if ( index.error == 2 ) { // 防止以為還需要input 
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
      
      if ( token_i == -1 ) { // 沒有更多了 
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
          index = M_bae( index ); // 再嘗試一次 

          if ( index.result == false ) {
            index.i = save_i; // 退回maybe_equality_exp 
            index.result = true; 
          } // end if 
          else if ( index.error == 2 ) { // 防止以為還需要input 
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
      
      if ( token_i == -1 ) { // 沒有更多了 
        index.result = true;
      } // end if
      else if ( glob_codes[token_i].token == "!" ) {
        index.i = token_i;
        if ( token_i + 1 < glob_codes.size() ) { // 嘗試'!='  
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
              index = M_ee( index ); // 再嘗試一次 

              if ( index.result == false ) {
                index.i = save_i; // 退回maybe_relational_exp 
                index.result = true; 
              } // end if 
              else if ( index.error == 2 ) { // 防止以為還需要input 
                index.error = 0;
              } // end else if 
            } // end else 
          } // end if 
          else { // 退回maybe_relational_exp 
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
        if ( token_i + 1 < glob_codes.size() ) { // 嘗試'=='  
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
              index = M_ee( index ); // 再嘗試一次 

              if ( index.result == false ) {
                index.i = save_i; // 退回maybe_relational_exp 
                index.result = true; 
              } // end if
              else if ( index.error == 2 ) { // 防止以為還需要input 
                index.error = 0;
              } // end else if 
            } // end else 
          } // end if 
          else { // 退回maybe_relational_exp 
            index.i = save_i;
            index.result = true;
          } // end else 
        } // end if 
        else { // waiting for remaining input 
          index.error = 2;
          index.result = true;
        } // end else 
      } // end else if 
      else { // 只有maybe_relational_exp  
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

      if ( token_i == -1 ) { // 沒有更多了 
        index.result = true;
      } // end if 
      else {
        if ( glob_codes[token_i].token == "<" ) {
          index.i = token_i;
          if ( token_i + 1 < glob_codes.size() ) { // 嘗試'<=' 
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
            index = M_re( index ); // 再嘗試一次 

            if ( index.result == false ) {
              index.i = save_i; // 退回maybe_shift_exp 
              index.result = true; 
            } // end if 
            else if ( index.error == 2 ) { // 防止以為還需要input 
              index.error = 0;
            } // end else if 
          } // end else 
        } // end if
        else if ( glob_codes[token_i].token == ">" ) {
          index.i = token_i;
          if ( token_i + 1 < glob_codes.size() ) { // 嘗試'>=' 
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
            index = M_re( index ); // 再嘗試一次 

            if ( index.result == false ) {
              index.i = save_i; // 退回maybe_shift_exp 
              index.result = true; 
            } // end if 
            else if ( index.error == 2 ) { // 防止以為還需要input 
              index.error = 0;
            } // end else if 
          } // end else 
        } // end else if 
        else { // 只有maybe_shift_exp   
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

      if ( token_i == -1 ) { // 沒有更多了  
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "<" ) { // 0到多次 
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
            index = M_se( index ); // 再嘗試一次  

            if ( index.result == false ) {
              index.i = save_i; // maybe_additive_exp 
              index.result = true; 
            } // end if 
            else if ( index.error == 2 ) { // 防止以為還需要input 
              index.error = 0;
            } // end else if 
          } // end else 
        } // end else if 
        else { // 只有一個'<', 退回maybe_additive_exp 
          index.result = true;
        } // end else 
      } // end else if 
      else if ( glob_codes[token_i].token == ">" ) { // 0到多次 
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
            index = M_se( index ); // 再嘗試一次  

            if ( index.result == false ) {
              index.i = save_i; // maybe_additive_exp 
              index.result = true; 
            } // end if 
            else if ( index.error == 2 ) { // 防止以為還需要input 
              index.error = 0;
            } // end else if 
          } // end else  
        } // end else if 
        else { // 只有一個'>', 退回maybe_additive_exp 
          index.result = true;
        } // end else 
      } // end else if 
      else { // 只有maybe_additive_exp 
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

      if ( token_i == -1 ) { // 沒有更多了 
        index.result = true;
      } // end if 
      else if ( glob_codes[token_i].token == "+" || glob_codes[token_i].token == "-" ) { // 再嘗試一次  
        index.i = token_i + 1;
        index.stats = 1;
        index = M_ae( index );

        if ( index.result == false ) {
          index.i = save_i; // maybe_mult_exp 
          index.result = true; 
        } // end if 
        else if ( index.error == 2 ) { // 防止以為還需要input 
          index.error = 0;
        } // end else if 
      } // end else if 
      else { // 退回maybe_mult_exp 
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
        if ( token_buffer.size() == 0 ) { // token buffer內沒東西 加入whitespace 
          element.row = x_axis;
          element.column = y_axis;
          y_axis = y_axis + str.size();
          element.token = str;
          element.type = "whitespace";
          glob_codes.push_back( element );
        } // end if 
        else { // token buffer內有東西 
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

          element.row = x_axis; // 加入whitespace 
          element.column = y_axis;
          y_axis = y_axis + str.size();
          element.token = str;
          element.type = "whitespace";
          glob_codes.push_back( element );
        } // end else 

        token_buffer.clear(); // token buffer清空  
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
      else if ( Is_SP_Token( str ) == true ) { // 如果是特別的token 
        if ( cin.peek() != '\n' && cin.peek() != ' ' && cin.peek() != '\t' ) { // 偷看後面的char 
          ch = cin.get();
          stringstream ss;
          ss << ch;
          peek = ss.str(); // peek = ch  
          Lines element;
          if ( ( peek == str && peek == "\\" ) || ( peek == str && peek == "/" ) ) { 
          // line-comment, '/', '\\' 
            if ( token_buffer.size() == 0 ) { // token buffer內沒東西 直接加入  
              element.row = x_axis;
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end if 
            else { // token buffer內有東西 
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

              element.row = x_axis; // 直接加入 
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end else 

            token_buffer.clear(); // token buffer清空  
          } // end if 
          else if ( ( str == "\\" && peek == "\'" ) || ( str == "\\" && peek == "\"" ) ) {
          // line-comment, '\'', '\"' , 也就是要印出 " or '  
            if ( token_buffer.size() == 0 ) { // token buffer內沒東西 直接加入  
              element.row = x_axis;
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end if 
            else { // token buffer內有東西 
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

              element.row = x_axis; // 直接加入 
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end else 

            token_buffer.clear(); // token buffer清空 
          } // end else if 
          else if ( ( str == "\\" && peek == "n" ) || ( str == "\\" && peek == "t" ) ) {
          // tab與newline   
            if ( token_buffer.size() == 0 ) { // token buffer內沒東西 直接加入  
              element.row = x_axis;
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end if 
            else { // token buffer內有東西 
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

              element.row = x_axis; // 直接加入 
              element.column = y_axis;
              y_axis = y_axis + str.size() + peek.size();
              element.token = str + peek;
              element.type = "tokenSP";
              glob_codes.push_back( element );
            } // end else 

            token_buffer.clear(); // token buffer清空 
          } // end else if 
          else { // str 與 peek為兩個不同token處理 
            if ( Is_Token( peek ) == true ) { // peek為token  
              if ( token_buffer.size() == 0 ) { // token buffer內沒東西 直接加入  
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
              else { // token buffer內有東西  
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

                element.row = x_axis; // 直接加入
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

              token_buffer.clear(); // token buffer清空  
            } // end if 
            else { // peek不為token  
              if ( token_buffer.size() == 0 ) { // token buffer內沒東西 直接加入  
                element.row = x_axis;
                element.column = y_axis;
                y_axis = y_axis + str.size();
                element.token = str;
                element.type = "token";
                glob_codes.push_back( element );

                token_buffer = token_buffer + peek;
              } // end if 
              else { // token buffer內有東西  
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

                element.row = x_axis; // 直接加入
                element.column = y_axis;
                y_axis = y_axis + str.size();
                element.token = str;
                element.type = "token";
                glob_codes.push_back( element );

                token_buffer.clear(); // token buffer清空
                token_buffer = token_buffer + peek;
              } // end else 
            } // end else 
          } // end else 
        } // end if 
        else { // 後面沒有char了  表示只有單一個   
          Lines element;
          if ( token_buffer.size() == 0 ) { // token buffer內沒東西 直接加入  
            element.row = x_axis;
            element.column = y_axis;
            y_axis = y_axis + str.size();
            element.token = str;
            element.type = "token";
            glob_codes.push_back( element );
          } // end if 
          else { // token buffer內有東西 
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

            element.row = x_axis; // 直接加入  
            element.column = y_axis;
            y_axis = y_axis + str.size();
            element.token = str;
            element.type = "token";
            glob_codes.push_back( element );
          } // end else 

          token_buffer.clear(); // token buffer清空 
        } // end else 
      } // end else if 
      else if ( Is_Token( str ) == true ) { // 如果是普通的token 
        Lines element;
        if ( token_buffer.size() == 0 ) { // token buffer內沒東西 直接加入  
          element.row = x_axis;
          element.column = y_axis;
          y_axis = y_axis + str.size();
          element.token = str;
          element.type = "token";
          glob_codes.push_back( element );
        } // end if 
        else { // token buffer內有東西 
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

          element.row = x_axis; // 直接加入  
          element.column = y_axis;
          y_axis = y_axis + str.size();
          element.token = str;
          element.type = "token";
          glob_codes.push_back( element );
        } // end else 

        token_buffer.clear(); // token buffer清空 	
      } // end else if 
      else { // keep getting token
        token_buffer = token_buffer + str;
      } // end else 
    } // end if 
  } // end while 

  return 0;
} // main() 
