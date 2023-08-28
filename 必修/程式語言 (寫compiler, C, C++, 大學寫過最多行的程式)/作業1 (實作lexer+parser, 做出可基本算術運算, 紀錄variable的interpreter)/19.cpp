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
}; // 檢查文法時的return  

struct Defined{
  string variable; // 變數名稱  
  string data; // 內容  
  string type; // 型別 
};

struct CalStack{
  string data; // 內容  
  string type; // 型別  
};

vector<Lines> glob_codes; // 使用者輸入的code  
vector<Defined> glob_defined_list; // 已宣告的variable 
int glob_newline_flag = 0; // 處理input a\n ;\n   vs     a\n := 1;\n 

bool Is_IDENT( string str, int index ) ; // identifier  
string Not_IDENT( string token ) ; // not ident 
int Is_SIGN( string str ) ; // sign +, - 
string Is_NUM( string str ) ; // number  
bool Is_White_Space( string TMP ) ; // whitespace  
bool Token_list( string str ) ; // 判斷文法時 合法的token  
bool Is_Token( string str ) ; // 切token時 所需要辨識出的token  
bool Is_SP_Token( string str ) ; // special token, //, ', "  
int FindNextToken( int i ) ; // 跳過whitespace 找出下個token  
bool Find_IDENT( string str ) ; // 尋找有沒有這個ident  
Defined Get_IDENT_value( string str ) ; // get這個ident的值  
void Overwrite_IDENT( Defined element ) ; // 覆蓋這個ident的值  
void Examine_float() ; // 檢查是不是浮點數  
int PeekToken( int i ) ; // 偷看下一個token  
void ClearTakenToken( int j ) ; // 刪除已經拿過的token(已執行過的code)  
void Line_Comment() ; // if found line-comment, delete words after line-comment 
int Check_all_defined( int start, int end ) ; // 檢查是否所有ident都已經define過  
bool Is_zero( string str ) ;
int Check_divide_zero() ;
void Unrecognized_or_Unexpected( string str, int index ) ; // 檢查是兩種中哪一種錯誤  
CalStack Calculation( int index, vector<Lines>& formula ) ; // 計算整個算式  
CalStack Cal_substack( vector<CalStack>& substack ) ; // 計算小部分的算式(ex 括號 先乘除後加減)  
string Double_to_string( double d ) ; // 小數轉為string  
void Double_decimal_3( double d ) ; // printf(%1.3f) 
double Float_tolerance( double d ) ; // 0.0001 
int Syntax_check( int i ) ; // 檢查有無文法錯誤  
ReturnResult Command( ReturnResult tmp ) ;
ReturnResult Statement( ReturnResult tmp ) ;
ReturnResult BooleanExp( ReturnResult tmp ) ;
ReturnResult ArithExp( ReturnResult tmp ) ;
ReturnResult Term( ReturnResult tmp ) ;
ReturnResult Factor( ReturnResult tmp ) ;


bool Is_IDENT( string str, int index ) { // identifier 
  bool result = true;
  if ( ( ( int ) str[0] < 65 || ( int ) str[0] > 90 ) ) {  // 1st char not letters
    if ( ( ( int ) str[0] < 97 || ( int ) str[0] > 122 ) ) {
      result = false;
    } // end if
  } // end if

  for ( int i = 1; i < str.size() ; i++ ) {
    if ( ( ( int ) str[i] < 65 || ( int ) str[i] > 90 ) ) { // not letters
      if ( ( int ) str[i] < 97 || ( int ) str[i] > 122 ) {
        if ( ( ( int ) str[i] < 48 || ( int ) str[i] > 57 ) && ( int ) str[i] != 95 ) {
        // not digits and underline 
          result = false;
        } // end if 
      } // end if 
    } // end if 
  } // end for 

  if ( str == "quit" ) {
    result = true;
    for ( int i = 0; i < index ; i++ ) { // "QUIT" is first token or not
      if ( Is_White_Space( glob_codes[i].token ) == true ) { // 若此quit前面只有whitespace(此為第一個token) 
        result = false;
      } // end if 
      else {
        result = true;
        i = index; // break
      } // end else 
    } // end for 
  } // end if 

  return result;
} // Is_IDENT() 

string Not_IDENT( string token ) {
  string str;
  if ( ( ( int ) token[0] < 65 || ( int ) token[0] > 90 ) ) {  // 1st char not letters
    if ( ( ( int ) token[0] < 97 || ( int ) token[0] > 122 ) ) {
      str = token[0];
    } // end if
  } // end if

  for ( int i = 1; i < token.size() ; i++ ) {
    if ( ( ( int ) token[i] < 65 || ( int ) token[i] > 90 ) ) { // not letters
      if ( ( int ) token[i] < 97 || ( int ) token[i] > 122 ) {
        if ( ( ( int ) token[i] < 48 || ( int ) token[i] > 57 ) && ( int ) token[i] != 95 ) {
        // not digits and underline 
          str = token[i];
          i = token.size(); // break 
        } // end if 
      } // end if 
    } // end if 
  } // end for 

  if ( token.find( "quit" ) != -1 ) {
    str = "quit";
  } // end if 

  return str;
} // Not_IDENT() 

int Is_SIGN( string str ) { // sign +, - 
  int result = 0;
  if ( str == "+" ) {
    result = 1;
  } // end if 
  else if ( str == "-" ) {
    result = -1;
  } // end else if 

  return result;
} // Is_SIGN() 

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
    else if ( i == 0 && ( ( int ) str[i] == 45 || ( int ) str[i] == 43 ) ) {  // +, - sign 
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

bool Is_White_Space( string TMP ) { // whitespace 
  if ( TMP == " " || TMP == "\t" || TMP == "\n" ) {
    return true;
  } // end if 
  else {
    return false;
  } // end else 
} // Is_White_Space() 

bool Token_list( string str ) { // 判斷文法時 合法的token 
  if ( str == "+" || str == "-" || str == "*" || str == "(" || str == ")" || str == "\'" || str == "\"" ) {
    return true;
  } // end if 
  else if ( str == ";" || str == "." || str == "/" || str == "\\" || str == "=" || str == "<" ) {
    return true;
  } // end else if 
  else if ( str == ">" || str == ":=" || str == "<=" || str == ">=" || str == "<>" ) {
    return true;
  } // end else if 
  else {
    return false;
  } // end else 
} // Token_list() 

bool Is_Token( string str ) { // 切token時 所需要辨識出的token  
  if ( str == "+" || str == "-" || str == "*" || str == "(" || str == ")" || str == "\'" || str == "\"" ) {
    return true;
  } // end if 
  else if ( str == ";" || str == "." || str == "/" || str == "\\" || str == ":" || str == "=" ) {
    return true;
  } // end else if 
  else if ( str == "<" || str == ">" ) {
    return true;
  } // end else if 
  else {
    return false;
  } // end else 
} // Is_Token() 

bool Is_SP_Token( string str ) { // special token, line-comment, '/', '\\', '<>', '>=', '<=' or ':=' 
  if ( str == "/" || str == "\\" || str == ":" || str == "<" || str == ">" ) {
    return true;
  } // end if 
  else {
    return false;
  } // end else 
} // Is_SP_Token() 

int FindNextToken( int i ) { // 跳過whitespace 找出下個token  
  int while_break = 0;
  while ( i < glob_codes.size() && while_break == 0 ) {
    if ( Is_White_Space( glob_codes[i].token ) == false ) {
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
} // FindNextToken() 

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

Defined Get_IDENT_value( string str ) { // make sure that this IDENT is defined or not
  Defined element;
  for ( int i = 0; i < glob_defined_list.size() ; i++ ) {
    if ( str == glob_defined_list[i].variable ) {
      element.data = glob_defined_list[i].data;
      element.type = glob_defined_list[i].type;
      element.variable = glob_defined_list[i].variable;
      i = glob_defined_list.size(); // break 
    } // end if 
  } // end for 

  return element;
} // Get_IDENT_value() 

void Overwrite_IDENT( Defined element ) { // 覆蓋這個ident的值  
  for ( int i = 0; i < glob_defined_list.size() ; i++ ) {
    if ( element.variable == glob_defined_list[i].variable ) {
      glob_defined_list[i].data = element.data;
      glob_defined_list[i].type = element.type;
      i = glob_defined_list.size();  // break 
    } // end if 
  } // end for 
} // Overwrite_IDENT() 

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

int PeekToken( int i ) { // 偷看下一個token  
  i = FindNextToken( i );
  return i;
} // PeekToken() 

void ClearTakenToken( int j ) { // del the token that already gets 
  for ( int i = 0; i <= j ; i++ ) {
    glob_codes.erase( glob_codes.begin() );
  } // end for 
} // ClearTakenToken() 

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
        index = FindNextToken( index );
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

int Check_divide_zero() {
  int i = 0, flag = 0;
  bool error = 0;
  string last_token;
  if ( FindNextToken( 0 ) != -1 ) {
    last_token = glob_codes[FindNextToken( 0 )].token;
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
      else {
        if ( flag == 0 ) {
          flag = 1;
        } // end if  	
      } // end else 
    } // end if 
    else {
      if ( flag == 1 ) {
        if ( Is_zero( glob_codes[i].token ) == true ) {
          error = 1;
          i = glob_codes.size(); // break
        } // end if 
        else {
          flag = 0;
        } // end else 
      } // end if 
    } // end else 

    last_token = glob_codes[i].token;
    i++;
    i = FindNextToken( i );
  } // end while 

  return error;
} // Check_divide_zero() 

void Unrecognized_or_Unexpected( string str, int index ) { // 檢查是否所有ident都已經define過  
// Is_ident, Token_list , Is_num
  bool result_token; 
  result_token = Token_list( str );
  if ( result_token == true ) {
    cout << "Unexpected token : \'" << str << "\'" << endl; 
  } // end if 
  else if ( Is_NUM( str ) == "float" || Is_NUM( str ) == "integer" ) {
    cout << "Unexpected token : \'" << str << "\'" << endl;
  } // end else if
  else if ( Is_IDENT( str, index ) == true ) {
    cout << "Unexpected token : \'" << str << "\'" << endl;
  } // end else if
  else {
    cout << "Unrecognized token with first char : \'" << str[0] << "\'" << endl;
  } // end else 
} // Unrecognized_or_Unexpected() 

CalStack Calculation( int index, vector<Lines>& formula ) { // 計算整個算式  
  vector<CalStack> stack;
  vector<CalStack> substack;
  CalStack element;
  Lines lp_rp_element;
  vector<Lines> lp_rp_tmp; // 處理左右括號算式的暫存vector  
  int rp = -1, lp = -1; // 紀錄'('和')'位置  
  bool error = false;
  for ( int i = 0; i <= index ; i++ ) {
    if ( Is_White_Space( formula[i].token ) == false && formula[i].token != ";" ) { // 忽略';'  
      element.data = formula[i].token;
      element.type = formula[i].type;
      stack.push_back( element );
    } // end if 
  } // end for 

  for ( int i = 0; i < stack.size() ; i++ ) { // 先處理括號 優先計算  
    if ( stack[i].data == "(" ) { lp = i; } // end if,  找出最裡層左括號  

    if ( i == stack.size() - 1 && lp != -1 ) { // end if, 取出括號中的算式 (不含括號)  
      for ( int j = lp + 1; j < stack.size() ; j++ ) {
        if ( stack[j].data == ")" ) {
          rp = j; 
          j = stack.size(); // break j的for	
        } // end if 
        else {
          lp_rp_element.column = -1;
          lp_rp_element.row = -1;
          lp_rp_element.token = stack[j].data;
          lp_rp_element.type = stack[j].type;
          lp_rp_tmp.push_back( lp_rp_element );
        } // end else 
      } // end for 

      CalStack result;
      result = Calculation( lp_rp_tmp.size() - 1, lp_rp_tmp ); // 開始計算子stack  
      lp_rp_tmp.clear();
      stack[rp].data = result.data;
      stack[rp].type = result.type;
      for ( int j = lp; j < rp ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
        stack.erase( stack.begin() + lp );
      } // end for 

      rp = -1;
      lp = -1;
      i = -1; // 再檢查還有沒有括號  
    } // end if 
  } // end for 

  for ( int i = 0; i < stack.size() - 1 ; i++ ) { // 處理 *,/  
    if ( stack[i].data == "*" || stack[i].data == "/" ) {
      for ( int j = i - 1; j <= i + 1 ; j++ ) {
        CalStack element;
        element.data = stack[j].data;
        element.type = stack[j].type;
        substack.push_back( element );
      } // end for 

      CalStack result;
      result = Cal_substack( substack ); // 開始計算子stack  
      if ( result.data == "Error" ) {
        error = true; 
        element.data = "Error"; 
        element.type = "integer"; 
        i = stack.size(); // break
      } // end if 

      if ( error == false ) {
        substack.clear();
        stack[i+1].data = result.data;
        stack[i+1].type = result.type;
        for ( int j = i - 1; j < i + 1 ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
          stack.erase( stack.begin()+ i - 1 );
        } // end for 

        i = 0; // 再檢查還有沒有*,/ 	
      } // end if
    } // end if 
  } // end for 

  if ( error != true ) { // 要是有分母0, 後面皆不計算 直接輸出error  
    for ( int i = 0; i < stack.size() - 1 ; i++ ) { // 處理 +,-  
      if ( stack[i].data == "+" || stack[i].data == "-" ) {
        for ( int j = i - 1; j <= i + 1 ; j++ ) {
          CalStack element;
          element.data = stack[j].data;
          element.type = stack[j].type;
          substack.push_back( element );
        } // end for 

        CalStack result;
        result = Cal_substack( substack ); // 開始計算子stack  
        substack.clear();
        stack[i+1].data = result.data;
        stack[i+1].type = result.type;
        for ( int j = i - 1; j < i + 1 ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
          stack.erase( stack.begin() + i - 1 );
        } // end for 

        i = 0; // 再檢查還有沒有*,/ 
      } // end if 
    } // end for 

    for ( int i = 0; i < stack.size() - 1 ; i++ ) { // 處理 =  < > <= >= <>
      if ( stack[i].data == "=" || stack[i].data == "<" || stack[i].data == ">" || stack[i].data == "<=" ) {
        for ( int j = i - 1; j <= i + 1 ; j++ ) {
          CalStack element;
          element.data = stack[j].data;
          element.type = stack[j].type;
          substack.push_back( element );
        } // end for 

        CalStack result;
        result = Cal_substack( substack ); // 開始計算子stack  
        substack.clear();
        stack[i+1].data = result.data;
        stack[i+1].type = result.type;
        for ( int j = i - 1; j < i + 1 ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
          stack.erase( stack.begin() + i - 1 );
        } // end for 

        i = 0; // 再檢查還有沒有*,/ 
      } // end if 
      else if ( stack[i].data == "<=" || stack[i].data == ">=" || stack[i].data == "<>" ) {
        for ( int j = i - 1; j <= i + 1 ; j++ ) {
          CalStack element;
          element.data = stack[j].data;
          element.type = stack[j].type;
          substack.push_back( element );
        } // end for 

        CalStack result;
        result = Cal_substack( substack ); // 開始計算子stack  
        substack.clear();
        stack[i+1].data = result.data;
        stack[i+1].type = result.type;
        for ( int j = i - 1; j < i + 1 ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
          stack.erase( stack.begin() + i - 1 );
        } // end for 

        i = 0; // 再檢查還有沒有*,/ 
      } // end else if 
    } // end for 

    if ( stack.size() > 1 && stack[1].data == ":=" ) { // overwrite變數表值  
      if ( Find_IDENT( stack[2].data ) == true ) {
        Defined ident;
        ident = Get_IDENT_value( stack[2].data );
        stack[2].data = ident.data;
        stack[2].type = ident.type;
      } // end if 

      if ( Find_IDENT( stack[0].data ) == true ) { // 查表
        Defined ident;
        element.type = stack[2].type;
        if ( element.type == "float" ) { // 若變數為小數  
          ident.data = stack[2].data;

          double d;
          d = atof( stack[2].data.c_str() );
          element.data = Double_to_string( d );
        } // end if 
        else {
          ident.data = stack[2].data;
          element.data = stack[2].data;
        } // end else 

        ident.variable = stack[0].data;
        ident.type = stack[2].type;
        Overwrite_IDENT( ident ); // overwrite 
      } // end if 
    } // end if 
    else if ( Find_IDENT( stack[0].data ) == true ) { // abc; 
      Defined ident;
      ident = Get_IDENT_value( stack[0].data );
      element.type = ident.type;
      if ( element.type == "float" ) {
        double d;
        d = atof( ident.data.c_str() );
        element.data = Double_to_string( d );
      } // end if 
      else {
        element.data = ident.data;
      } // end else 
    } // end else if 
    else {
      element.type = stack[0].type;
      if ( element.type == "float" ) {
        double d;
        d = atof( stack[0].data.c_str() );
        element.data = Double_to_string( d );
      } // end if 
      else {
        element.data = stack[0].data;
      } // end else 
    } // end else 
  } // end if 

  return element;
} // Calculation() 

CalStack Cal_substack( vector<CalStack>& substack ) { // 計算小部分的算式(ex 括號 先乘除後加減)  
  int index;
  string type = "none";
  CalStack cs;
  bool error = false;

  for ( int i = 0; i < substack.size() ; i++ ) {
    if ( substack[i].data == "=" || substack[i].data == "<" || substack[i].data == ">" ) {
      index = i;
      type = "boolean";
      i = substack.size(); // break 
    } // end if 
    else if ( substack[i].data == "<=" || substack[i].data == ">=" || substack[i].data == "<>" ) {
      index = i;
      type = "boolean";
      i = substack.size(); // break 
    } // end else if 
  } // end for 

  for ( int i = 0; i < substack.size() && type == "none" ; i++ ) {
    if ( Find_IDENT( substack[i].data ) == true ) { // 查表
      Defined ident;
      ident = Get_IDENT_value( substack[i].data ); // Get_IDENT_value
      if ( ident.type == "float" ) {
        type = "float";
        i = substack.size(); // break 
      } // end if 
    } // end if 
    else if ( substack[i].type == "float" ) {
      type = "float";
      i = substack.size(); // break 
    } // end else if 
  } // end for 

  if ( type == "none" ) { type = "integer"; } // end if 

  for ( int i = substack.size() - 2; i >= 0 ; i-- ) {
    if ( type == "float" && substack.size() > 2 ) {
      double a, b, result;
      bool float_error = false;
      if ( substack[i+1].type == "integer" || "float" ) { // B
        if ( Find_IDENT( substack[i+1].data ) == true ) { // 查表
          Defined ident;
          ident = Get_IDENT_value( substack[i+1].data ); // Get_IDENT_value
          b = atof( ident.data.c_str() );
        } // end if 
        else {
          b = atof( substack[i+1].data.c_str() );
        } // end else 
      } // end if 
      else if ( substack[i+1].type == "IDENT" ) {
        if ( Find_IDENT( substack[i+1].data ) == true ) { // 查表
          Defined ident;
          ident = Get_IDENT_value( substack[i+1].data ); // Get_IDENT_value
          b = atof( ident.data.c_str() );
        } // end if 
      } // end else if 

      if ( substack[i-1].type == "integer" || "float" ) { // A
        if ( Find_IDENT( substack[i-1].data ) == true ) { // 查表
          Defined ident;
          ident = Get_IDENT_value( substack[i-1].data ); // Get_IDENT_value
          a = atof( ident.data.c_str() );
        } // end if 
        else {
          a = atof( substack[i-1].data.c_str() );
        } // end else 
      } // end if 
      else if ( substack[i-1].type == "IDENT" ) {
        if ( Find_IDENT( substack[i-1].data ) == true ) { // 查表
          Defined ident;
          ident = Get_IDENT_value( substack[i-1].data ); // Get_IDENT_value
          a = atof( ident.data.c_str() );
        } // end if 
      } // end else if 

      if ( substack[i].data == "+" ) {
        result = a + b;
        substack[i+1].data = Double_to_string( result );
        substack[i+1].type = "float";
      } // end if 
      else if ( substack[i].data == "-" ) {
        result = a - b;
        substack[i+1].data = Double_to_string( result );
        substack[i+1].type = "float";
      } // end else if 
      else if ( substack[i].data == "*" ) {
        result = a * b;
        substack[i+1].data = Double_to_string( result );
        substack[i+1].type = "float";
      } // end else if 
      else if ( substack[i].data == "/" ) {
        if ( b == 0.0 ) {
          cs.data = "Error";
          cs.type = "float";
          error = true;
          float_error = true;
          i = -1; // break 
        } // end if 
        else {
          result = a / b;
          substack[i+1].data = Double_to_string( result );
          substack[i+1].type = "float";
        } // end else
      } // end else if 

      if ( float_error == false ) {
        for ( int j = 0; j < 2 ; j++ ) {
          substack.erase( substack.begin() + i - 1 );
        } // end for 

        i += 2;
      } // end if 
    } // end if 
    else if ( type == "integer" && substack.size() > 2 ) { // type=="integer"
      int a, b, result;
      stringstream int_to_string;
      bool integer_error = false;
      if ( substack[i+1].type == "integer" ) { // B
        if ( Find_IDENT( substack[i+1].data ) == true ) { // 查表
          Defined ident;
          ident = Get_IDENT_value( substack[i+1].data ); // Get_IDENT_value
          b = atoi( ident.data.c_str() );
        } // end if 
        else {
          b = atoi( substack[i+1].data.c_str() );
        } // end else 
      } // end if 
      else if ( substack[i+1].type == "IDENT" ) {
        if ( Find_IDENT( substack[i+1].data ) == true ) { // 查表
          Defined ident;
          ident = Get_IDENT_value( substack[i+1].data ); // Get_IDENT_value
          b = atoi( ident.data.c_str() );
        } // end if 
      } // end else if 

      if ( substack[i-1].type == "integer" ) { // A
        if ( Find_IDENT( substack[i-1].data ) == true ) { // 查表
          Defined ident;
          ident = Get_IDENT_value( substack[i-1].data ); // Get_IDENT_value
          a = atoi( ident.data.c_str() );
        } // end if 
        else {
          a = atoi( substack[i-1].data.c_str() );
        } // end else 
      } // end if 
      else if ( substack[i-1].type == "IDENT" ) {
        if ( Find_IDENT( substack[i-1].data ) == true ) { // 查表
          Defined ident;
          ident = Get_IDENT_value( substack[i-1].data ); // Get_IDENT_value
          a = atoi( ident.data.c_str() );
        } // end if  
      } // end else if 

      if ( substack[i].data == "+" ) {
        result = a + b;
        int_to_string << result;
        string str = int_to_string.str();
        substack[i+1].data = str;
        substack[i+1].type = "integer";
      } // end if 
      else if ( substack[i].data == "-" ) {
        result = a - b;
        int_to_string << result;
        string str = int_to_string.str();
        substack[i+1].data = str;
        substack[i+1].type = "integer";
      } // end else if 
      else if ( substack[i].data == "*" ) {
        result = a * b;
        int_to_string << result;
        string str = int_to_string.str();
        substack[i+1].data = str;
        substack[i+1].type = "integer";
      } // end else if 
      else if ( substack[i].data == "/" ) {
        if ( b == 0 ) {
          cs.data = "Error";
          cs.type = "integer";
          error = true;
          integer_error = true;
          i = -1; // break 
        } // end if 
        else {
          result = a / b;
          if ( a != 0 && result == 0 ) {
            double d_result, d_A, d_B;
            d_A = a * 1.0;
            d_B = b * 1.0;
            d_result = d_A / d_B;
            stringstream double_to_string;
            double_to_string << d_result;
            string str = double_to_string.str();
            substack[i+1].data = str;
            substack[i+1].type = "float";
          } // end if 
          else {
            int_to_string << result;
            string str = int_to_string.str();
            substack[i+1].data = str;
            substack[i+1].type = "integer";
          } // end else 
        } // end else 
      } // end else if 

      if ( integer_error == false ) {
        for ( int j = 0; j < 2 ; j++ ) {
          substack.erase( substack.begin() + i - 1 );
        } // end for 

        i += 2;
      } // end if 
    } // end else if 
    else if ( type == "boolean" && substack.size() > 2 ) { // type=="boolean"
      double float_a, float_b;
      if ( substack[index-1].data == "Error" || substack[index+1].data == "Error" ) {
        error = true;
      } // end if 

      if ( error == true ) {
        cs.data = "Error";
        cs.type = "boolean";
      } // end if 
      else {
        if ( Find_IDENT( substack[index-1].data ) == true ) {
          Defined ident;
          ident = Get_IDENT_value( substack[index-1].data );
          float_a = atof( ident.data.c_str() );
        } // end if 
        else {
          float_a = atof( substack[index-1].data.c_str() );
        } // end else 

        if ( Find_IDENT( substack[index+1].data ) == true ) {
          Defined ident;
          ident = Get_IDENT_value( substack[index+1].data );
          float_b = atof( ident.data.c_str() );
        } // end if 
        else {
          float_b = atof( substack[index+1].data.c_str() );
        } // end else 

        float_a = Float_tolerance( float_a );
        float_b = Float_tolerance( float_b );
        if ( substack[index].data == "=" ) {
          if ( float_a == float_b ) {
            cs.data = "true";
            cs.type = "boolean";
          } // end if 
          else {
            cs.data = "false";
            cs.type = "boolean";
          } // end else 
        } // end if
        else if ( substack[index].data == "<" ) {
          if ( float_a < float_b ) {
            cs.data = "true";
            cs.type = "boolean";
          } // end if 
          else {
            cs.data = "false";
            cs.type = "boolean";
          } // end else 
        } // end else if 
        else if ( substack[index].data == ">" ) {
          if ( float_a > float_b ) {
            cs.data = "true";
            cs.type = "boolean";
          } // end if 
          else {
            cs.data = "false";
            cs.type = "boolean";
          } // end else 
        } // end else if 
        else if ( substack[index].data == "<=" ) {
          if ( float_a <= float_b ) {
            cs.data = "true";
            cs.type = "boolean";
          } // end if 
          else {
            cs.data = "false";
            cs.type = "boolean";
          } // end else 
        } // end if 
        else if ( substack[index].data == ">=" ) {
          if ( float_a >= float_b ) {
            cs.data = "true";
            cs.type = "boolean";
          } // end if 
          else {
            cs.data = "false";
            cs.type = "boolean";
          } // end else
        } // end if 
        else if ( substack[index].data == "<>" ) {
          if ( float_a != float_b ) {
            cs.data = "true";
            cs.type = "boolean";
          } // end if 
          else {
            cs.data = "false";
            cs.type = "boolean";
          } // end else 
        } // end else if 

        i = -1; // 跳出for
      } // end else 
    } // end else if 
  } // end for

  if ( error != true ) {
    if ( type == "float" ) {
      cs.data = substack[0].data;
      cs.type = "float";
    } // end if 
    else if ( type == "integer" ) {
      cs.data = substack[0].data;
      cs.type = "integer";
    } // end else if 
    else { // boolean 
    
    } // end else 
  } // end if 

  return cs;
} // Cal_substack() 

string Double_to_string( double d ) { // 小數轉為string  
  int index;
  string str, ch;
  stringstream double_to_string;
  double_to_string << d;
  str = double_to_string.str();

  return str;
} // Double_to_string() 

void Double_decimal_3( double d ) { // printf(%1.3f) 
  printf( "%1.3f\n", d );
} // Double_decimal_3() 

double Float_tolerance( double d ) { // 0.0001 
  if ( d < 0.0001 ) {
    return 0.0001;
  } // end if 
  else {
    return d;
  } // end else 
} // Float_tolerance() 

int Syntax_check( int i ) { // 檢查有無文法錯誤  
  // PrintCodes();
  Examine_float();

  int defined_or_not, brk = 0, index;
  bool dvd_0 = false;
  ReturnResult tmp;
  CalStack result;
  tmp.i = i;
  tmp.stats = 1;
  tmp.result = true;

  while ( tmp.i != -1 && tmp.result == true && glob_codes.size() > 0 && brk == 0 && dvd_0 == false ) {
    tmp.result = false;
    tmp = Command( tmp );

    dvd_0 = Check_divide_zero();
    if ( dvd_0 == 0 ) {
      defined_or_not = Check_all_defined( 0, tmp.i );
    } // end if 
    else if ( dvd_0 == 1 ) {
      defined_or_not = Check_all_defined( 0, glob_codes.size() - 1 );
    } // end else if 

    if ( dvd_0 == -1 ) {
      cout << "Undefined identifier : \'" << glob_codes[defined_or_not].token << "\'" << endl;
      ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
    } // end if 
    else if ( dvd_0 == 1 ) {
      ClearTakenToken( glob_codes.size() - 1 ); // 清除整行
    } // end if  
    else if ( tmp.result == false ) { // 第一道關卡and第二道關卡 
      Unrecognized_or_Unexpected( glob_codes[tmp.i].token, tmp.i );
      // cout << "Syntax Wrong." << endl; 
      ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
      glob_newline_flag = 0; // 歸零   
    } // end else if 
    else if ( defined_or_not != -1 && tmp.result == true ) { // 第三道關卡  
      if ( glob_newline_flag == 1 ) {
        cout << "Undefined identifier : \'" << glob_codes[defined_or_not].token << "\'" << endl;
        ClearTakenToken( defined_or_not ); // 清除至定義token 
        glob_newline_flag = 0; // 歸零 
        tmp.i = i; // 歸零
        tmp.stats = 1; // 歸零 
        tmp.result = true; // 歸零 
      } // end if 
      else {
        cout << "Undefined identifier : \'" << glob_codes[defined_or_not].token << "\'" << endl;
        ClearTakenToken( glob_codes.size() - 1 ); // 清除整行 
      } // end else 

      cout << "> "; 
      // PrintCodes();
    } // end else if 
    else if ( tmp.i == -1 ) {
      glob_newline_flag = 1; // 處理input a\n ;\n   vs     a\n := 1;\n
      index = FindNextToken( 0 );
      if ( index != -1 && glob_codes[index].token == "quit" ) {
        tmp.i = index;
      } // end if 

      brk = 1;
    } // end else if 
    else if ( tmp.i != -1 && glob_codes[tmp.i].token == "quit" ) {
      brk = 1; // break 
    } // end if
    else if ( tmp.i != -1 && tmp.result == true && glob_codes.size() > 1 ) { 
      result = Calculation( tmp.i, glob_codes ); // 開始計算  
      if ( result.type == "float" ) {
        double d;
        d = atof( result.data.c_str() );
        Double_decimal_3( d );
      } // end if 
      else {
        cout << result.data << endl;
      } // end else
 
      cout << "> ";
      ClearTakenToken( tmp.i ); // 清除已拿過 (不缺任何待下次輸入補上的資料 確定文法已完整解析 ) 
      tmp.i = i; // 從index 0 重新開始分析下一批文法  
      tmp.stats = 1;
      glob_newline_flag = 0; // 歸零 
    } // end else if 

    if ( FindNextToken( 0 ) == -1 ) { // 沒有可token 剩下whitespace  
      ClearTakenToken( glob_codes.size() - 1 ); // 清除整行	
    } // end if 
  } // end while 

  if ( dvd_0 == 1 ) {
    cout << "error" << endl;
    cout << "> ";
    return false;
  } // end if 
  else if ( tmp.i == -1 ) { // 待下次使用者輸入補上資料  
    // cout << "Waiting for remaining inputs." << endl; 
    // PrintCodes();
    // cout << "> ";
    return false;
  } // end else if 
  else if ( tmp.result == false ) { // 第一道關卡and第二道關卡 
    // PrintCodes();
    cout << "> ";
    return false;
  } // end else if 
  else if ( tmp.i != -1 && glob_codes[tmp.i].token == "quit" ) {
    cout << "Program exits..." << endl; 
    return true;
  } // end if 
  else if ( defined_or_not != -1 && tmp.result == true ) { // 第三道關卡  
    // cout << "> ";
    return false;
  } // end else if 
  else { // correct
    // cout << "Syntax Correct." << endl; 
    // ClearTakenToken( tmp.i ); // 清除整行 
    // PrintCodes();
    return false;
  } // end else 
} // Syntax_check() 

ReturnResult Command( ReturnResult tmp ) {
  tmp.i = FindNextToken( tmp.i );
  int save_i = tmp.i;
  vector<int> index;

  if ( tmp.i == -1 ) {
    tmp.result = true;
    index.clear();
    return tmp;
  } // end if  
  else if ( tmp.stats == 1 ) {
    tmp.stats = 1;
    tmp = Statement( tmp ); // i, stats=1
    if ( tmp.result == true && tmp.i != -1 ) {
      tmp.stats = 2;
      tmp.i++;
      tmp = Command( tmp );
      index.clear();
      return tmp;
    } // end if 
    else if ( tmp.i == -1 ) { // 待使用者下次input補齊  
      index.clear();
      return tmp;
    } // end else if 
    else {
      index.push_back( tmp.i ); // 紀錄Statement最遠走到哪  
      tmp.i = save_i;
      tmp.stats = 1;
      tmp = BooleanExp( tmp );
      if ( tmp.result == true && tmp.i != -1 ) {
        tmp.stats = 2;
        tmp.i++;
        tmp = Command( tmp );
        index.clear();
        return tmp;
      } // end if 
      else if ( tmp.i == -1 ) { // 待使用者下次input補齊  
        index.clear();
        return tmp;
      } // else if 
      else {
        index.push_back( tmp.i ); // 紀錄BooleanExp最遠走到哪  
        tmp.i = save_i;
        tmp.stats = 1;
        tmp = ArithExp( tmp );
        if ( tmp.result == true && tmp.i != -1 ) {
          tmp.stats = 2;
          tmp.i++; // 可能是這行 -1 -> 0
          tmp = Command( tmp );
          if ( tmp.result == false ) {
            index.push_back( tmp.i ); // 紀錄ArithExp最遠走到哪 
            tmp.i = save_i;
            tmp.stats = 1;
            if ( glob_codes[tmp.i].token == "quit" ) {
              tmp.result = true;
              index.clear();
              return tmp;
            } // end if 
            else {
              int max_index = -1;
              for ( int i = 0; i < index.size() ; i++ ) {
                if ( index[i] > max_index ) { max_index = index[i]; }
              } // end for 

              tmp.i = max_index;
              tmp.result = false;
              index.clear();
              return tmp;
            } // end else 
          } // end if 
          else {
            index.clear();
            return tmp;
          } // end else 
        } // end if 
        else if ( tmp.i == -1 ) { // 待使用者下次input補齊  
          index.clear();
          return tmp;
        } // end else if 
        else {
          index.push_back( tmp.i ); // 紀錄ArithExp最遠走到哪  
          tmp.i = save_i;
          tmp.stats = 1;
          if ( glob_codes[tmp.i].token == "quit" ) {
            tmp.result = true;
            index.clear();
            return tmp;
          } // end if 
          else {
            int max_index = -1;
            for ( int i = 0; i < index.size() ; i++ ) {
              if ( index[i] > max_index ) { max_index = index[i]; }
            } // end for 

            tmp.i = max_index;
            tmp.result = false;
            index.clear();
            return tmp;
          } // end else 
        } // end else 
      } // end else 
    } // end else 
  } // end else if 
  else { // stats=2
    if ( glob_codes[tmp.i].token == ";" ) {
      tmp.result = true;
      index.clear();
      return tmp;
    } // end if 
    else {
      tmp.result = false;
      index.clear();
      return tmp;
    } // end else 
  } // end else 
} // Command() 

ReturnResult Statement( ReturnResult tmp ) {
  tmp.i = FindNextToken( tmp.i );

  if ( tmp.i == -1 ) {
    tmp.result = true;
    return tmp;
  } // end if 
  else if ( Is_IDENT( glob_codes[tmp.i].token, tmp.i ) == true ) {
    string tmp_IDENT = glob_codes[tmp.i].token;
    int tmp_index = tmp.i;
    tmp.i++;
    tmp.i = FindNextToken( tmp.i );
    if ( tmp.i != -1 && glob_codes[tmp.i].token == ":=" ) {
      tmp.i++;
      tmp.stats = 1;
      tmp = ArithExp( tmp );
      if ( tmp.result == true ) {  // add IDENT to defined-list
        Defined new_ident;
        if ( Find_IDENT( tmp_IDENT ) == false ) {
          glob_codes[tmp_index].type = "IDENT";

          Defined element;
          element.variable = tmp_IDENT;
          element.data = "unknown";
          element.type = "unknown";
          glob_defined_list.push_back( element );
        } // end if 
      } // end if
 
      return tmp;
    } // end if
    else if ( tmp.i == -1 ) {
      tmp.result = true;
      return tmp;
    } // end else if 
    else {
      tmp.result = false;
      return tmp;
    } // end else 
  } // end else if 
  else { 
    tmp.result = false;
    return tmp;
  } // end else 
} // Statement() 

ReturnResult BooleanExp( ReturnResult tmp ) {
  tmp.i = FindNextToken( tmp.i );

  if ( tmp.i == -1 ) {
    tmp.result = true;
    return tmp;
  } // end if 
  else {
    tmp.stats = 1;
    tmp = ArithExp( tmp );
    if ( tmp.result == true ) {
      if ( tmp.i != -1 ) {
        tmp.i++;
        tmp.i = FindNextToken( tmp.i );
        if ( tmp.i != -1 && ( glob_codes[tmp.i].token == "=" || glob_codes[tmp.i].token == "<>" ) ) {
          tmp.stats = 1;
          tmp.i++;
          tmp = ArithExp( tmp );
          return tmp;
        } // end if 
        else if ( tmp.i != -1 && ( glob_codes[tmp.i].token == ">" || glob_codes[tmp.i].token == "<" ) ) {
          tmp.stats = 1;
          tmp.i++;
          tmp = ArithExp( tmp );
          return tmp;
        } // end else if 
        else if ( tmp.i != -1 && ( glob_codes[tmp.i].token == ">=" || glob_codes[tmp.i].token == "<=" ) ) {
          tmp.stats = 1;
          tmp.i++;
          tmp = ArithExp( tmp );
          return tmp;
        } // end else if 
        else if ( tmp.i == -1 ) {
          tmp.result = true;
          return tmp;
        } // end else if 
        else {
          tmp.result = false;
          return tmp;
        } // end else
      } // end if 
      else {
        tmp.result = true;
        return tmp;
      } // end else 
    } // end if 
    else {
      tmp.result = false;
      return tmp;
    } // end else 
  } // end else 
} // BooleanExp() 

ReturnResult ArithExp( ReturnResult tmp ) {
  tmp.i = FindNextToken( tmp.i );
  int peek;

  if ( tmp.i == -1 ) {
    tmp.result = true;
    return tmp;
  } // end if 
  else if ( tmp.stats == 1 ) {
    tmp.stats = 1;
    tmp = Term( tmp );
    peek = tmp.i + 1;
    peek = PeekToken( peek );
    if ( tmp.result == true ) {
      if ( peek != -1 && ( glob_codes[peek].token == "+" || glob_codes[peek].token == "-" ) ) {
        tmp.i++;
        tmp.stats = 2;
        tmp = ArithExp( tmp );
        return tmp;
      } // end if 
      else if ( peek == -1 ) {
        tmp.result = true;
        return tmp;
      } // end else if 
      else {
        return tmp;
      } // end else 
    } // end if 
    else {
      return tmp;
    } // end else 
  } // end else if 
  else if ( tmp.stats == 2 ) {
    if ( glob_codes[tmp.i].token == "+" || glob_codes[tmp.i].token == "-" ) {
      tmp.i++;
      tmp.stats = 3;
      tmp = ArithExp( tmp );
      return tmp;
    } // end if 
    else {
      tmp.result = false;
      return tmp;
    } // end else 
  } // end else if 
  else if ( tmp.stats == 3 ) {
    tmp.stats = 1;
    tmp = ArithExp( tmp ); // i, stats=1
    return tmp;
  } // end if 
  else {
    tmp.result = false;
    return tmp;
  } // end else 
} // ArithExp() 

ReturnResult Term( ReturnResult tmp ) {
  tmp.i = FindNextToken( tmp.i );
  int peek;

  if ( tmp.i == -1 ) {
    tmp.result = true;
    return tmp;
  } // end if 
  else if ( tmp.stats == 1 ) {
    tmp.stats = 1;
    tmp = Factor( tmp );
    if ( tmp.result == true ) {
      peek = tmp.i + 1;
      peek = PeekToken( peek );

      if ( peek != -1 && ( glob_codes[peek].token == "*" || glob_codes[peek].token == "/" ) ) {
        tmp.i++;
        tmp.stats = 2;
        tmp = Term( tmp );
        return tmp;
      } // end if 
      else if ( peek == -1 ) {
        tmp.result = true;
        return tmp;
      } // end else if 
      else {
        return tmp;
      } // end else 
    } // end if 
    else {
      return tmp;
    } // end else 
  } // end else if 
  else if ( tmp.stats == 2 ) {
    if ( glob_codes[tmp.i].token == "*" || glob_codes[tmp.i].token == "/" ) {
      tmp.i++;
      tmp.stats = 3;
      tmp = Term( tmp );
      return tmp;
    } // end if 
    else {
      tmp.result = false;
      return tmp;
    } // end else 
  } // end else if 
  else if ( tmp.stats == 3 ) {
    tmp.stats = 1;
    tmp = Term( tmp ); // i, stats=1
    return tmp;
  } // end else if 
  else {
    tmp.result = false;
    return tmp;
  } // end else 
} // Term() 

ReturnResult Factor( ReturnResult tmp ) {
  tmp.i = FindNextToken( tmp.i );
  int save_i = tmp.i;

  if ( tmp.i == -1 ) {
    tmp.result = true;
    return tmp;
  } // end if 
  else if ( glob_codes[tmp.i].token == "(" ) {
    tmp.i++;
    tmp.stats = 1;
    tmp = ArithExp( tmp );
    if ( tmp.result == true ) {
      if ( tmp.i == -1 ) {
        tmp.result = true;
        return tmp;
      } // end if 
      else {
        tmp.i++;
        tmp.i = FindNextToken( tmp.i );
        if ( tmp.i != -1 && glob_codes[tmp.i].token == ")" ) {
          tmp.result = true;
          return tmp;
        } // end if 
        else if ( tmp.i == -1 ) {
          tmp.result = true;
          return tmp;
        } // end else if 		
        else {
          tmp.result = false;
          return tmp;
        } // end else 
      } // end else 
    } // end if 
    else {
      tmp.result = false;
      return tmp;
    } // end else 
  } // end else if 
  else if ( Is_SIGN( glob_codes[tmp.i].token ) != 0 ) {
    int sign_index = tmp.i;
    glob_codes[tmp.i].type = "SIGN";
    tmp.i++;
    tmp.i = FindNextToken( tmp.i );
    if ( tmp.i != -1 && Is_NUM( glob_codes[tmp.i].token ) == "integer" ) {
      int integer;
      stringstream int_to_string;
      integer = atoi( glob_codes[tmp.i].token.c_str() );
      integer = integer * Is_SIGN( glob_codes[sign_index].token );
      int_to_string << integer;
      glob_codes[tmp.i].token = int_to_string.str();
      glob_codes.erase( glob_codes.begin() + sign_index );
      tmp.i--;

      tmp.result = true;
      return tmp;
    } // end if 
    else if ( tmp.i != -1 && Is_NUM( glob_codes[tmp.i].token ) == "float" ) {
      double flt;
      stringstream flt_to_string;
      flt = atof( glob_codes[tmp.i].token.c_str() );
      flt = flt * Is_SIGN( glob_codes[sign_index].token );
      flt_to_string << flt;
      glob_codes[tmp.i].token = flt_to_string.str();
      glob_codes.erase( glob_codes.begin() + sign_index );
      tmp.i--;

      tmp.result = true;
      return tmp;
    } // end else if 
    else if ( tmp.i == -1 ) {
      tmp.result = true;
      return tmp;
    } // end else if 
    else {
      tmp.result = false;
      return tmp;
    } // end else 
  } // end else if 
  else if ( Is_NUM( glob_codes[tmp.i].token ) == "integer" ) {
    glob_codes[tmp.i].type = Is_NUM( glob_codes[tmp.i].token );
    tmp.result = true;
    return tmp;
  } // end else if 
  else if ( Is_NUM( glob_codes[tmp.i].token ) == "float" ) {
    glob_codes[tmp.i].type = Is_NUM( glob_codes[tmp.i].token );
    tmp.result = true;
    return tmp;
  } // end else if 
  else if ( Is_IDENT( glob_codes[tmp.i].token, tmp.i ) == true ) {
    glob_codes[tmp.i].type = "IDENT";
    tmp.result = true;
    return tmp;
  } // end else if 
  else {
    glob_codes[tmp.i].token = Not_IDENT( glob_codes[tmp.i].token ); // quit@ -> quit 
    if ( Is_IDENT( glob_codes[tmp.i].token, tmp.i ) == true ) { // quit@ -> quit 
      tmp.result = true;
      return tmp;
    } // end if 
    else {
      tmp.result = false;
      return tmp;
    } // end else 
  } // end else 
} // Factor() 

int main() {
  char ch;
  int x_axis = 1, y_axis = 1;
  int brk = 0;
  int uTestNum;
  string str, token_buffer, peek;
  bool quit;
  cin >> uTestNum;
  cout << "Program starts..." << endl;
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
            element.type = "unknwon";
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
          quit = Syntax_check( 0 );
          if ( quit == true ) {
            brk = 1; // break 
          } // end if 
          else {
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
          // line-comment, '/', '\\', '<>', '>=', '<=' or ':=' 
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
                element.type = "unknwon";
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
          // line-comment, '/', '\\', '<>', '>=', '<=' or ':=' 
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
                element.type = "unknwon";
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
          else if ( ( str == ":" && peek == "=" ) || ( str == "<" && peek == ">" ) ) {
          // line-comment, '/', '\\', '<>', '>=', '<=' or ':=' 
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
                element.type = "unknwon";
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
          else if ( ( str == "<" && peek == "=" ) || ( str == ">" && peek == "=" ) ) {
          // line-comment, '/', '\\', '<>', '>=', '<=' or ':=' 
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
                element.type = "unknwon";
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
                  element.type = "unknwon";
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
                  element.type = "unknwon";
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
              element.type = "unknwon";
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
            element.type = "unknwon";
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

