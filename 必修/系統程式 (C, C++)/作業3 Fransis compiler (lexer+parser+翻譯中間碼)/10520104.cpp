# include <iostream>
# include <stdio.h>
# include <string> // string型別, strcpy, assign, substr, erase, find, erase, strstr 
# include <vector>  // 動態陣列 
# include <stdlib.h> // atoi 
# include <sstream> // char to string  
#include <fstream>
using namespace std;

struct Lines{ 
  int row;
  int column;
  string token;
  string type;
}; // 使用者輸入的code 

struct QuadType{
  string quadratic;
  string instruction;
};

struct ReturnResult{
  int i;
  bool result;
  int error; // 1:input不足, 2:變數宣告有問題  
  int pass; // 1: T/F, 2: symbol, 3: compile  
  int subroutine;
  string type;
  int mode;
}; // Recursive-Decent-Parsing時return的type 

struct InputType{ 
  int tablenum;
  int column;
  string token;
  string type;
}; // 使用者輸入的code 

struct AddTableType{ 
  int newsubr;
  bool result;
};

struct IDTable{ 
  string id;
  int subroutine;
  string type;
  int pointer;
};

struct CalStack{
  string data; // 內容  
  string type;
  int tablenum;
  int column;
};

void Readfile( string filename, vector<string>& tmp );
void Scanner( string filename, string ext );
void Print( vector<string>& );
void Lexical_Analysis();
void Find_Token();
bool Is_Symbol( string str );
bool Is_White_Space( string TMP );
void Check_type(); 
bool Is_IDENT( string str ); // identifier 
void Is_String(); // string
string Is_NUM( string str ); 
int Cal_ASCII( string str, string type );
void Set_vector_100();
int Hash_Func( int index, string type, int i, int subroutine, string id_type );
void Write_file( string filename, ReturnResult index );
int NextToken( int i );

void Examine_float();
void Char_String_tab_newline();
void Cut_Full(); // 切出全形符號  
int FindToken( int i ) ; // 跳過whitespace 找出下個token  
bool Find_type( string type );
bool Is_operator( string type );

ReturnResult Parser();
void Parser_2nd_pass();
void Link( int start,  int end, int subroutine );
CalStack Calculation( int subroutine, int index, vector<CalStack>& formula );
CalStack Cal_substack( vector<CalStack>& substack );
AddTableType ADD_to_ID_Table( string id, int subroutine, int i, string type );
ReturnResult Grammar( ReturnResult index ); // grammar 
ReturnResult Prog( ReturnResult index );
ReturnResult ID( int mode, ReturnResult index ); // identifier 
ReturnResult Declarations( ReturnResult index ); // declarations 
ReturnResult Var( ReturnResult index ); // var 
ReturnResult Vars( ReturnResult index ); // var 
ReturnResult Var_sub1( ReturnResult index ); // var_sub1 
ReturnResult Var_sub2( ReturnResult index ); // var_sub2 
ReturnResult Dim( ReturnResult index ); // dimension 
ReturnResult Dims( ReturnResult index ); // dimension 
ReturnResult Nums( ReturnResult index ); // numbers 
ReturnResult Label( ReturnResult index ); // label 
ReturnResult Labels( ReturnResult index ); // label 
ReturnResult Sub( ReturnResult index ); // subroutine 
ReturnResult Subs( ReturnResult index ); // subroutine 
ReturnResult Stmts( ReturnResult index ); // stmts 
ReturnResult Exp( ReturnResult index );
ReturnResult Q( ReturnResult index );
ReturnResult T( ReturnResult index );
ReturnResult F( ReturnResult index );
ReturnResult E( ReturnResult index );
ReturnResult Assign( ReturnResult index );
ReturnResult Output( ReturnResult index );
ReturnResult Input( ReturnResult index );
ReturnResult Goto( ReturnResult index );
ReturnResult If( ReturnResult index );
ReturnResult Call( ReturnResult index );


vector <string> glob_table1; // delimiter 
vector <string> glob_table2; // reserved word 
vector <string> glob_table3; // int 
vector <string> glob_table4; // real
vector <IDTable> glob_table5; // symbol 
vector <int> glob_table7;
vector <InputType> glob_input; // 使用者輸入的code  
vector <QuadType> glob_quad;
vector <int> glob_cal_stack;
int glob_T_reg;
int line = 1;



void Readfile( string filename, vector<string>& tmp, string ext ) { // ext:副檔名  
  ifstream infile;
  filename = filename + ext;
  string str;
  infile.open(filename.c_str());	//.c_str()用來將string 轉成 char[XXX], 因為open函數只能讀char[XXX], 但string撰寫比較方便
	
  if ( infile == NULL ) { cout << "Fail to open " << filename << endl; } //NULL為"空"的意思 
  else {
    cout << "完成開啟" << filename << endl; 
	while ( infile >> str != 0 ) {	//while( infile >> ch)	//當檔案中還有東西可以讀取, >>表示從檔案將資料傳到變數中 
	  if ( str == " " ) {
				//cout << "空白" << endl; 
	  }
	  else if ( str == "\n" ) {
		//cout << "\\n" << endl; 
	  }
	  else if ( str == "\t" ) {
		//cout << "\\t" << endl; 
	  }
	  else {
	  	// cout << "@" << str << "#" << endl;
        tmp.push_back( str );
	  }
	}
  }
}

void Scanner( string filename, string ext ) {
  ifstream infile;
  filename = filename + ext;
  string str;
  infile.open(filename.c_str());	//.c_str()用來將string 轉成 char[XXX], 因為open函數只能讀char[XXX], 但string撰寫比較方便
	
  if ( infile == NULL ) { cout << "Fail to open " << filename << endl; } //NULL為"空"的意思 
  else {
    cout << "完成開啟" << filename << endl; 
    char ch;
    int x_axis = 1, y_axis = 1;
    int brk = 0;
    string str, token_buffer, peek, filename;
    bool quit;

    while ( infile.peek() != EOF ) {
      ch = infile.get();

      stringstream char_to_string;

      char_to_string << ch;
      str = char_to_string.str(); // str = ch  

      if ( Is_White_Space( str ) == true ) { // is whitespace, cutting token  
        InputType element; 
		element.tablenum = -1;
        element.column = -1;
          
        if ( token_buffer.size() == 0 ) { // token buffer內沒東西 加入whitespace 
          element.type = "whitespace";
          element.token = str;
          glob_input.push_back( element );
        } // end if 
        else { // token buffer內有東西 
          if ( Is_NUM( token_buffer ) == "integer" || Is_NUM( token_buffer ) == "float" ) {
            element.type = Is_NUM( token_buffer );
          } // end if 
          else {
            element.type = "unknown";
          } // end else
          
          element.token = token_buffer;
		  glob_input.push_back( element ); 
          element.token = str;
          element.type = "whitespace";
          glob_input.push_back( element );
        } // end else 

        token_buffer.clear(); // token buffer清空  
        if ( str == "\n" ) {

        } // end if, row+1 if token is newline 
      } // end if 
      else if ( Is_Symbol( str ) == true ) { // 如果是普通的token 
        InputType element;
		element.tablenum = -1;
        element.column = -1;

        if ( token_buffer.size() == 0 ) { // token buffer內沒東西 直接加入  
          element.token = str;
          element.type = "delimiter";
          glob_input.push_back( element );
        } // end if 
        else { // token buffer內有東西 
          element.token = token_buffer;
          if ( Is_NUM( token_buffer ) == "integer" || Is_NUM( token_buffer ) == "float" ) {
            element.type = Is_NUM( token_buffer );
          } // end if 
          else {
            element.type = "unknown";
          } // end else 

          glob_input.push_back( element );

          y_axis = y_axis + str.size();
          element.token = str;
          element.type = "delimiter";
          glob_input.push_back( element );
        } // end else 

        token_buffer.clear(); // token buffer清空 
      } // end else if 
      else { // keep getting token
        token_buffer = token_buffer + str;
      } // end else 
    } // end while 
    
    if ( infile.peek() == EOF && token_buffer.size() != 0 ) {
      InputType element;
	  element.tablenum = -1;
      element.column = -1;

      if ( Is_NUM( token_buffer ) == "integer" || Is_NUM( token_buffer ) == "float" ) {
        element.type = Is_NUM( token_buffer );
      } // end if 
      else {
        element.type = "unknown";
      } // end else
          
      element.token = token_buffer;
	  glob_input.push_back( element ); 
	}

    token_buffer.clear(); 
    infile.close();

  }
}


void Print(vector<string>& tmp) {
  for( int i = 0; i < tmp.size(); i++ ) {
  	cout << "Printing:" << tmp[i] << endl; 
  }
}

void Lexical_Analysis() {
  Cut_Full();
  Find_Token();
  Check_type();
  Examine_float();
}

void Find_Token() {
  string str;
  for ( int i = 0; i < glob_input.size(); i++ ) {
    bool result = false;
    str = glob_input[i].token;

    for ( int j = 0; j < str.size(); j++) {
      str[j] = toupper( str[j] ) ; 
	}

    for ( int j = 0; j < glob_table2.size() && result == false; j++ ) {
      if ( str == glob_table2[j] ) {
        glob_input[i].tablenum = 2;
        glob_input[i].column = j + 1;
        glob_input[i].type = "resw";
        result = true;
        j = glob_table2.size();
      }
  	}
  }
}


bool Is_Symbol( string str ) { // 切token時 所需要辨識出的Symbol 
  bool result = false;

  for ( int i = 0; i < glob_table1.size(); i++ ) {
    if ( str == glob_table1[i] ) {
      result = true;
      break;
	}
  }

  return result;
} // Is_Symbol() 

bool Is_White_Space( string TMP ) { // whitespace 
  if ( TMP == " " || TMP == "\t" || TMP == "\n" ) {
    return true;
  } // end if 
  else {
    return false;
  } // end else 
} // Is_White_Space() 


void Check_type() { // 確認文法中所有該對應的type  
  Is_String(); // 處理String 
  

  for ( int i = 0; i < glob_input.size(); i++  ) { // 處理ID 
    if ( Is_IDENT( glob_input[i].token ) == true && glob_input[i].type == "unknown" ) {
      glob_input[i].type = "ID";
    } // end if 
  } // end for 


}

bool Is_IDENT( string str ) {
  bool result = true;
  if ( ( ( int ) str[0] < 65 || ( int ) str[0] > 90 ) ) {  // 1st char not A-Z
    if ( ( ( int ) str[0] < 97 || ( int ) str[0] > 122 ) ) { // 1st char not a-z
      if ( ( int ) str[0] != 95 ) { // 1st char not '_' 
        result = false;
      } // end if 
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

  return result;
} // Is_IDENT() 

void Is_String() {
  int start = -1, end = -1, flag = 0;
  string str; 

  for ( int i = 0; i < glob_input.size() ; i++ ) { // 處理string  

    if ( flag == 0 ) {
      if ( glob_input[i].token == "\'" ) {
        flag = 1;
        start = i;
      } // end if
    } // end if 
    else { // flag == 1 
      if ( glob_input[i].token == "\'" ) {
        flag = 0;
        end = i;
      } // end if
    } // end else 

    if ( start != -1 && end != -1 ) {	
      for ( int j = start + 1; j < end ; j++ ) { // 兩個/" 之間  
        if ( j == start + 1 ) {
          str = glob_input[j].token;
		}
		else {
		  str += glob_input[j].token;	
		}
      } // end for

      str = "\'" + str + "\'";
      
      for ( int j = start + 1; j < end ; j++ ) { // 清除已拿token  
        glob_input.erase( glob_input.begin() + start + 1 );
        i--;
      } // end for
    
      InputType element;
      element.token = str;
      element.type = "string";
      element.column = -1;
      element.tablenum = -1;
      
      glob_input.insert(glob_input.begin() + start + 1, element);

      start = -1;
      end = -1;
    } // end if 
    else if ( start != -1 && i == glob_input.size() - 1 ) { // 沒有第二個/"  
      
    } // end else if 
  } // end for
} 

string Symbol_type( string str, string type ) { // 給予對應的type  
  if ( str == "(" ) {
    return "LBR";
  } // end if 
  else if ( str == ")" ) {
    return "RBR";
  } // end if 
  else if ( str == "." ) {
    return "DOT";
  } // end if 
  else {
    return type; // 照原本的傳回  
  } // end else 
} // Is_Symbol() 


int NextToken( int i ) { // 回傳下個token位置, 沒token回傳-1 
  int while_break = 0;
  string token, type;
  while ( i < glob_input.size() && while_break == 0 ) {
    token = glob_input[i].token;
    type = glob_input[i].type;
    if ( Is_White_Space( token ) == false ) {
      while_break++; // break 
    } // end if 
    else {
      i++;
    }  // end else
  } // end while 

  if ( i >= glob_input.size() ) { // 沒token回傳-1 
    i = -1;
  } // end if 

  return i;
}

//void Comment() {
//  for ( int i = 0; i < glob_input.size(); i++ ) {
//  	if ( glob_input[i].token == "." ) {
//  	  for ( int j = i; j < glob_input.size(); j++ ) {
//  	    if ( glob_input[j].token == "\n" ) {
//  	      for( int k = 0; k < j - i; k++ ) {
//  	      	glob_input.erase( glob_input.begin() + i );
//  	      	j--;
//		  }
//	    }
//	  }
//	}
//  }
//}

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
      if ( ( int ) str[i] == 72 && i == str.size() - 1 ) {
        result = true;
	  }
	  else {
        result = false;
        i = str.size();
	  }
    } // end else if 
  } // end for 
  
  if ( result == false ) { return "error"; } // end if 
  else if ( dotcount == 1 ) { return "float"; } // end else if 
  else if ( dotcount == 0 ) { return "integer"; } // end else if 
  else { return "what?"; } // end else 
} // Is_NUM() 

int Cal_ASCII( string str, string type ) {
  int result = 0;
  if ( type != "string" ) {
    for ( int i = 0;  i < str.size() ; i++ ) { // 一個字元一個字元 判斷 
      result += ( int ) str[i];
    } // end for   	
  }
  else {
    for ( int i = 1;  i < str.size() - 1 ; i++ ) { // 一個字元一個字元 判斷 
      result += ( int ) str[i];
    } // end for
  }
  return result;	
}

void Set_vector_100() {
  string tmp = "";
  IDTable element;
  
  for ( int i = 0; i < 100; i++ ) {
  	glob_table3.push_back( tmp ); // int 
  	glob_table4.push_back( tmp ); // real 
  }
  
  element.id = "";
  element.pointer = -1;
  element.subroutine = -1;
  element.type = "";
  for ( int i = 0; i < 100; i++ ) {
    glob_table5.push_back( element ); // symbol 
  }
  
}

int Hash_Func( int index, string type, int i, int subroutine, string id_type ) {
  int save, result = -1;
  
  if ( type == "integer" ) {
  	if ( glob_table3[index] == "" ) {
  		glob_table3[index] = glob_input[i].token;
  		glob_input[i].tablenum = 3;
  		glob_input[i].column = index;
  		result = index;
	}
	else {
	  save = index;

	  if ( glob_table3[index] == glob_input[i].token ) {
  		glob_input[i].tablenum = 3;
  		glob_input[i].column = index;
  		result = index;
	  } 

	  while( glob_table3[index] != glob_input[i].token ) {
	  	index++;
        if ( glob_table3[index] == glob_input[i].token ) {
  		  glob_input[i].tablenum = 3;
  		  glob_input[i].column = index;
  		  result = index;
          break;
		}
	  	else if ( glob_table3[index] == "" ) {
  		  glob_table3[index] = glob_input[i].token;
  		  glob_input[i].tablenum = 3;
  		  glob_input[i].column = index;
  		  result = index;
  		  break;
		}

        if ( index == save ) {
          cout << "hash table已滿" << endl;
		  break; 
		} 

        if ( index == 100 ) {
          index = -1;
		}
	  }
	}
  }
  else if ( type == "float" ) {
  	if ( glob_table4[index] == "" ) {
  		glob_table4[index] = glob_input[i].token;
  		glob_input[i].tablenum = 4;
  		glob_input[i].column = index;
  		result = index;
	}
	else {
	  save = index;

	  if ( glob_table4[index] == glob_input[i].token ) {
  		glob_input[i].tablenum = 4;
  		glob_input[i].column = index;
  		result = index;
	  } 

	  while( glob_table4[index] != glob_input[i].token ) {
	  	index++;
        if ( glob_table4[index] == glob_input[i].token ) {
  		  glob_input[i].tablenum = 4;
  		  glob_input[i].column = index;
  		  result = index;
          break;
		}
	  	else if ( glob_table4[index] == "" ) {
  		  glob_table4[index] = glob_input[i].token;
  		  glob_input[i].tablenum = 4;
  		  glob_input[i].column = index;
  		  result = index;
  		  break;
		}

        if ( index == save ) {
          cout << "hash table已滿" << endl;
		  break; 
		} 

        if ( index == 100 ) {
          index = -1;
		}
	  }
	}
  }
  else if ( type == "string" ) {
  	if ( glob_table5[index].id == "" ) {
  		glob_table5[index].id = glob_input[i].token;
  		glob_input[i].tablenum = 5;
  		glob_input[i].column = index;
  		result = index;
	}
	else {
	  save = index;
	  
	  if ( glob_table5[index].id == glob_input[i].token ) {
  		glob_input[i].tablenum = 5;
  		glob_input[i].column = index;
  		result = index;
	  } 

	  while( glob_table5[index].id != glob_input[i].token ) {
	  	index++;
	  	
	  	if ( glob_table5[index].id == glob_input[i].token ) {
  		  glob_input[i].tablenum = 5;
  		  glob_input[i].column = index;
  		  result = index;
          break;
		}
	  	else if ( glob_table5[index].id == "" ) {
  		  glob_table5[index].id = glob_input[i].token;
  		  glob_input[i].tablenum = 5;
  		  glob_input[i].column = index;
  		  result = index;
  		  break;
		}

        if ( index == save ) {
          cout << "hash table已滿" << endl;
		  break; 
		} 

        if ( index == 100 ) {
          index = -1;
		}
	  }
	}
  }
  else if ( type == "ID" ) {
      //  struct IDTable{ 
      //  string id;
      //  int subroutine;
      //  string type;
      //  int pointer;
      //  };
    if ( id_type == "-1" ) {  // assign或exp算出來的id  
      id_type = "";
	}  
      
	if ( glob_table5[index].id == glob_input[i].token && glob_table5[index].subroutine == subroutine ) {
  	  glob_input[i].tablenum = 5;
  	  glob_input[i].column = index;
  	  glob_table5[index].subroutine = subroutine;
  	  glob_table5[index].type = id_type;
  	  result = index;
	} 
    else if ( glob_table5[index].id == "" ) {
  	  glob_table5[index].id = glob_input[i].token;
  	  glob_input[i].tablenum = 5;
  	  glob_input[i].column = index;
  	  glob_table5[index].subroutine = subroutine;
      glob_table5[index].type = id_type;
  	  result = index;
	}
	else {
	  save = index;
	  index++;
	  while( 1 ) {
	  	if ( glob_table5[index].id == glob_input[i].token && glob_table5[index].subroutine == subroutine ) {
  		  glob_input[i].tablenum = 5;
  		  glob_input[i].column = index;
  		  glob_table5[index].subroutine = subroutine;
  		  glob_table5[index].type = id_type;
  		  result = index;
  		  break;
	    } 
        else if ( glob_table5[index].id == "" ) {
  		  glob_table5[index].id = glob_input[i].token;
  		  glob_input[i].tablenum = 5;
  		  glob_input[i].column = index;
  		  glob_table5[index].subroutine = subroutine;
  		  glob_table5[index].type = id_type;
  		  result = index;
  		  break;
		}

        if ( index == save ) {
          cout << "hash table已滿" << endl;
		  break; 
		} 

        if ( index == 100 ) {
          index = -1;
		}
		
		index++;
	  }	  
	}
  }

  return result;
}

void Write_file( string filename, ReturnResult result ) {
  int index;
  int last = -1, next;
  if ( filename.find( "_input" ) != -1 ) {
  	index = filename.find( "_input" );
  	filename.erase( index, filename.size() - index );
  }

  int start;
  ofstream outfile;
  filename = filename + "_output" + ".txt";

  outfile.open(filename.c_str());

  if ( outfile == NULL ){cout << "Fail to save " << filename << endl;}
  else {
    for ( int i = 1; i < glob_quad.size(); i++ ) {
      outfile << i << "\t" << glob_quad[i].quadratic << "\t" <<  glob_quad[i].instruction << endl;
    }

    if ( result.result == false ) {
  	  if ( result.i != -1 ) {
  	    outfile << "line " << line << "  : Unexpected token: " << glob_input[result.i].token << endl;
		outfile << "Syntax Error" << endl;	
	  }
	  else {
	    outfile << "line " << line << "  : Unexpected token at the end." << endl;	
		outfile << "Syntax Error" << endl;		
	  }
    }
  }
  cout << filename << " saved successfully" << endl;
  outfile.close();
}

void Examine_float() { // 檢查是不是浮點數 
  int flag = 0, start, end;
  int dotcount = 0;
  string temp = "NULL";
  for ( int i = 0; i < glob_input.size() ; i++ ) { // 使用者輸入的code的token  
    if ( glob_input[i].type == "integer" ) { // 此token為整數  
      if ( flag == 0 ) { // 前面還不認為是小數  
        temp = glob_input[i].token;
        start = i;
        flag = 1; // 從現在起開始將tokens組合為小數 
      } // end if 
      else if ( flag == 1 ) { // 已認定是小數  
        temp += glob_input[i].token; // 開始組合小數  
      } // end if 
    } // end if 
    else if ( glob_input[i].token == "." ) { // 此token為'.' 
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
          glob_input[start].type = "float";
          glob_input[start].token = temp;
          for ( int j = start + 1; j <= end ; j++ ) {
            glob_input.erase( glob_input.begin() + start + 1 );
            i--;
          } // end for 

          temp = ".";
          start = i;
          dotcount = 1;
        } // end else 
      } // end else if 
    } // end if 

    if ( i < glob_input.size() - 1 && glob_input[i+1].type != "integer" && glob_input[i+1].token != "." ) {
      if ( flag == 1 ) {
        end = i;
        if ( start != end ) { // case: .23.
          glob_input[start].type = "float";
          glob_input[start].token = temp;
          for ( int j = start + 1; j <= end ; j++ ) {
            glob_input.erase( glob_input.begin() + start + 1 );
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

void Char_String_tab_newline() {
  int start = -1, end = -1, flag = 0;
  string str;

  for ( int i = 0; i < glob_input.size() ; i++ ) { // 先處理string  
    if ( flag == 0 ) {
      if ( glob_input[i].token == "\"" ) {
        flag = 1;
        start = i;
      } // end if
    } // end if 
    else { // flag == 1 
      str = str + glob_input[i].token;

      if ( glob_input[i].token == "\"" ) {
        flag = 0;
        end = i;
      } // end if
    } // end else 

    if ( start != -1 && end != -1 ) {
      str = ""; 
      for ( int j = start + 1; j < end ; j++ ) { // 兩個/" 之間  
        if ( glob_input[j].token == "\\n" ) {
          glob_input[j].token = "\n";
        } // end if 
        else if ( glob_input[j].token == "\\t" ) {
          glob_input[j].token = "\t";
        } // end else if 

        str = str + glob_input[j].token;
      } // end for

      // cout << "str: " << str << endl;
      glob_input[start].token = str;
      glob_input[start].type = "string";

      for ( int j = start + 1; j <= end ; j++ ) { // 清除已拿token  
        glob_input.erase( glob_input.begin() + start + 1 );
        i--;
      } // end for

      start = -1;
      end = -1;
    } // end if 
    else if ( start != -1 && i == glob_input.size() - 1 ) { // 沒有第二個/"  
      
    } // end else if 
  } // end for 

  for ( int i = 0; i < glob_input.size() ; i++ ) { // 處理char  
    if ( flag == 0 ) {
      if ( glob_input[i].token == "\'" ) {
        flag = 1;
        start = i;
      } // end if
    } // end if 
    else { // flag == 1 
      str = str + glob_input[i].token;

      if ( glob_input[i].token == "\'" ) {
        flag = 0;
        end = i;
      } // end if
    } // end else 

    if ( start != -1 && end != -1 ) {
      str = ""; 
      for ( int j = start + 1; j < end ; j++ ) { // 兩個/' 之間  
        if ( glob_input[j].token == "\\n" ) {
          glob_input[j].token = "\n";
        } // end if 
        else if ( glob_input[j].token == "\\t" ) {
          glob_input[j].token = "\t";
        } // end else if 

        str = str + glob_input[j].token;
      } // end for

      // cout << "str: " << str << endl;
      glob_input[start].token = str;
      glob_input[start].type = "char";

      for ( int j = start + 1; j <= end ; j++ ) { // 清除已拿token  
        glob_input.erase( glob_input.begin() + start + 1 );
        i--;
      } // end for

      start = -1;
      end = -1;
    } // end if 
    else if ( start != -1 && i == glob_input.size() - 1 ) { // 沒有第二個/'  
      
    } // end else if 
  } // end for 
} // Char_String_tab_newline() 

void Cut_Full() {
  InputType element;
  for ( int i = 0; i < glob_input.size(); i++ ) {
    if ( Is_NUM( glob_input[i].token ) == "integer" || Is_NUM( glob_input[i].token ) == "float" ) {
      glob_input[i].type = Is_NUM( glob_input[i].token );
    } // end if

  	if ( glob_input[i].token.find("↑") != -1 ) {
  	  if ( glob_input[i].token.find("↑") == 0 ) {
  		string tmp = glob_input[i].token.substr( 0, 2 );
  	  	glob_input[i].token.erase( 0, 2 );
  	  	element.column = 1;
  	  	element.tablenum = 9;
  	  	element.token = tmp;
  	  	element.type = "delimiter";
  	  	glob_input.insert( glob_input.begin() + i, element );	
	  }
  	  else if ( glob_input[i].token.size() > 1 ) {
  	  	string tmp = glob_input[i].token.substr( 0, glob_input[i].token.find("↑") );
  	  	glob_input[i].token.erase( 0, glob_input[i].token.find("↑") );
  	  	element.column = -1;
  	  	element.tablenum = -1;
  	  	element.token = tmp;
  	  	element.type = "unknown";
  	  	glob_input.insert( glob_input.begin() + i, element );
  	  	i--;
	  }
	  else { // 只有"↑"  
	    element.tablenum = 1;
  	  	element.column = 9;
  	  	element.type = "delimiter";
	  }
	} 
  }

  for ( int i = 0; i < glob_input.size(); i++ ) {
  	if ( glob_input[i].token.find("‘") != -1 ) {
  	  if ( glob_input[i].token.find("‘") == 0 ) {
  		string tmp = glob_input[i].token.substr( 0, 2 );
  	  	glob_input[i].token.erase( 0, 2 );
  	  	element.column = 1;
  	  	element.tablenum = 10;
  	  	element.token = tmp;
  	  	element.type = "delimiter";
  	  	glob_input.insert( glob_input.begin() + i, element );	
	  }
  	  else if ( glob_input[i].token.size() > 1 ) {
  	  	string tmp = glob_input[i].token.substr( 0, glob_input[i].token.find("‘") );
  	  	glob_input[i].token.erase( 0, glob_input[i].token.find("‘") );
  	  	element.column = -1;
  	  	element.tablenum = -1;
  	  	element.token = tmp;
  	  	element.type = "unknown";
  	  	glob_input.insert( glob_input.begin() + i, element );
  	  	i--;
	  }
	  else { // 只有"↑"  
	    element.tablenum = 1;
  	  	element.column = 10;
  	  	element.type = "delimiter";
	  }
	} 
  }

  for ( int i = 0; i < glob_input.size(); i++ ) {
  	if ( glob_input[i].token.find("’") != -1 ) {
  	  if ( glob_input[i].token.find("’") == 0 ) {
  		string tmp = glob_input[i].token.substr( 0, 2 );
  	  	glob_input[i].token.erase( 0, 2 );
  	  	element.column = 1;
  	  	element.tablenum = 11;
  	  	element.token = tmp;
  	  	element.type = "delimiter";
  	  	glob_input.insert( glob_input.begin() + i, element );	
	  }
  	  else if ( glob_input[i].token.size() > 1 ) {
  	  	string tmp = glob_input[i].token.substr( 0, glob_input[i].token.find("’") );
  	  	glob_input[i].token.erase( 0, glob_input[i].token.find("’") );
  	  	element.column = -1;
  	  	element.tablenum = -1;
  	  	element.token = tmp;
  	  	element.type = "unknown";
  	  	glob_input.insert( glob_input.begin() + i, element );
  	  	i--;
	  }
	  else { // 只有"↑"  
	    element.tablenum = 1;
  	  	element.column = 11;
  	  	element.type = "delimiter";
	  }
	} 
  }
}

int FindToken( int i ) { // 跳過whitespace 找出下個token  
  int while_break = 0;
  string token, type;
  while ( i < glob_input.size() && while_break == 0 ) {
    token = glob_input[i].token;
    if ( Is_White_Space( token ) == false ) {
      while_break++; // break 
    } // end if 
    else {
      i++;
    }  // end else
  } // end while 

  if ( i >= glob_input.size() ) {
    i = -1;
  } // end if 

  return i;
} // FindToken() 

bool Find_type( string type ) {
  bool result = false;

  for ( int i = 0; i < type.size(); i++) {
    type[i] = toupper( type[i] ) ; 
  }

  if ( type == "BOOLEAN" || type == "INTEGER" || type == "REAL" ) {
  	result = true;
  }
 
  return result;
}

bool Is_operator( string op ) {
  bool result = false;

  for ( int i = 0; i < op.size(); i++) {
    op[i] = toupper( op[i] ) ; 
  }

  if ( op == "AND" || op == "OR" ) {
  	result = true;
  }
  else if ( op == "EQ" || op == "GE" || op == "LE" || op == "NE" ) {
  	result = true;
  }
  else if ( op == "GT" || op == "LT" ) {
  	result = true;
  }
 
  return result;
}

ReturnResult Parser() {
  int save, table6_line;
  glob_T_reg = 0;
  QuadType tmp;
  ReturnResult index;
  index.error = 0;
  index.i = 0;
  index.pass = 1;
  index.result = true;
  index.subroutine = -1;
  
  while ( index.result == true && index.i != -1 && index.error == 0 ) {
  	save = index.i;
  	index = Grammar( index );
  	// cout << "end token: " << glob_input[index.i].token << endl; 
  	// cout << "T/F: " << index.result << endl;
  	// cout << "index.i: " << index.i << endl;
  	if ( index.result == true ) {
  	  //index.i = save;
  	  //index.pass = 2;
  	  //index = Grammar( index );
  	  int flag = 0;
      for ( int i = save; i <= index.i && index.error == 0; i++ ) {
      	if ( flag == 1 ) {
      	  if ( glob_input[i].token == "DIMENSION" || glob_input[i].token == "BOOLEAN" || glob_input[i].token == "CHARACTER" || 
		    glob_input[i].token == "INTEGER" || glob_input[i].token == "LABEL" || glob_input[i].token == "REAL" ) {
      	    index.type = glob_input[i].token;
      	    flag = 2;
		  }	
		}
      	else if ( glob_input[i].type == "ID" ) {
      	  if ( index.type == "PROGRAM" || index.type == "SUBROUTINE" ) {
      	  	AddTableType temp;

      	  	if ( flag == 0 ) {
      	      temp = ADD_to_ID_Table( glob_input[i].token, -1, i, index.type );
      	      if ( temp.result == false ) {
      	        index.error = 2;
      	        i = index.i + 1 ; // break 
      	      }
      	      else {
      	        index.subroutine = temp.newsubr;
      	      	table6_line = glob_quad.size();
      	      	glob_table5[temp.newsubr].pointer = table6_line;
      	      }
			  flag = 1;	
			}
//			else {
//		      temp = ADD_to_ID_Table( glob_input[i].token, index.subroutine, i, index.type );
//      	      if ( temp.result == false ) {
//      	        index.error = 2;
//      	        i = index.i + 1 ; // break 
//      	      }
//      	      else {
//      	      	table6_line = glob_quad.size();
//      	      	glob_table5[temp.newsubr].pointer = table6_line;
//			  }
//			}
		  }
		  else if ( index.type == "CALL" ) {
      	    int j = i + 1, dim = 0, position = glob_table7.size();
      	    vector <int> size_tmp;

      	    while ( glob_input[j].token != ")" ) { // add to table7 
      	      if ( glob_input[j].type != "delimiter" && glob_input[j].type != "whitespace" ) {
      	      	bool found = false;
      	      	dim++;
				if ( glob_input[j].type == "integer" ) {
				  for ( int k = 0; k < glob_table3.size(); k++ ) {
				  	if ( glob_input[j].token == glob_table3[k] ) {
				  	  size_tmp.push_back( 3 );
				  	  size_tmp.push_back( k );
				  	  found = true;
				  	  k = glob_table3.size();
					}
				  }

				  if ( found == false ) {
      				size_tmp.push_back( 3 );
      				int ascii_sum = Cal_ASCII( glob_input[i].token , glob_input[i].type ) % 100;
				    size_tmp.push_back( Hash_Func( ascii_sum, glob_input[i].type, i, -1, glob_input[i].type) );
				  }
				}
				else if ( glob_input[j].type == "float" ) {
				  for ( int k = 0; k < glob_table4.size(); k++ ) {
				  	if ( glob_input[j].token == glob_table4[k] ) {
				  	  size_tmp.push_back( 4 );
				  	  size_tmp.push_back( k );
				  	  found = true;
				  	  k = glob_table4.size();
					}
				  }

				  if ( found == false ) {
      				size_tmp.push_back( 4 );
      				int ascii_sum = Cal_ASCII( glob_input[i].token , glob_input[i].type ) % 100;
				    size_tmp.push_back( Hash_Func( ascii_sum, glob_input[i].type, i, -1, glob_input[i].type) );
				  }
				}
				else if ( glob_input[j].type == "ID" ) {
				  for ( int k = 0; k < glob_table5.size(); k++ ) {
				  	if ( glob_input[j].token == glob_table5[k].id && index.subroutine == glob_table5[k].subroutine ) {
				  	  size_tmp.push_back( 5 );
				  	  size_tmp.push_back( k );
				  	  found = true;
				  	  k = glob_table5.size();
					}
				  }

				  if ( found == false ) {
      	            index.error = 2;
      	            i = index.i + 1 ; // break 
				  }
				}
			  }
			  j++;
		    }
		    
		    if ( index.error == 0 ) {
		      tmp.instruction = "CALL ";
			  for ( int k = i; k <= j; k++ ) {
			    tmp.instruction += glob_input[k].token;
			  } 
		      i = j;
		      
		      glob_table7.push_back( dim ); // dimension 
			  for ( int j = 0; j < size_tmp.size(); j++ ) {
			    glob_table7.push_back( size_tmp[j] );
			  }

      	      string table, column;
      	      stringstream int_to_string1, int_to_string2;
              int_to_string1 << 7;
              table = int_to_string1.str();
              int_to_string2 << position;
              column = int_to_string2.str();
              
      	      tmp.quadratic = "((2,3)	,CALL	,	,(" + table + "," + column + ")	)";
      	      glob_quad.push_back( tmp );
			}
		  }
		  else if ( index.type == "BOOLEAN" || index.type == "CHARACTER" || 
		  index.type == "INTEGER" || index.type == "LABEL" || index.type == "REAL" ) {
		  	AddTableType temp;
		    temp = ADD_to_ID_Table( glob_input[i].token, index.subroutine, i, index.type );
      	    if ( temp.result == false ) {
      	      index.error = 2;
      	      i = index.i + 1 ; // break 
      	    }
      	    else {
      	      string table, column;
      	      stringstream int_to_string1, int_to_string2;
              int_to_string1 << glob_input[i].tablenum;
              table = int_to_string1.str();
              int_to_string2 << glob_input[i].column;
              column = int_to_string2.str();
              
      	      tmp.instruction = glob_input[i].token;
      	      tmp.quadratic = "((" + table + "," + column + ")	,	,	,	)";
      	      glob_quad.push_back( tmp );
			}
		  }
		  else if ( index.type == "DIMENSION" ) {
		  	AddTableType temp;
		    temp = ADD_to_ID_Table( glob_input[i].token, index.subroutine, i, index.type );
      	    if ( temp.result == false ) {
      	      index.error = 2;
      	      i = index.i + 1 ; // break 
      	    }
      	    else {
      	      int j = i + 1, dim = 0, position = glob_table7.size(), type = -1;
      	      vector <int> size_tmp;
      	      while ( glob_input[j].token != ")" ) { // add to table7 
      	      	if ( glob_input[j].type != "delimiter" && glob_input[j].type != "whitespace" ) {
      	      	  dim++;
				  if ( glob_input[j].type == "integer" ) {
				    size_tmp.push_back( atoi( glob_input[j].token.c_str() ) );
				  } 
				}
				j++;
			  }
			  
		      if ( index.error == 0 ) {
			    glob_table7.push_back( dim ); // dimension 
			    for ( int k = 0; k < size_tmp.size(); k++ ) {
			      glob_table7.push_back( size_tmp[k] );
			    }
			    glob_table7.push_back( type );
			    glob_table5[temp.newsubr].pointer = position;


      	        string table, column;
      	        stringstream int_to_string1, int_to_string2;
                int_to_string1 << glob_input[i].tablenum;
                table = int_to_string1.str();
                int_to_string2 << glob_input[i].column;
                column = int_to_string2.str();
              
      	        tmp.instruction = glob_input[i].token;
      	        tmp.quadratic = "((" + table + "," + column + ")	,	,	,	)";
      	        glob_quad.push_back( tmp );
		        i = j;
			  }
			}
		  }
		}
		else if ( glob_input[i].token == "GTO" && index.type != "IF" ) {
		  string id;
		  QuadType tmp;
		  tmp.instruction = "";
		  for ( int j = save; j < index.i; j++ ) {
		  	tmp.instruction += glob_input[j].token;

		  	if ( glob_input[j].type == "ID" ) {
		  	  id = glob_input[j].token;
			  j = index.i; // break 
			} 
		  }
		  
		  // cout << "id: " << id << endl;
		  
		  for ( int j = glob_quad.size() - 1; j >= 0 ; j-- ) {
		  	if ( glob_quad[j].instruction.find( id ) != string::npos ) {
		  	  stringstream ss;
		  	  ss << j;
		  	  id = ss.str();
		  	  tmp.quadratic = "((2,11)	,	,	,(6," + id + "))"; // "(GTO	,	,	,(6," + gto + "))";
		  	  j = -1; // break 
			}
		  }
		
      	  glob_quad.push_back( tmp ); // push THEN之後的GTO  
		}
		else if ( glob_input[i].token == "ENP" ) {
		  QuadType tmp;
		  tmp.instruction = "";
		  for ( int j = save; j < index.i; j++ ) {
		  	tmp.instruction += glob_input[j].token;
		  } 
      	  tmp.quadratic = "((2,6)	,	,	,	)";
      	  glob_quad.push_back( tmp );
		}
		else if ( glob_input[i].token == "ENS" ) {
		  QuadType tmp;
		  tmp.instruction = "";
		  for ( int j = save; j < index.i; j++ ) {
		  	tmp.instruction += glob_input[j].token;
		  } 
      	  tmp.quadratic = "((2,7)	,	,	,	)";
      	  glob_quad.push_back( tmp );
		}
	  }
	  
	  if ( index.type == "ASSIGN" ) {
	    Link( save, index.i, index.subroutine );
        for ( int i = save; i <= index.i && index.error == 0; i++ ) {
          if ( glob_input[i].token != ";" && glob_input[i].type != "whitespace" ) {
          	glob_cal_stack.push_back( i );
		  }
        }
//        cout << "glob_cal_stack: " << endl;
//	    for ( int i = 0; i < glob_cal_stack.size(); i++ ) {
//	      cout << glob_input[glob_cal_stack[i]].token << endl;
//	    }
//	    cout << endl;
	    



        string A, B, oprtr, symbol = "";
        QuadType tmp;
        vector<string> dim, dim2;
        InputType it;
        int flag = -1, start = -1, end = -1, LP, RP, EQ, ASSIGN, pointer, col, tbl;
        int min =  glob_cal_stack[0];
        int max = glob_cal_stack[glob_cal_stack.size() - 1];
        // cout << "min: "<< min << "max: " << max << endl;


        for ( int i = min; i <= max; i++ ) {
          if ( ( glob_input[i].type == "DIMENSION" || glob_input[i].type == "BOOLEAN" || glob_input[i].type == "CHARACTER" || 
		  glob_input[i].type == "INTEGER" || glob_input[i].type == "LABEL" || glob_input[i].type == "REAL" )  && flag == -1 ) {
            ASSIGN = i;
            flag = 0;
            if ( glob_input[i].type == "DIMENSION" ) { // A ( K ) -> A(K) 
              string str = glob_input[i].token;
              for ( int j = i + 1; j <= max ; j++ ) {
              	str += glob_input[j].token;
				
				if ( glob_input[j].token == ")" ) {
				  glob_input.erase( glob_input.begin() + j );
              	  j = max + 2; // break 
				}
				else {
				  glob_input.erase( glob_input.begin() + j );
				}
				
				index.i--;
				max--;
				j--;
			  }
			  
			  glob_input[i].token = str;
			}
	      }
  	      else if ( glob_input[i].token == "=" ) {
  	        EQ = i;
  	        flag = 1;
	      }
  	      else if ( glob_input[i].type == "DIMENSION" && flag == 1 ) {
			for ( int j = 0; j < glob_table5.size(); j++ ) {
			  if ( glob_table5[j].id == glob_input[i].token && glob_table5[j].subroutine == index.subroutine ) {
			  	pointer = glob_table5[j].pointer;
			  	// cout << "pointer111: " << pointer << endl;
			  }
			}

            flag = 2;
//            for ( int j = ASSIGN + 1; j < EQ; j++ ) {
//      	      glob_input[ASSIGN].token += glob_input[j].token;
//      	      cout <<  " glob_input[ASSIGN].token:  " <<  glob_input[ASSIGN].token << endl; 
//      	      glob_input.erase( glob_input.begin() + ASSIGN + 1 );
//      	      EQ--;
//      	      j--;
//      	      i--;
//      	      max--;
//	        }
            start = i;
            symbol = glob_input[i].token;
			// cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%symbol: " << symbol << endl;
            col = glob_input[i].column;
            tbl = glob_input[i].tablenum;
	      }
	      else if ( glob_input[i].token == ")" && flag == 2 ) {
	        end = i;
	        flag = 1;
	  
	        if ( start != -1 && end != -1 ) {
	          for ( int j = start; j <= end; j++ ) {
	            if ( glob_input[j].type != "delimiter" ) {
	    	      dim.push_back( glob_input[j].token );
		        }
	  	        glob_input.erase( glob_input.begin() + j );
	  	        index.i--;
	  	        max--;
	  	        j--;
	  	        end--;
	          }
	  
	          if ( dim.size() == 3 ) { // for 2-dimensional array 
	            int first_dimension;
	            stringstream int_to_string;
	            it.column = -1;
	            it.tablenum = -1;
	            it.type = "integer";
                // cout << "pointer: " << pointer << endl;
                // for ( int j = 0; j < glob_table7.size(); j++ ) {
                //   cout << "glob_table7[j]: " << glob_table7[j] << endl;
				// }

	            it.column = -1;
	            it.tablenum = -1;
                it.type = "delimiter";
                it.token = "DIM_END";
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 

				first_dimension = glob_table7[pointer + 1];
				it.type = "ID"; 
				int_to_string << first_dimension;
                it.token = int_to_string.str();
                for ( int j = 0; j < glob_table3.size() ; j++ ) {
                  if ( glob_table3[j] == it.token ) {
                  	it.column = j;
				  }
                  it.tablenum = 3;
				}
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 
                
                it.column = -1;
	            it.tablenum = -1;
                it.type = "delimiter";
                it.token = "*";
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 
                it.type = "delimiter";
                it.token = ")";
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 
                it.type = "integer";
                it.token = "1";
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 
                it.type = "delimiter";
                it.token = "-";
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 
                it.type = "ID";
                it.token = dim[2];
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 
                it.type = "delimiter";
                it.token = "(";
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 
                it.type = "delimiter";
                it.token = "+";
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 
                it.type = "ID";
                it.token = dim[1];
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 
                
                it.type = "delimiter";
                it.token = "DIM_START";
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 

                it.type = "ID";
                it.token = symbol;
                it.column = col;
                it.tablenum = tbl;
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++;
	          }
	          else if ( dim.size() == 2 ) { // for 1-dimensional array 
	            it.column = -1;
	            it.tablenum = -1;
                it.type = "delimiter";
                it.token = "DIM_END";
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 

 				it.type = "ID"; 
                it.token = dim[1];
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 

                it.type = "delimiter";
                it.token = "DIM_START";
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++; 
                
                it.type = "ID";
                it.token = symbol;
                it.column = col;
                it.tablenum = tbl;
                glob_input.insert( glob_input.begin() + start, it);
                index.i++;
                max++;
			  }
	          start = -1, end = -1;
	          dim.clear();
	        }
	      }
        }
  
        Link( min, max, index.subroutine );

//        for ( int i = 0; i < glob_table5.size(); i++ ) {
//  	      cout << "id: " << glob_table5[i].id << " subroutine: " << glob_table5[i].subroutine << " type: " << glob_table5[i].type << " pointer: " << glob_table5[i].pointer << endl;
//        }
//        cout << endl;
//        cout << "linked " << endl; 
//        for ( int i = min; i <= max; i++ ) {
//  	      cout << glob_input[i].token << " " << glob_input[i].type << " " << glob_input[i].tablenum << " " << glob_input[i].column << endl;
//        }

        vector<CalStack> stack;
        CalStack element;

        for ( int i = min; i <= max ; i++ ) {
          if ( Is_White_Space( glob_input[i].token ) == false && glob_input[i].token != ";" ) { // 忽略';'  
            element.data = glob_input[i].token;
            element.type = glob_input[i].type;
            element.tablenum = glob_input[i].tablenum;
            element.column = glob_input[i].column;
            stack.push_back( element );
          } // end if 
        } // end for 


        Calculation( index.subroutine, stack.size() - 1, stack );
	  }
	  else if ( index.type == "IF" ) {
	  	int Then, Else, save1, save2, flag = 0;
        vector<CalStack> stack;
        CalStack element;
        
        Link( save, index.i, index.subroutine );

        for ( int i = save; i <= index.i && index.error == 0; i++ ) {
          if ( glob_input[i].token == "IF" ) {
          	flag = 1;
		  }
          else if ( glob_input[i].token == "THEN" ) {
          	Then = i;
          	i = index.i + 1; // break 
		  }
          else if ( flag == 1 && glob_input[i].token != ";" && glob_input[i].type != "whitespace" ) {
            element.data = glob_input[i].token;
            element.type = glob_input[i].type;
            element.tablenum = glob_input[i].tablenum;
            element.column = glob_input[i].column;
            stack.push_back( element );
		  }
        }

		Calculation( index.subroutine, stack.size() - 1, stack );

      	string reg, column, label = "";
      	stringstream int_to_string1, int_to_string2;
        int_to_string1 << glob_T_reg;
        reg = int_to_string1.str();
        int_to_string2 << glob_quad.size() + 1;
        column = int_to_string2.str();
    
    	save1 = glob_quad.size();
    	for ( int j = save; j <= index.i; j++ ) {
    	  if ( glob_input[j].token == "IF" ) {
    	  	j = index.i + 1;
		  }
		  else {
		  	label += glob_input[j].token;
		  }
		}
		
		tmp.instruction = label + "IF T" + reg + " GO TO " + column + ", ELSE GO TO save1";
      	tmp.quadratic = "((2,12)	,(0," + reg + ")	,(6," + column + ")	,(6,save1)	)";
      	glob_quad.push_back( tmp );

        flag = 0;
        stack.clear();
        for ( int i = Then; i <= index.i && index.error == 0; i++ ) {
          if ( glob_input[i].token == "THEN" ) {
          	flag = 1;
		  }
          else if ( glob_input[i].token == "ELSE" ) {
          	Else = i;
          	i = index.i + 1; // break 
		  }
          else if ( flag == 1 && glob_input[i].token != ";" && glob_input[i].type != "whitespace" ) {
            element.data = glob_input[i].token;
            element.type = glob_input[i].type;
            element.tablenum = glob_input[i].tablenum;
            element.column = glob_input[i].column;
            stack.push_back( element );
		  }
        }

		string gto;
		stringstream int_to_string;
		int_to_string << glob_quad.size() + 1;
		gto = int_to_string.str();
		glob_quad[save1].instruction = label + "IF T" + reg + " GO TO " + column + ", ELSE GO TO " + gto;
		glob_quad[save1].quadratic = "((2,12)	,(0," + reg + ")	,(6," + column + ")	,(6," + gto + ")	)";
        
        
        CalStack result;
        result = Calculation( index.subroutine, stack.size() - 1, stack );
        gto = result.data;
        
       	save2 = glob_quad.size();
		tmp.instruction = "";
      	tmp.quadratic = "((2,11)	,	,	,(6,GTO))"; // "(GTO	,	,	,(6," + gto + "))";
      	glob_quad.push_back( tmp ); // push THEN之後的GTO  
      	
      	if ( gto.find( "GTO" ) == string::npos ) {
      	  stringstream int_to_string;
		  int_to_string << glob_quad.size();
		  gto = int_to_string.str();
		  glob_quad[save1].instruction = label + "IF T" + reg + " GO TO " + column + ", ELSE GO TO " + gto;
		  glob_quad[save1].quadratic = "((2,12)	,(0," + reg + ")	,(6," + column + ")	,(6," + gto + ")	)";
		}
        

		stack.clear();
        for ( int i = Else; i <= index.i && index.error == 0; i++ ) {
          if ( glob_input[i].token == "ELSE" ) {
          	flag = 1;
		  }
          else if ( flag == 1 && glob_input[i].token != ";" && glob_input[i].type != "whitespace" ) {
            element.data = glob_input[i].token;
            element.type = glob_input[i].type;
            element.tablenum = glob_input[i].tablenum;
            element.column = glob_input[i].column;
            stack.push_back( element );
		  }
        }
        
        result = Calculation( index.subroutine, stack.size() - 1, stack );

		if ( gto.find( "GTO" ) == string::npos ) {
		  stringstream int_to_string3;
		  int_to_string3 << glob_quad.size();
		  gto = int_to_string3.str();
		  glob_quad[save2].quadratic = "((2,11)	,	,	,(6," + gto + ")	)";
		}
		else {
		  glob_quad[save2].instruction = gto;
		}
	  }

	  glob_cal_stack.clear();
	  line++;
	}
  	
  	index.i = FindToken( index.i + 1 );
  }
  
  return index;
}

void Parser_2nd_pass() {
  int start = -1, end = -1;
  for ( int i = 0; i < glob_quad.size(); i++ ) {
  	if ( glob_quad[i].quadratic.find( "CALL" ) != string::npos ) {
  	  start = glob_quad[i].instruction.find( "CALL " ) + 5;
  	  end = glob_quad[i].instruction.find( "(" ) - 1;
  	  
  	  string str = "";
  	  
  	  for ( int j = start; j <= end; j++ ) {
		str += glob_quad[i].instruction[j];
	  }	  
  	  
  	  for ( int j = 0; j < glob_table5.size(); j++ ) {
  	  	if ( glob_table5[j].id == str && glob_table5[j].subroutine == -1 ) {
  	  	  stringstream int_to_string;
		  int_to_string << j;
		  string quad = int_to_string.str();
  	  	  quad = "(5," + quad +  ")";
  	  	  glob_quad[i].quadratic.replace( glob_quad[i].quadratic.find( "CALL" ), 4, quad );
		} 
	  }
	}
  	else if ( glob_quad[i].quadratic.find( "GTO" ) != string::npos ) {
  	  start = glob_quad[i].instruction.find( "GTO " ) + 4;
  	  end = glob_quad[i].instruction.size() - 1;
  	  string str = "";
  	  for ( int j = start; j <= end; j++ ) {
  	    str += glob_quad[i].instruction[j];
	  }
	  
	  //cout << "strttt: " << str << endl;
	  
  	  for ( int j = glob_quad.size() - 1; j >= 0; j-- ) {
  	  	if ( glob_quad[j].instruction != str && glob_quad[j].instruction.find( str ) != string::npos ) {
  	  	  stringstream int_to_string;
		  int_to_string << j;
		  string quad = int_to_string.str();
  	  	  glob_quad[i].quadratic.replace( glob_quad[i].quadratic.find( "GTO" ), 3, quad );
  	  	  j = -1; // break;
		} 
	  }  
	}
  }
}

void Link( int start,  int end, int subroutine ) {
  int index, ascii_sum;
  for ( int i = start; i <= end; i++ ) {
  	if ( glob_input[i].token.find( "(" ) != -1 && glob_input[i].token.find( ")" ) != -1 ) { // array *********************************************
  	  string str = "";
  	  for ( int k = 0; k < glob_input[i].token.size(); k++ ) {
	    if ( glob_input[i].token[k] == '(' ) {
   	      for ( int j = 0; j < glob_table5.size(); j++ ) {
  	        if ( str == glob_table5[j].id && subroutine == glob_table5[j].subroutine ) {
  	  	      glob_input[i].type = glob_table5[j].type;
  	          glob_input[i].tablenum = 5;
  	          glob_input[i].column = j;
  	          j = glob_table5.size(); // break; 
	        }
	      }
	      k = glob_input[i].token.size(); // break; 
		}
		else {
		  str += glob_input[i].token[k];
		}
	  }
	}
	else {
  	  for ( int j = 0; j < glob_table5.size(); j++ ) {
  	    if ( glob_input[i].token == glob_table5[j].id && subroutine == glob_table5[j].subroutine ) {
  	  	  glob_input[i].type = glob_table5[j].type;
  	  	  glob_input[i].tablenum = 5;
  	  	  glob_input[i].column = j;
  	  	  j = glob_table5.size(); // break; 
	    }
  	    else if ( glob_input[i].token == glob_table5[j].id && glob_table5[j].subroutine == -1 ) { // program or subroutine 
  	  	  glob_input[i].type = glob_table5[j].type;
  	  	  glob_input[i].tablenum = 5;
  	  	  glob_input[i].column = j;
  	  	  j = glob_table5.size(); // break; 
	    }
	  }
	}
  }

  for ( int i = start; i <= end; i++ ) { 
  	if ( glob_input[i].type == "integer" ) {
      int ascii_sum = Cal_ASCII( glob_input[i].token , glob_input[i].type ) % 100;
      Hash_Func( ascii_sum, glob_input[i].type, i, -1, glob_input[i].type );
	}
	else if ( glob_input[i].type == "float" ) {
      int ascii_sum = Cal_ASCII( glob_input[i].token , glob_input[i].type ) % 100;
      Hash_Func( ascii_sum, glob_input[i].type, i, -1, glob_input[i].type );
	}
  }
}

CalStack Calculation( int subroutine, int index, vector<CalStack>& formula ) { // 計算整個算式  
  vector<CalStack> stack;
  vector<CalStack> substack;
  CalStack element;
  CalStack lp_rp_element;
  vector<CalStack> lp_rp_tmp; // 處理左右括號算式的暫存vector  
  int rp = -1, lp = -1; // 紀錄'('和')'位置  
  bool error = false;

  for ( int i = 0; i <= index ; i++ ) {
    if ( Is_White_Space( formula[i].data ) == false && formula[i].data != ";" ) { // 忽略';'  
      if ( formula[i].data == "GTO" ) {
        element.data = formula[i].data;
        element.type = formula[i].type;
        element.tablenum = formula[i].tablenum;
        element.column = formula[i].column;
        stack.push_back( element );
        
        element.data = "unknown";
        element.type = "unknown";
        element.tablenum = -1;
        element.column = -1;
		stack.push_back( element ); // 多push一個 造成 GTO unknown L92  
	  }
	  else {
        element.data = formula[i].data;
        element.type = formula[i].type;
        element.tablenum = formula[i].tablenum;
        element.column = formula[i].column;
        stack.push_back( element );
	  }
    } // end if 
  } // end for 

  for ( int i = 0; i < stack.size() ; i++ ) { // 先處理括號 優先計算  
    if ( stack[i].data == "DIM_START" ) { lp = i; } // end if,  找出最裡層左括號  

    if ( i == stack.size() - 1 && lp != -1 ) { // end if, 取出括號中的算式 (不含括號)  
      for ( int j = lp + 1; j < stack.size() ; j++ ) {
        if ( stack[j].data == "DIM_END" ) {
          rp = j; 
          j = stack.size(); // break j的for	
        } // end if 
        else {
          lp_rp_element.data = stack[j].data;
          lp_rp_element.type = stack[j].type;
          lp_rp_element.tablenum = stack[j].tablenum;
          lp_rp_element.column = stack[j].column;
          lp_rp_tmp.push_back( lp_rp_element );
        } // end else 
      } // end for 

      CalStack result;
//      for ( int j = 0; j < lp_rp_tmp.size(); j++ ) {
//      	cout << "lp_rp_tmp[j].data: " << lp_rp_tmp[j].data << endl;
//	  }
      result = Calculation( subroutine, lp_rp_tmp.size() - 1, lp_rp_tmp ); // 開始計算子stack  
      lp_rp_tmp.clear();
      stack[rp].data = result.data;
      stack[rp].type = result.type;
      stack[rp].tablenum = result.tablenum;
      stack[rp].column = result.column;

      stringstream regnum, int_to_string1, int_to_string2, int_to_string3, int_to_string4;
      string reg, tbl, col, t1, c1;
      glob_T_reg++;
      regnum << glob_T_reg;
      reg = regnum.str();
	  int_to_string1 << stack[rp].column;
	  col = int_to_string1.str();
	  int_to_string4 << stack[rp].tablenum;
	  tbl = int_to_string4.str();
	  int_to_string2 << stack[lp - 1].tablenum;
	  t1 = int_to_string2.str();
	  int_to_string3 << stack[lp - 1].column;
	  c1 = int_to_string3.str();

	  QuadType tmp;
      tmp.instruction = "T" + reg + "=" + stack[lp - 1].data + "(" + result.data + ")";
      tmp.quadratic = "((1,4)	,(" + t1 + "," + c1 + ")	,(" + tbl +"," + col + ")	,(0," + reg + ")	)"; 
      glob_quad.push_back( tmp );
      
      stack[rp].data = "T" + reg;
      stack[rp].tablenum = 0;
      stack[rp].column = glob_T_reg;
      
      for ( int j = lp - 1; j < rp ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
        stack.erase( stack.begin() + lp - 1 );
      } // end for 

      rp = -1;
      lp = -1;
      i = -1; // 再檢查還有沒有括號  
    } // end if 
  } // end for 
  
  lp_rp_tmp.clear();
  rp = -1, lp = -1; // 紀錄'('和')'位置  

  for ( int i = stack.size() - 1; i >= 0 ; i-- ) { // 先處理括號 優先計算  
    if ( stack[i].data == "(" ) { lp = i; } // end if,  找出最裡層左括號  

    if ( i == 0 && lp != -1 ) { // end if, 取出括號中的算式 (不含括號)  
      for ( int j = lp + 1; j < stack.size() ; j++ ) {
        if ( stack[j].data == ")" ) {
          rp = j; 
          j = stack.size(); // break j的for	
        } // end if 
        else {
          lp_rp_element.data = stack[j].data;
          lp_rp_element.type = stack[j].type;
          lp_rp_element.tablenum = stack[j].tablenum;
          lp_rp_element.column = stack[j].column;
          lp_rp_tmp.push_back( lp_rp_element );
        } // end else 
      } // end for 

      CalStack result;
      result = Calculation( subroutine, lp_rp_tmp.size() - 1, lp_rp_tmp ); // 開始計算子stack  
      lp_rp_tmp.clear();
      stack[rp].data = result.data;
      stack[rp].type = result.type;
      stack[rp].tablenum = result.tablenum;
      stack[rp].column = result.column;
      for ( int j = lp; j < rp ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
        stack.erase( stack.begin() + lp );
      } // end for 

      rp = -1;
      lp = -1;
      i = stack.size(); // 再檢查還有沒有括號  
    } // end if 
  } // end for 
  
//  for ( int i = 0; i < stack.size(); i++ ) {
//  	cout << "@@@ " << stack[i].data << " " << stack[i].type << " " << stack[i].tablenum << " " << stack[i].column << endl;
//  }
//  cout << endl; 
  
  for ( int i = stack.size() - 1; i >= 0; i-- ) { // 處理 "↑" 
    if ( stack[i].data == "↑" ) {
      for ( int j = i - 1; j <= i + 1 ; j++ ) {
        CalStack element;
        element.data = stack[j].data;
        element.type = stack[j].type;
        element.tablenum = stack[j].tablenum;
        element.column = stack[j].column;
        substack.push_back( element );
      } // end for 

      CalStack result;
      result = Cal_substack( substack ); // 開始計算子stack  

      substack.clear();
      stack[i+1].data = result.data;
      stack[i+1].type = result.type;
      stack[i+1].tablenum = result.tablenum;
      stack[i+1].column = result.column;
      for ( int j = i - 1; j < i + 1 ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
        stack.erase( stack.begin()+ i - 1 );
      } // end for 

      i = stack.size() - 1; // 再檢查還有沒有*,/ 	
    } // end if 
  } // end for

  for ( int i = 0; i < stack.size(); i++ ) { // 處理 *,/  
    if ( stack[i].data == "*" || stack[i].data == "/" ) {
      for ( int j = i - 1; j <= i + 1 ; j++ ) {
        CalStack element;
        element.data = stack[j].data;
        element.type = stack[j].type;
        element.tablenum = stack[j].tablenum;
        element.column = stack[j].column;
        substack.push_back( element );
      } // end for 

      CalStack result;
      result = Cal_substack( substack ); // 開始計算子stack  
      substack.clear();
      int end = i + 1;
      int start = i - 1;
      for ( int j = start; j < end ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
        stack.erase( stack.begin() + start );
        i--;
      } // end for 
      stack[i+1].data = result.data;
      stack[i+1].type = result.type;
      stack[i+1].tablenum = result.tablenum;
      stack[i+1].column = result.column;

      i = 0; // 再檢查還有沒有*,/ 	
    } // end if 
  } // end for 

  for ( int i = 0; i < stack.size(); i++ ) { // 處理 +,-  
    if ( stack[i].data == "+" || stack[i].data == "-" ) {
      for ( int j = i - 1; j <= i + 1 ; j++ ) {
        CalStack element;
        element.data = stack[j].data;
        element.type = stack[j].type;
        element.tablenum = stack[j].tablenum;
        element.column = stack[j].column;
        substack.push_back( element );
      } // end for 

      CalStack result;
      result = Cal_substack( substack ); // 開始計算子stack  
      substack.clear();
      int end = i + 1;
      int start = i - 1;
      for ( int j = start; j < end ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
        stack.erase( stack.begin() + start );
        i--;
      } // end for 
      stack[i+1].data = result.data;
      stack[i+1].type = result.type;
      stack[i+1].tablenum = result.tablenum;
      stack[i+1].column = result.column;

      i = 0; // 再檢查還有沒有*,/ 
    } // end if 
  } // end for 
  
  for ( int i = 0; i < stack.size(); i++ ) { // 處理 OP  GTO 
    if ( Is_operator( stack[i].data ) == true || stack[i].data == "GTO" ) {
      if ( stack[i].data == "GTO" ) {
        for ( int j = i; j <= i + 2 ; j++ ) {
          CalStack element;
          element.data = stack[j].data;
          element.type = stack[j].type;
          element.tablenum = stack[j].tablenum;
          element.column = stack[j].column;
          substack.push_back( element );
        } // end for 
	  }
	  else {
        for ( int j = i - 1; j <= i + 1 ; j++ ) {
          CalStack element;
          element.data = stack[j].data;
          element.type = stack[j].type;
          element.tablenum = stack[j].tablenum;
          element.column = stack[j].column;
          substack.push_back( element );
        } // end for 
	  }

      CalStack result;
      result = Cal_substack( substack ); // 開始計算子stack  
      substack.clear();
      int end, start;
      if ( stack[i].data.find( "GTO" ) != string::npos ) {
	  	end = i + 2;
        start = i;
        
        //cout << "GTOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO: " <<  result.data << endl;

        for ( int j = start; j < end ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
          stack.erase( stack.begin() + start );
        } // end for 

        stack[i].data = result.data;
        stack[i].type = result.type;
        stack[i].tablenum = result.tablenum;
        stack[i].column = result.column;  
	  }
	  else {
	  	end = i + 1;
        start = i - 1;

      for ( int j = start; j < end ; j++ ) { // 刪除已拿過括號內的東西(含括號)  
        stack.erase( stack.begin() + start );
        i--;
      } // end for 

      stack[i+1].data = result.data;
      stack[i+1].type = result.type;
      stack[i+1].tablenum = result.tablenum;
      stack[i+1].column = result.column;
        
	  }

      i = 0; // 再檢查還有沒有*,/ 
    } // end if 
  } // end for

//  for ( int i = 0; i < stack.size(); i++ ) {
//    if ( stack[i].data == "=" && stack[2].data.find( "(" ) != string::npos ) {
//      cout << "1D ARRAY: " <<  stack[2].data << endl; 
//    } // end if 
//  } // end for 
  
  
  if ( stack.size() == 3 && stack[1].data == "=" && stack[0].data.find( "(" ) == string::npos ) {
    stringstream int_to_string1, int_to_string2, int_to_string3, int_to_string4;
    string t1, c1, t2, c2;

	int_to_string1 << stack[0].column;
	c1 = int_to_string1.str();
	int_to_string2 << stack[0].tablenum;
	t1 = int_to_string2.str();
	int_to_string3 << stack[2].column;
	c2 = int_to_string3.str();
	int_to_string4 << stack[2].tablenum;
	t2 = int_to_string4.str();

	QuadType tmp;
    tmp.instruction = stack[0].data + "=" + stack[2].data;
    tmp.quadratic = "((1,4)	,(" + t2 + "," + c2 + ")	,	,(" + t1 + "," + c1 + ")	)"; 
    glob_quad.push_back( tmp );
  }
  else if ( stack.size() == 3 && stack[1].data == "=" && stack[0].data.find( "(" ) != string::npos ){ // 1D array在'='左側  
    stringstream int_to_string1, int_to_string2, int_to_string3, int_to_string4, int_to_string5, int_to_string6;
    string t1, c1, t2, c2, c3, t3;

	int_to_string1 << stack[0].column;
	c1 = int_to_string1.str();
	int_to_string2 << stack[0].tablenum;
	t1 = int_to_string2.str();
	int_to_string3 << stack[2].column;
	c2 = int_to_string3.str();
	int_to_string4 << stack[2].tablenum;
	t2 = int_to_string4.str();
	
	int subroutine = glob_table5[stack[0].column].subroutine;
	string str = stack[0].data.substr( stack[0].data.find( "(" ) + 1, stack[0].data.find( ")" ) - stack[0].data.find( "(" ) - 1 );
	for ( int i = 0; i < glob_table5.size(); i++ ) {
	  if ( subroutine == glob_table5[i].subroutine && str == glob_table5[i].id ) {
	    int_to_string5 << i;
	    c3 = int_to_string5.str();
	    int_to_string6 << 5;
	    t3 = int_to_string6.str();
	  }
	}

	QuadType tmp;
    tmp.instruction = stack[0].data + "=" + stack[2].data;
    tmp.quadratic = "((1,4)	,(" + t2 + "," + c2 + ")	,(" + t1 + "," + c1 + ")	,(" + t3 + "," + c3 + ")	)"; 
    glob_quad.push_back( tmp );
  }

  element.data = stack[0].data;
  element.type = "ASSIGN";
  element.tablenum = stack[0].tablenum;
  element.column = stack[0].column;

  //cout << "result: " << element.data << endl; 
  return element;
} // Calculation() 

CalStack Cal_substack( vector<CalStack>& substack ) { // 計算小部分的算式(ex 括號 先乘除後加減)  
  int index, gto = 0;
  CalStack cs;
  bool error = false;
  string reg, A, B;
  QuadType tmp;

//    for ( int i = 0; i < substack.size(); i++ ) {
//    	cout << "substack data:  " << substack[i].data << endl;
//    }

  for ( int i = 0; i < substack.size(); i++ ) {
    if ( i == 1 ) {

      string A_table, A_column, B_table, B_column; 
      stringstream A_t, A_c, B_t, B_c;

      B = substack[i+1].data;
      B_t << substack[i+1].tablenum;
      B_c << substack[i+1].column;
      B_table = B_t.str();
      B_column = B_c.str();

      A = substack[i-1].data;
      A_t << substack[i-1].tablenum;
      A_c << substack[i-1].column;
      A_table = A_t.str();
      A_column = A_c.str();
      
      stringstream regnum;
      glob_T_reg++;

	  if ( substack[0].data == "GTO" ) {
        glob_T_reg--;
      }

      regnum << glob_T_reg;
      reg = regnum.str();

      if ( substack[i].data == "↑" ) {
        tmp.instruction = "T" + reg + "=" + A + "↑" + B;
        tmp.quadratic = "((1,9)	,(" + A_table + "," + A_column + ")	,(" + B_table + "," + B_column +")	,(0," + reg + ")	)";
        glob_quad.push_back( tmp );
      } // end else if 
      else if ( substack[i].data == "*" ) {
        tmp.instruction = "T" + reg + "=" + A + "*" + B;
        tmp.quadratic = "((1,7)	,(" + A_table + "," + A_column + ")	,(" + B_table + "," + B_column +")	,(0," + reg + ")	)";
        glob_quad.push_back( tmp );
      } // end else if 
      else if ( substack[i].data == "/" ) {
        tmp.instruction = "T" + reg + "=" + A + "/" + B;
        tmp.quadratic = "((1,8)	,(" + A_table + "," + A_column + ")	,(" + B_table + "," + B_column +")	,(0," + reg + ")	)";
        glob_quad.push_back( tmp );
      } // end else if 
      else if ( substack[i].data == "+" ) {  
        tmp.instruction = "T" + reg + "=" + A + "+" + B;
        tmp.quadratic = "((1,5)	,(" + A_table + "," + A_column + ")	,(" + B_table + "," + B_column +")	,(0," + reg + ")	)";
        glob_quad.push_back( tmp );
      } // end if 
      else if ( substack[i].data == "-" ) {    
        tmp.instruction = "T" + reg + "=" + A + "-" + B;
        tmp.quadratic = "((1,6)	,(" + A_table + "," + A_column + ")	,(" + B_table + "," + B_column +")	,(0," + reg + ")	)";
        glob_quad.push_back( tmp );
      } // end else if 
      else if ( Is_operator( substack[i].data ) == true ) {
      	string table2_str = "";
      	stringstream table2;
      	
      	table2 << substack[i].column;
      	table2_str = table2.str();

        tmp.instruction = "T" + reg + "=" + A + " " + substack[i].data + " " + B;
        tmp.quadratic = "((2," + table2_str + ")	,(" + A_table + "," + A_column + ")	,(" + B_table + "," + B_column +")	,(0," + reg + ")	)";
        glob_quad.push_back( tmp );
      } // end else if 
      else if ( substack[0].data == "GTO" ) {
        gto = 1;
      } // end else if 


      for ( int j = 0; j < 2 ; j++ ) {
        substack.erase( substack.begin() + i - 1 );
      } // end for 

      i = -1;

    } // end else if 
  } // end for
  if ( gto == 1 ) {
  	cs.data = "GTO " + substack[2].data;
    cs.type = "resw";
    cs.tablenum = 2;
    cs.column = 11;
  }
  else {
    cs.data = "T" + reg;
    cs.type = substack[0].type;
    cs.tablenum = 0;
    cs.column = glob_T_reg;
  }

  //cout << "sub result: " << cs.data << endl; 
  return cs;
} // Cal_substack() 

AddTableType ADD_to_ID_Table( string id, int subroutine, int i, string type ) {
  AddTableType result;
  result.newsubr = -1;
  result.result = true;
  int ascii_sum, index;

  if ( subroutine == -1 ) { // adding subroutine 
  	for ( int j = 0; j < glob_table5.size(); j++ ) {
  	  if ( glob_table5[j].id == id && glob_table5[j].subroutine == -1 ) { // 已經有此subroutine  
  	  	result.result = false;
  	  	break;
	  }
	}
	
	if ( result.result == true ) {
      ascii_sum = Cal_ASCII( id, "ID" );
      index = ascii_sum % 100 ;
      result.newsubr = Hash_Func( index, "ID", i, -1, "" );

      //  struct IDTable{ 
      //  string id;
      //  int subroutine;
      //  string type;
      //  int pointer;
      //  };
	}
  }
  else {
    ascii_sum = Cal_ASCII( id, "ID" );
    index = ascii_sum % 100 ;
    result.newsubr = Hash_Func( index, "ID", i, subroutine, type );
  }
  
  if ( result.newsubr == -1 ) {
  	result.result = false;
  }
  else {
  	result.result = true;
  }
  
  return result;
}

ReturnResult Grammar( ReturnResult index ) {
  int token_i = FindToken( index.i );
  int save_i;
  
  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	index.i = token_i;
  	index = ID( 1, index ); // Label id 
  	
  	if ( index.result == true ) {
  	  token_i = FindToken( index.i + 1 );

  	  if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
      }
      else {
      	save_i = token_i;
      	index.i = save_i;
	    index = Prog( index ); // prog 
	  
	    if ( index.result == false ) {
	  	  index.i = save_i;
	  	  index = Declarations( index ); // declarations 
	  	
	      if ( index.result == false ) {
	  	    index.i = save_i;
	  	    index = Stmts( index ); // stmts 
	      }
	    }	
	  }
	}
	else {
	  index.i = token_i;
	  index = Prog( index ); // prog 
	  
	  if ( index.result == false ) {
	  	index.i = token_i;
	  	index = Declarations( index ); // declarations 
	  	
	    if ( index.result == false ) {
	  	  index.i = token_i;
	  	  index = Stmts( index ); // stmts 
	    }
	  }
	}
  }
  
  return index;
}

ReturnResult Prog( ReturnResult index ) {
  int token_i = FindToken( index.i );
  string str;
  if ( token_i != -1 ) {
  	str = glob_input[token_i].token;

    for ( int i = 0; i < str.size(); i++ ) {
  	  str[i] = toupper( str[i] );
    }
  }
  
  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else if ( str == "PROGRAM" ) {
  	index.type = "PROGRAM";
  	index.i = token_i;
    token_i = FindToken( token_i + 1 );
    
    if ( token_i != -1 ) {
      index.i = token_i;
      index = ID( 2, index ); // id 

      if ( index.result == true ) {
        token_i = FindToken( index.i + 1 );

        if ( token_i != -1 ) {
          if ( glob_input[token_i].token == ";" ) {
            index.i = token_i;
            index.result = true;
		  }
		  else {
		    index.result = false;
		  }
		}
		else {
		  index.error = 1;
		  index.result = false;
		}
	  }
	}
	else {
	  index.error = 1;
	  index.result = false;	
	}
  }
  else if ( glob_input[token_i].token == "ENP" || glob_input[token_i].token == "ENS" ) {
  	index.i = token_i;
  	token_i = FindToken( token_i + 1 );

    if ( token_i != -1 ) {
      if ( glob_input[token_i].token == ";" ) {
        index.i = token_i;
        index.result = true;
	  }
	  else {
		index.result = false;
	  }
	}
	else {
	  index.error = 1;
	  index.result = false;	
	}
  }
  else {
	index.result = false;
  } 
  
  return index;
}

ReturnResult ID( int mode, ReturnResult index ) { // identifier 
  int token_i = FindToken( index.i );
  AddTableType tmp;

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else if ( mode == 0 ) { // id ? 
    index.mode = 0;
  	if ( Is_IDENT( glob_input[token_i].token ) == true ) {
  	  index.i = token_i;
  	  index.result = true;
//  	  if ( index.pass == 2 ) { // ***add to table 
//  	    tmp = ADD_to_ID_Table( glob_input[token_i].token, index.subroutine, token_i, type );
//  	    if ( tmp.result == false ) {
//  	      index.error = 2;
//		}
//	  }
	}
	else {
	  index.result = false;	
	}
  }
  else if ( mode == 1 ) { // label_id ? 
    index.mode = 1;
  	if ( Is_IDENT( glob_input[token_i].token ) == true ) {
  	  if ( ( int ) glob_input[token_i].token[0] == 76 ) { // 開頭為L 
  	    index.result = true;
  	    int i = 1;
  	    while ( index.result == true && i != glob_input[token_i].token.size() ) {
  	      if ( ( int ) glob_input[token_i].token[i] <= 57 && ( int ) glob_input[token_i].token[i] >= 48 ) {
  	      	index.result = true;
		  }
		  else {
		  	index.result = false;
		  }
		  i++;
		}
		
		if ( index.result == true ) {
		  index.i = token_i;
//		  if ( index.pass == 2 ) { // ***add to table 
//  	        tmp = ADD_to_ID_Table( glob_input[token_i].token, index.subroutine, token_i, type );
//  	        if ( tmp.result == false ) {
//  	          index.error = 2;
//		    }
//	      }
		}
	  }
	  else {
	  	index.result = false;
	  }
	}
	else {
	  index.result = false;	
	}
  }
  else if ( mode == 2 ) { // subroutine, program 
    index.mode = 2;
  	if ( Is_IDENT( glob_input[token_i].token ) == true ) {
  	  index.i = token_i;
  	  index.result = true;
//  	  if ( index.pass == 2 ) { // ***add to table 
//  	    tmp = ADD_to_ID_Table( glob_input[token_i].token, -1, token_i, type );
//  	    if ( tmp.result == false ) {
//  	      index.error = 2;
//		}
//		else {
//		  index.subroutine = tmp.newsubr;
//		}
//	  }
	}
	else {
	  index.result = false;	
	}
  }
 
  return index;
}

ReturnResult Declarations( ReturnResult index ) { // declarations
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	index.i = token_i;
  	index = Var( index ); // var 
  	
  	if ( index.result == false ) {
  	  index.i = token_i;
  	  index = Dim( index ); // dimension
  	  
  	  if ( index.result == false ) {
  	    index.i = token_i;
  	    index = Label( index ); // label 

  	    if ( index.result == false ) {
  	      index.i = token_i;
  	      index = Sub( index ); // subroutine 
	    }
	  }
	}
  }
 
  return index;
}

ReturnResult Var( ReturnResult index ) { // var 
  int token_i = FindToken( index.i );
  string str, type;
  if ( token_i != -1 ) {
  	str = glob_input[token_i].token;
  	
    for ( int i = 0; i < str.size(); i++ ) {
  	  str[i] = toupper( str[i] );
    }
  }

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	if ( str == "VARIABLE" ) {
  	  index.i = token_i;
  	  token_i = FindToken( token_i + 1 );

  	  if ( Find_type( glob_input[token_i].token ) == true ) {
  	  	type = glob_input[token_i].token;
  	  	index.type = type;
  	    index.i = token_i;
  	    token_i = FindToken( token_i + 1 );
  	  
  	    if ( token_i == -1 ) { // input不足  
          index.error = 1;
          index.result = false;
	    }
	    else {
	  	  if ( glob_input[token_i].token == ":" ) {
	  	    index.i = token_i;
	  	    token_i = FindToken( token_i + 1 );

  	        if ( token_i == -1 ) { // input不足  
              index.error = 1;
              index.result = false;
	        }
	        else {
	  	      index.i = token_i;
              index = ID( 0, index );
        
              if ( index.result == true ) {
                token_i = FindToken( index.i + 1 );

  	            if ( token_i == -1 ) { // input不足  
                  index.error = 1;
                  index.result = false;
	            }
	            else {
	  	          index.i = token_i;
                  index = Vars( index );
		        }
		      }
		      else {
		        index.result = false;	
		      }
		    }
		  }
		  else {
		    index.result = false;	
		  }
	    }
	  }
	  else {
	    index.result = false;
	  }
  		
	}
	else {
	  index.result = false;
	}
  } 
   
  return index;	
}

ReturnResult Vars( ReturnResult index ) { // vars 
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else if ( glob_input[token_i].token == ";" ) {
    index.i = token_i;
    index.result = true;
  }
  else if ( glob_input[token_i].token == "," ) {
    index.i = token_i;
    token_i = FindToken( token_i + 1 );

    if ( token_i == -1 ) { // input不足  
      index.error = 1;
      index.result = false;
    }
    else {
      index.i = token_i;
      index = ID( 0, index );
      
      if ( index.result == true ) {
        token_i = FindToken( index.i + 1 );

        if ( token_i == -1 ) { // input不足  
          index.error = 1;
          index.result = false;
        }
        else {
          index.i = token_i;
          index = Vars( index );
		}
	  }
	}
  }
  else {
	index.result = false;
  }
  
  return index;
}

ReturnResult Var_sub1( ReturnResult index ) { // var_sub1 
  int token_i = FindToken( index.i );
  string type;

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	if ( Find_type( glob_input[token_i].token ) == true ) {
  	  type = glob_input[token_i].token;
  	  index.type = type;
  	  index.i = token_i;
  	  token_i = FindToken( token_i + 1 );
  	  
  	  if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
	  }
	  else {
	  	if ( glob_input[token_i].token == ":" ) {
	  	  index.i = token_i;
	  	  token_i = FindToken( token_i + 1 );

  	      if ( token_i == -1 ) { // input不足  
            index.error = 1;
            index.result = false;
	      }
	      else {
	  	    index.i = token_i;
            index = Var_sub2( index );
		  }
		}
		else {
		  index.result = false;	
		}
	  }
	}
	else {
	  index.result = false;
	}
  } 
   
  return index;	
}

ReturnResult Var_sub2( ReturnResult index ) { // var_sub2 
  int token_i = FindToken( index.i );
  int save_i;

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else if ( glob_input[token_i].token == "," ) {
    index.i = token_i;
    token_i = FindToken( token_i + 1 );

    if ( token_i == -1 ) { // input不足  
      index.error = 1;
      index.result = false;
    }
    else {
      index.i = token_i;
      index = ID( 0, index );
      
      if ( index.result == true ) {
      	save_i = index.i;
        token_i = FindToken( index.i + 1 );

        if ( token_i == -1 ) { // input不足  
          index.error = 1;
          index.result = false;
        }
        else {
          index.i = token_i;
          index = Var_sub2( index );

          if ( index.result == false ) {
            index.i = save_i;
            index.result = true;
		  }
		}
	  }
	}
  }
  else {
    index.i = token_i;
    index = ID( 0, index );
    
    if ( index.result == true ) {
      save_i = index.i;
      token_i = FindToken( index.i + 1 );
      
      if ( token_i != -1 ) { // input不足  
        index.i = token_i;
        index = Var_sub2( index );
        
        if ( index.result == false ) {
          index.i = save_i;
          index.result = true;
		}
      }
	}
  }
  
  return index;
}

ReturnResult Dim( ReturnResult index ) { // dimension 
  int token_i = FindToken( index.i );
  string str;
  if ( token_i != -1 ) {
  	str = glob_input[token_i].token;

    for ( int i = 0; i < str.size(); i++ ) {
  	  str[i] = toupper( str[i] );
    }
  }

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
    if ( str == "DIMENSION" ) {
      index.type = "DIMENSION";
      index.i = token_i;
      token_i = FindToken( token_i + 1 );
      
      if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
      }
      else {
        if ( Find_type( glob_input[token_i].token ) == true ) {
          index.i = token_i;
          token_i = FindToken( token_i + 1 );

          if ( token_i == -1 ) { // input不足  
            index.error = 1;
            index.result = false;
          }
          else {
            if ( glob_input[token_i].token == ":" ) {
               index.i = token_i;
               token_i = FindToken( token_i + 1 );
               
              if ( token_i == -1 ) { // input不足  
                index.error = 1;
                index.result = false;
              }
              else {
              	index.i = token_i;
              	index = Dims( index );
              	
              	if ( index.result == true ) {
              	  token_i = FindToken( index.i + 1 );

              	  if ( token_i == -1 ) { // input不足  
                    index.error = 1;
                    index.result = false;
                  }
                  else {
                    if ( glob_input[token_i].token == ";" ) {
                      index.i = token_i;
                      index.result = true;
					}
					else {
					  index.result = false;	
					}
				  }
			    }
			  }
			}
			else {
			  index.result = false;
			} 
		  }
		}
		else {
		  index.result = false;
		}
	  }
	}
	else {
	  index.result = false;
	}
  } 
  
  return index;
}

ReturnResult Dims( ReturnResult index ) { // dimension 
  int token_i = FindToken( index.i );
  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	index.i = token_i;
  	index = ID( 0, index );
  	
  	if ( index.result == true ) {
  	  token_i = FindToken( index.i + 1 );

  	  if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
      }
      else {
        if ( glob_input[token_i].token == "(" ) {
          index.i = token_i;
		  token_i = FindToken( token_i + 1 );

          if ( token_i == -1 ) { // input不足  
            index.error = 1;
            index.result = false;
          }
          else {
          	index.i = token_i;
          	index = Nums( index );
          	
          	if ( index.result == true ) {
          	  token_i = FindToken( index.i + 1 );

              if ( token_i == -1 ) { // input不足  
                index.error = 1;
                index.result = false;
              }
              else {
              	if ( glob_input[token_i].token == ")" ) {
              	  index.result = true;
                  index.i = token_i;
                  token_i = FindToken( token_i + 1 );
                  
                  if ( token_i != -1 ) {
                    if ( glob_input[token_i].token == "," ) {
                      index.i = token_i;
                      token_i = FindToken( token_i + 1 );

                      if ( token_i == -1 ) { // input不足  
                        index.error = 1;
                        index.result = false;
                      }
                      else {
                        index.i = token_i;
                        index = Dims( index );
					  }
				    }
				    else {
				  	  index.result = true; // 沒有更多了  
				    } 
				  } 
              	}
              	else {
              	  index.result = false;	
				}
			  }
			}
			else {
			  index.i = token_i;
          	  index = ID( 0, index );

          	  if ( index.result == true ) {
          	    token_i = FindToken( index.i + 1 );

                if ( token_i == -1 ) { // input不足  
                  index.error = 1;
                  index.result = false;
                }
                else {
              	  if ( glob_input[token_i].token == ")" ) {
              	    index.result = true;
                    index.i = token_i;
                    token_i = FindToken( token_i + 1 );
                  
                    if ( token_i != -1 ) {
                      if ( glob_input[token_i].token == "," ) {
                        index.i = token_i;
                        token_i = FindToken( token_i + 1 );

                        if ( token_i == -1 ) { // input不足  
                          index.error = 1;
                          index.result = false;
                        }
                        else {
                          index.i = token_i;
                          index = Dims( index );
					    }
				      }
				      else {
				  	    index.result = true; // 沒有更多了  
				      } 
				    } 
              	  }
              	  else {
              	    index.result = false;	
				  }
			    }
			  }
			}
		  }
		}
		else {
		  index.result = false;
		}
	  }
	}
  }
  
  return index;
} 

ReturnResult Nums( ReturnResult index ) { // numbers 
  int token_i = FindToken( index.i );
  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else if ( glob_input[token_i].type == "integer" || glob_input[token_i].type == "float" )  {
  	index.i = token_i;
  	index.result = true;
	
	token_i = FindToken( token_i + 1 );
	
	if ( token_i != -1 ) {
	  if ( glob_input[token_i].token == "," ) {	
	    index.i = token_i;
	    token_i = FindToken( token_i + 1 );
	    
	    if ( token_i == -1 ) { // input不足  
          index.error = 1;
          index.result = false;
        }
        else {
          index.i = token_i;
          index = Nums( index );
		}
	  }
	}
  }
  else {
  	index.result = false;
  }

  return index;
} 

ReturnResult Label( ReturnResult index ) { // label 
  int token_i = FindToken( index.i );
  string str;
  if ( token_i != -1 ) {
  	str = glob_input[token_i].token;
    for ( int i = 0; i < str.size(); i++ ) {
  	  str[i] = toupper( str[i] );
    }
  }

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	if ( str == "LABEL" ) {
  	  index.type = "LABEL";
  	  index.i = token_i;
  	  token_i = FindToken( token_i + 1 );
  	  
  	  if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
      }
      else {
        index.i = token_i;
        index = Labels( index );
	  }
	}
	else {
	  index.result = false;
	}
  } 
  
  return index;
} 

ReturnResult Labels( ReturnResult index ) { // label 
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	index.i = token_i;
  	index = ID( 1, index );
  	
  	if ( index.result == true ) {
  	  token_i =  FindToken( index.i + 1 );
  	
  	  if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
      }
      else {
      	if ( glob_input[token_i].token == "," ) {
      	  index.i = token_i;
      	  token_i = FindToken( token_i + 1 );

          if ( token_i == -1 ) { // input不足  
            index.error = 1;
            index.result = false;
          }
          else {
          	index.i = token_i;
          	index = Labels( index );
		  }
		}
		else if ( glob_input[token_i].token == ";" ) {
		  index.i = token_i;
		  index.result = true;
		}
		else {
		  index.result = false;
		}
	  }
	}
  }
  
  return index;
} 

ReturnResult Sub( ReturnResult index ) { // subroutine 
  int token_i = FindToken( index.i );
  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else if ( glob_input[token_i].token == "SUBROUTINE" ) {
    index.i = token_i;
    token_i = FindToken( token_i + 1 );
    
    if ( token_i == -1 ) { // input不足  
      index.error = 1;
      index.result = false;
    }
    else {
      index.i = token_i;
      index = Subs( index );
	}
	index.type = "SUBROUTINE";
  }
  else {
  	index.result = false;
  }
  
  return index;
} 

ReturnResult Subs( ReturnResult index ) { // subroutine 
  int token_i = FindToken( index.i );
  int save_i;

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	index.i = token_i;
  	index = ID( 2, index );
  	
  	if ( index.result == true ) {
  	  token_i = FindToken( index.i + 1 );
  	  
      if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
      }
      else {
        if ( glob_input[token_i].token == "(" ) {
          index.i = token_i;
          token_i = FindToken( token_i + 1 );
          
          if ( token_i == -1 ) { // input不足  
            index.error = 1;
            index.result = false;
          }
          else {
          	if ( glob_input[token_i].token == ")" ) {
          	  index.i = token_i;
          	  token_i = FindToken( token_i + 1 );

              if ( token_i == -1 ) { // input不足  
                index.error = 1;
                index.result = false;
              }
              else if ( glob_input[token_i].token == ";" ) {
              	index.i = token_i;
              	index.result = true;
			  }
			  else if ( glob_input[token_i].token == "," ) {
			  	index.i = token_i;
			  	token_i = FindToken( token_i + 1 );
			  	
                if ( token_i == -1 ) { // input不足  
                  index.error = 1;
                  index.result = false;
                }
                else {
                  index.i = token_i;
                  index = Subs( index );
				}
			  }
			  else {
			  	index.result = false;
			  }
			}
			else {
			  index.i = token_i;			  
			  index = Var_sub1( index );
			  
			  if ( index.result == true ) {
			  	token_i = FindToken( index.i + 1 );
			  	
                if ( token_i == -1 ) { // input不足  
                  index.error = 1;
                  index.result = false;
                }
                else if ( glob_input[token_i].token == ")" ) {
          	      index.i = token_i;
          	      token_i = FindToken( token_i + 1 );

                  if ( token_i == -1 ) { // input不足  
                    index.error = 1;
                    index.result = false;
                  }
                  else if ( glob_input[token_i].token == ";" ) {
              	    index.i = token_i;
              	    index.result = true;
			      }
			      else if ( glob_input[token_i].token == "," ) {
			  	    index.i = token_i;
			  	    token_i = FindToken( token_i + 1 );
			  	
                    if ( token_i == -1 ) { // input不足  
                      index.error = 1;
                      index.result = false;
                    }
                    else {
                      index.i = token_i;
                      index = Subs( index );
				    }
			      }
			      else {
			  	    index.result = false;
			      }
			    }
			    else {
			      index.result = false;
			    }
			  }
			}
		  }
		}
		else {
		  index.result = false;
		}
	  }
	}
  }
  
  return index;
} 

ReturnResult Stmts( ReturnResult index ) { // stmts 
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
    index.i = token_i;
    index = Call( index );
    
    if ( index.result == false ) {
      index.i = token_i;
      index = If( index );
      
      if ( index.result == false ) {
        index.i = token_i;
        index = Goto( index );
        
        if ( index.result == false ) {
          index.i = token_i;
          index = Input( index );
          
          if ( index.result == false ) {
            index.i = token_i;
            index = Output( index );
            
            if ( index.result == false ) {
              index.i = token_i;
              index = Assign( index );

              if ( index.result == false ) {
                index.i = token_i;
                index = Exp( index );
	          }
	        }
	      }
	    }
	  }
	}
	
	if ( index.result == true ) {
	  token_i = FindToken( index.i + 1 );

      if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
      }
      else if ( glob_input[token_i].token == ";" ) {
      	index.i = token_i;
      	index.result = true;
	  }
	  else {
	  	index.result = false;
	  }
	}
  }
 
  return index;	
}

ReturnResult Exp( ReturnResult index ) {
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	index.i = token_i;
  	index = Q( index );
  	
  	if ( index.result == true ) {
  	  token_i = FindToken( index.i + 1 );
  	  
  	  if ( token_i != -1 ) {
  	  	if ( Is_operator( glob_input[token_i].token ) == true ) {
  	  	  index.i = token_i;
  	  	  token_i = FindToken( token_i + 1 );
  	  	  
  	  	  if ( token_i == -1 ) {
  	  	  	index.error = 1;
  	  	  	index.result = false;
		  }
		  else {
		  	index.i = token_i;
		  	index = Exp( index );
		  }
		}
	  }
	}
  }

  return index;	
}

ReturnResult Q( ReturnResult index ) {
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	index.i = token_i;
  	index = T( index );
  	
  	if ( index.result == true ) {
  	  token_i = FindToken( index.i + 1 );
  	  
  	  if ( token_i != -1 ) {
  	  	if ( glob_input[token_i].token == "+" ) {
  	  	  index.i = token_i;
  	  	  token_i = FindToken( token_i + 1 );
  	  	  
  	  	  if ( token_i == -1 ) {
  	  	  	index.error = 1;
  	  	  	index.result = false;
		  }
		  else {
		  	index.i = token_i;
		  	index = Q( index );
		  }
		}
		else if ( glob_input[token_i].token == "-" ) {
  	  	  index.i = token_i;
  	  	  token_i = FindToken( token_i + 1 );
  	  	  
  	  	  if ( token_i == -1 ) {
  	  	  	index.error = 1;
  	  	  	index.result = false;
		  }
		  else {
		  	index.i = token_i;
		  	index = Q( index );
		  }
		}
	  }
	}
  }

  return index;		
}

ReturnResult T( ReturnResult index ) {
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	index.i = token_i;
  	index = F( index );
  	
  	if ( index.result == true ) {
  	  token_i = FindToken( index.i + 1 );
  	  
  	  if ( token_i != -1 ) {
  	  	if ( glob_input[token_i].token == "*" ) {
  	  	  index.i = token_i;
  	  	  token_i = FindToken( token_i + 1 );
  	  	  
  	  	  if ( token_i == -1 ) {
  	  	  	index.error = 1;
  	  	  	index.result = false;
		  }
		  else {
		  	index.i = token_i;
		  	index = T( index );
		  }
		}
		else if ( glob_input[token_i].token == "/" ) {
  	  	  index.i = token_i;
  	  	  token_i = FindToken( token_i + 1 );
  	  	  
  	  	  if ( token_i == -1 ) {
  	  	  	index.error = 1;
  	  	  	index.result = false;
		  }
		  else {
		  	index.i = token_i;
		  	index = T( index );
		  }
		}
	  }
	}
  }

  return index;	
}

ReturnResult F( ReturnResult index ) {
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	index.i = token_i;
  	index = E( index );
  	
  	if ( index.result == true ) {
  	  token_i = FindToken( index.i + 1 );
  	  
  	  if ( token_i != -1 ) {
  	  	if ( glob_input[token_i].token == "↑" ) {
  	  	  index.i = token_i;
  	  	  token_i = FindToken( token_i + 1 );
  	  	  
  	  	  if ( token_i == -1 ) {
  	  	  	index.error = 1;
  	  	  	index.result = false;
		  }
		  else {
		  	index.i = token_i;
		  	index = F( index );
		  }
		}
	  }
	}
  }

  return index;	
}

ReturnResult E( ReturnResult index ) {
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	// index.type = "EXP";
    index.i = token_i;
    index = ID( 0, index ); // 先當"-1" 太麻煩  
    
    if ( index.result == true ) {
      token_i = FindToken( index.i + 1 );

      if ( token_i != -1 && glob_input[token_i].token == "(" ) {
      	index.i = token_i;
      	token_i = FindToken( token_i + 1 );
      	
      	if ( token_i == -1 ) {
          index.error = 1;
          index.result = false;
		}
		else {
      	  index.i = token_i;
      	  index = Nums( index );
      	  
		  if ( index.result == true ) {
			token_i = FindToken( index.i + 1 );
			
			if ( token_i != -1 && glob_input[token_i].token == ")" ) {
			  index.i = token_i;
			  index.result = true;
			}
			else {
			  index.result = false;
			}
		  }
		  else {
      	    index.i = token_i;
      	    index = Var_sub2( index ); // 先當"-1" 太麻煩  
      	    
		    if ( index.result == true ) {
			  token_i = FindToken( index.i + 1 );
			
			  if ( token_i != -1 && glob_input[token_i].token == ")" ) {
			    index.i = token_i;
			    index.result = true;
			  }
			  else {
			    index.result = false;
			  }
		    }
		  }
		}
	  }
	}
	else {
	  index.i = token_i;
	  if ( glob_input[token_i].type == "integer" || glob_input[token_i].type == "float" )  {
        index.i = token_i;
        index.result = true;
      }
      else if ( glob_input[token_i].token == "(" ) {
      	index.i = token_i;
      	token_i = FindToken( token_i + 1 );
      	
      	if ( token_i == -1 ) {
          index.error = 1;
          index.result = false;
		}
		else {
      	  index.i = token_i;
      	  index = Exp( index );
      	  
		  if ( index.result == true ) {
			token_i = FindToken( index.i + 1 );
			
			if ( token_i != -1 && glob_input[token_i].token == ")" ) {
			  index.i = token_i;
			  index.result = true;
			}
			else {
			  index.result = false;
			}
		  }
		}
	  }
	  else {
	  	index.result = false;
	  }
	}
  }

  return index;	
}

ReturnResult Assign( ReturnResult index ) {
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	index.i = token_i;
  	index = Dims( index );
  	
  	if ( index.result == true ) {
  	  token_i = FindToken( index.i + 1 );
  	  
      if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
      }
      else if ( glob_input[token_i].token == "=" ) {
      	index.i = token_i;
      	token_i = FindToken( token_i + 1 );
      	
      	if ( token_i == -1 ) {
          index.error = 1;
          index.result = false;
		}
		else {
		  index.i = token_i;
		  index = Exp( index );
		  
		  if ( index.result == true ) {
		  	token_i = FindToken( index.i + 1 );
		  	
		  	if ( token_i != -1 ) {
              if ( glob_input[token_i].token == "," ) {
              	index.i = token_i;
              	token_i = FindToken( token_i + 1 );
              	
              	if ( token_i == -1 ) {
                  index.error = 1;
                  index.result = false;
		        }
		        else {
		          index.i = token_i;
		          index = Assign( index );
				}
			  } 
		    }
		  }
		}
	  }
	  else {
	  	index.result = false;
	  }
	}
	else {
	  index.i = token_i;
	  index = ID( 0, index ); // 先當"-1" 太麻煩  
	  
	  if ( index.result == true ) {
  	    token_i = FindToken( index.i + 1 );
  	  
        if ( token_i == -1 ) { // input不足  
          index.error = 1;
          index.result = false;
        }
        else if ( glob_input[token_i].token == "=" ) {
      	  index.i = token_i;
      	  token_i = FindToken( token_i + 1 );
      	
      	  if ( token_i == -1 ) {
            index.error = 1;
            index.result = false;
		  }
		  else {
		    index.i = token_i;
		    index = Exp( index );
		  
		    if ( index.result == true ) {
		  	  token_i = FindToken( index.i + 1 );
		  	
		  	  if ( token_i != -1 ) {
                if ( glob_input[token_i].token == "," ) {
              	  index.i = token_i;
              	  token_i = FindToken( token_i + 1 );
              	
              	  if ( token_i == -1 ) {
                    index.error = 1;
                    index.result = false;
		          }
		          else {
		            index.i = token_i;
		            index = Assign( index );
				  }
			    }
		      }
		    }
		  }
	    }
	    else {
	  	  index.result = false;
	    }
	  }
	}
  }
  
  if ( index.result == true ) {
  	index.type = "ASSIGN";
  }

  return index;		
}

ReturnResult Output( ReturnResult index ) {
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }

  return index;		
}

ReturnResult Input( ReturnResult index ) {
  int token_i = FindToken( index.i );

  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }

  return index;		
}

ReturnResult Goto( ReturnResult index ) {
  int token_i = FindToken( index.i );
  string str;

  if ( token_i == 91 ) {
  	token_i = 91;
  }

  if ( token_i != -1 ) {
  	str = glob_input[token_i].token;

    for ( int i = 0; i < str.size(); i++ ) {
  	  str[i] = toupper( str[i] );
    }
  }
  
  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	if ( str == "GTO" ) {
  	  index.i = token_i;
  	  token_i = FindToken( token_i + 1 );
  	  
      if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
      }
      else {
        index.i = token_i;
        index = ID( 1, index );
	  }
	}
	else {
	  index.result = false;
	}
  }

  return index;		
}

ReturnResult If( ReturnResult index ) {
  int token_i = FindToken( index.i );
  string str;
  
  if ( token_i != -1 ) {
  	str = glob_input[token_i].token;

    for ( int i = 0; i < str.size(); i++ ) {
  	  str[i] = toupper( str[i] );
    }
  }
  
  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	if ( str == "IF" ) {
  	  index.i = token_i;
  	  token_i = FindToken( token_i + 1 );
  	  
      if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
      }
      else {
      	index.i = token_i;
      	index = Exp( index );
      	
      	if ( index.result == true ) {
          token_i = FindToken( index.i + 1 );
                  
		  if ( token_i == -1 ) { // input不足  
            index.error = 1;
            index.result = false;
          }
          else {
			str = glob_input[token_i].token;
            for ( int i = 0; i < str.size(); i++ ) {
  	          str[i] = toupper( str[i] );
            }
              
            if ( str == "THEN" ) {
              index.i = token_i;
              token_i = FindToken( token_i + 1 );
                
			  if ( token_i == -1 ) { // input不足  
                index.error = 1;
                index.result = false;
              }
              else {
                index.i = token_i;
                index = Goto( index );
                  
                if ( index.result == true ) {
                  token_i = FindToken( index.i + 1 );
                    
				  if ( token_i == -1 ) { // input不足  
                    index.error = 1;
                    index.result = false;
                  }
                  else if ( glob_input[token_i].token == "ELSE" ) {
                    index.i = token_i;
                    token_i = FindToken( token_i + 1 );
                      
                    if ( token_i == -1 ) { // input不足  
                      index.error = 1;
                      index.result = false;
					}
					else {
					  index.i = token_i;
					  index = Assign( index );
					}
				  }
				  else {
					index.result = false;
				  }
			    }
				else {
				  index.i = token_i;
				  index = Assign( index );
				          
                  if ( index.result == true ) {
                    token_i = FindToken( index.i + 1 );
                    
				    if ( token_i == -1 ) { // input不足  
                      index.error = 1;
                      index.result = false;
                    }
                    else if ( glob_input[token_i].token == "ELSE" ) {
                      index.i = token_i;
                      token_i = FindToken( token_i + 1 );
                      
                      if ( token_i == -1 ) { // input不足  
                        index.error = 1;
                        index.result = false;
					  }
					  else {
					  	index.i = token_i;
					  	index = Assign( index );
					  }
					}
					else {
					  index.result = false;
					}
				  }
				}
			  }
			}
			else {
			  index.result = false;
			}	
		  }
//			else { IF後直接THEN  
//			  str = glob_input[token_i].token;
//              for ( int i = 0; i < str.size(); i++ ) {
//  	            str[i] = toupper( str[i] );
//              }
//              
//              if ( str == "THEN" ) {
//                index.i = token_i;
//                token_i = FindToken( token_i + 1 );
//                
//				if ( token_i == -1 ) { // input不足  
//                  index.error = 1;
//                  index.result = false;
//                }
//                else {
//                  index.i = token_i;
//                  index = Goto( index );
//                  
//                  if ( index.result == true ) {
//                    token_i = FindToken( index.i + 1 );
//                    
//				    if ( token_i == -1 ) { // input不足  
//                      index.error = 1;
//                      index.result = false;
//                    }
//                    else if ( glob_input[token_i].token == "ELSE" ) {
//                      index.i = token_i;
//                      token_i = FindToken( token_i + 1 );
//                      
//                      if ( token_i == -1 ) { // input不足  
//                        index.error = 1;
//                        index.result = false;
//					  }
//					  else {
//					  	index.i = token_i;
//					  	index = Assign( index );
//					  }
//					}
//					else {
//					  index.result = false;
//					}
//				  }
//				}
//			  }
//			  else {
//			  	index.result = false;
//			  }
//			}
		}
	  }
  	}
  	else {
  	  index.result = false;
	}
  }
  
  if ( index.result == true ) {
  	index.type = "IF";
  } 

  return index;		
}

ReturnResult Call( ReturnResult index ) {
  int token_i = FindToken( index.i );
  string str;

  if ( token_i != -1 ) {
  	str = glob_input[token_i].token;

    for ( int i = 0; i < str.size(); i++ ) {
  	  str[i] = toupper( str[i] );
    }
  }
  
  if ( token_i == -1 ) { // input不足  
    index.error = 1;
    index.result = false;
  }
  else {
  	if ( str == "CALL" ) {
  	  index.type = "CALL";
  	  index.i = token_i;
  	  token_i = FindToken( token_i + 1 );
  	  
      if ( token_i == -1 ) { // input不足  
        index.error = 1;
        index.result = false;
      }
      else {
      	index.i = token_i;
      	index = ID( 0, index );
      	
      	if ( index.result == true ) {
      	  token_i = FindToken( index.i + 1 );
      	  
      	  if ( token_i == -1 ) { // input不足  
            index.error = 1;
            index.result = false;
		  }
      	  else if ( glob_input[token_i].token == "(" ) {
      	    index.i = token_i;
      	    token_i = FindToken( token_i + 1 );
      	    
      	    if ( token_i == -1 ) { // input不足  
              index.error = 1;
              index.result = false;
			}
			else {
			  index.i = token_i;
			  index = Var_sub2( index );
			  
			  if ( index.result == true ) {
			    token_i = FindToken( index.i + 1 );
			    
      	        if ( token_i == -1 ) { // input不足  
                  index.error = 1;
                  index.result = false;
			    }
			    else if ( glob_input[token_i].token == ")" ) {
			      index.i = token_i;
			      index.result = true;
			    }
			    else {
			      index.result = false;
				}
			  }
			}
		  }
		  else {
		    index.result = false;
		  }
		}
	  }
	}
	else {
	  index.result = false;
	}
  }

  return index;		
}

int main() {
  string filename;
  int ascii_sum, index;
  
  Set_vector_100();
  Readfile( "Table1", glob_table1, ".table" );
  Readfile( "Table2", glob_table2, ".table" );
  glob_table7.push_back( -1 ); // 從1開始算 
  glob_table1.push_back(",");
  glob_table1.push_back(".");
  QuadType tmp;
  tmp.instruction = "";
  tmp.quadratic = "";
  glob_quad.push_back( tmp );
//  Print( glob_table1 );
//  Print( glob_table2 );

  cout << "input file name: ";
  cin >> filename;
  cout << endl; 
  
  Scanner( filename, ".txt" );
  
  Lexical_Analysis();

//  for ( int i = 0; i < glob_input.size(); i++ ) {
//  	if ( glob_input[i].type != "whitespace" ) {
//  	  cout << "token: " << glob_input[i].token << " type: " << glob_input[i].type << endl;
//	}
//  }
  
  for( int i = 0; i < glob_input.size(); i++ ) {
    if ( glob_input[i].type == "integer" || glob_input[i].type == "float" || glob_input[i].type == "string" ) {
      ascii_sum = Cal_ASCII( glob_input[i].token , glob_input[i].type );
      index = ascii_sum % 100 ;
      Hash_Func( index, glob_input[i].type, i, -1, glob_input[i].type);
	}
  }

  ReturnResult result = Parser();
  // for ( int j = 0; j < glob_table7.size(); j++ ) {
  //   cout << "glob_table7[j]: " << glob_table7[j] << endl;
  // }
  Parser_2nd_pass();
  Write_file( filename, result );
  
//  for ( int i = 0; i < glob_table5.size(); i++ ) {
//  	cout << "id: " << glob_table5[i].id << " subroutine: " << glob_table5[i].subroutine << " type: " << glob_table5[i].type << " pointer: " << glob_table5[i].pointer << endl;
//  }
  
  return 0;
} // main() 
