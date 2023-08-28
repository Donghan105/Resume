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

struct ReturnResult{
  int i;
  bool result;
  int error;
}; // Recursive-Decent-Parsing時return的type 

struct InputType{ 
  int tablenum;
  int column;
  string token;
  string type;
}; // 使用者輸入的code 

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
void Hash_Func( int index, string type );
void Write_file(string filename);
int NextToken( int i );

vector <string> glob_table1;
vector <string> glob_table2;
vector <string> glob_table3;
vector <string> glob_table4;
vector <string> glob_table5; // symbol
vector <string> glob_table6; // integer, float
vector <string> glob_table7; // string
vector <InputType> glob_input; // 使用者輸入的code  



void Readfile( string filename, vector<string>& tmp, string ext ) { // ext:副檔名  
  ifstream infile;
  filename = filename + ext;
  string str;
  infile.open(filename.c_str());	//.c_str()用來將string 轉成 char[XXX], 因為open函數只能讀char[XXX], 但string撰寫比較方便
	
  if ( infile == NULL ) { cout << "Fail to open " << filename << endl; } //NULL為"空"的意思 
  else {
    cout << "完成開啟" << filename << endl; 
	while( infile >> str!= 0 ){	//while( infile >> ch)	//當檔案中還有東西可以讀取, >>表示從檔案將資料傳到變數中 
	  if(str==" "){
				//cout << "空白" << endl; 
	  }
	  else if(str=="\n"){
		//cout << "\\n" << endl; 
	  }
	  else if(str=="\t"){
		//cout << "\\t" << endl; 
	  }
	  else {
        tmp.push_back(str);
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
          element.type = "token";
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
          element.type = "token";
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
  Find_Token();
}

void Find_Token() {
  string str;
  for ( int i = 0; i < glob_input.size(); i++ ) {
    bool result = false;
    str = glob_input[i].token;
    for ( int j = 0; j < str.size(); j++) {
      str[j] = tolower( str[j] ) ; 
	}
	
    for ( int j = 0; j < glob_table1.size() && result == false; j++ ) {
      if ( str == glob_table1[j] ) {
        glob_input[i].tablenum = 1;
        glob_input[i].column = j + 1;
        glob_input[i].type = "token";
        result = true;
        j = glob_table1.size();
      }
    }

    for ( int j = 0; j < str.size(); j++) {
      str[j] = toupper( str[j] ) ; 
	}

    for ( int j = 0; j < glob_table2.size() && result == false; j++ ) {
      if ( str == glob_table2[j] ) {
        glob_input[i].tablenum = 2;
        glob_input[i].column = j + 1;
        glob_input[i].type = "token";
        result = true;
        j = glob_table2.size();
      }
  	}

    for ( int j = 0; j < glob_table3.size() && result == false; j++ ) {
      if ( str == glob_table3[j] ) {
        glob_input[i].tablenum = 3;
        glob_input[i].column = j + 1;
        glob_input[i].type = "token";
        result = true;
        j = glob_table3.size();
      }
    }

    for ( int j = 0; j < glob_table4.size() && result == false; j++ ) {
      if ( str == glob_table4[j] ) {
        glob_input[i].tablenum = 4;
        glob_input[i].column = j + 1;
        glob_input[i].type = "token";
        result = true;
        j = glob_table4.size();
      }
    }
  }
}


bool Is_Symbol( string str ) { // 切token時 所需要辨識出的Symbol 
  bool result = false;

  for ( int i = 0; i < glob_table4.size(); i++ ) {
    if ( str == glob_table4[i] ) {
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

  for ( int i = 0; i < 100; i++ ) {
  	glob_table5.push_back( tmp );
  	glob_table6.push_back( tmp );
  	glob_table7.push_back( tmp );
  }
}

void Hash_Func( int index, string type, int i ) {
  int save;
  
  
  if ( type == "integer" || type == "float" ) {
  	if ( glob_table6[index] == "" ) {
  		glob_table6[index] = glob_input[i].token;
  		glob_input[i].tablenum = 6;
  		glob_input[i].column = index;
	}
	else {
	  save = index;

	  if ( glob_table6[index] == glob_input[i].token ) {
  		glob_input[i].tablenum = 6;
  		glob_input[i].column = index;
	  } 

	  while( glob_table6[index] != glob_input[i].token ) {
	  	index++;
        if ( glob_table6[index] == glob_input[i].token ) {
  		  glob_input[i].tablenum = 6;
  		  glob_input[i].column = index;
          break;
		}
	  	else if ( glob_table6[index] == "" ) {
  		  glob_table6[index] = glob_input[i].token;
  		  glob_input[i].tablenum = 6;
  		  glob_input[i].column = index;
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
  	if ( glob_table7[index] == "" ) {
  		glob_table7[index] = glob_input[i].token;
  		glob_input[i].tablenum = 7;
  		glob_input[i].column = index;
	}
	else {
	  save = index;
	  
	  if ( glob_table7[index] == glob_input[i].token ) {
  		glob_input[i].tablenum = 7;
  		glob_input[i].column = index;
	  } 

	  while( glob_table7[index] != glob_input[i].token ) {
	  	index++;
	  	
	  	if ( glob_table7[index] == glob_input[i].token ) {
  		  glob_input[i].tablenum = 7;
  		  glob_input[i].column = index;
          break;
		}
	  	else if ( glob_table7[index] == "" ) {
  		  glob_table7[index] = glob_input[i].token;
  		  glob_input[i].tablenum = 7;
  		  glob_input[i].column = index;
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
  	if ( glob_table5[index] == "" ) {
  		glob_table5[index] = glob_input[i].token;
  		glob_input[i].tablenum = 5;
  		glob_input[i].column = index;
	}
	else {
	  save = index;

	  if ( glob_table5[index] == glob_input[i].token ) {
  		glob_input[i].tablenum = 5;
  		glob_input[i].column = index;
	  } 

	  while( glob_table5[index] != glob_input[i].token ) {
        index++;
        if ( glob_table5[index] == glob_input[i].token ) {
  		  glob_input[i].tablenum = 5;
  		  glob_input[i].column = index;
          break;
		}
	  	else if ( glob_table5[index] == "" ) {
  		  glob_table5[index] = glob_input[i].token;
  		  glob_input[i].tablenum = 5;
  		  glob_input[i].column = index;
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
  
}

void Write_file(string filename) {
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
  	start = 0;
    for ( int i = 0; i < glob_input.size(); i++ ) {

      if ( glob_input[i].token != "\n" ) { 
      	if ( glob_input[i].type == "string" ) {
      	  for ( int j = 1; j < glob_input[i].token.size() - 1; j++ ) {
      	  	outfile << glob_input[i].token[j];
		  }
		}
		else {
		  outfile << glob_input[i].token;	
		} 
	  }
	  else { 
	  	outfile << glob_input[i].token;

	  	for ( int j = start; j < i; j++ ) {
	  	  if ( glob_input[j].type != "whitespace" ) {
	  	    if ( j > 0 && j+2 < i && glob_input[j].type == "ID" &&  glob_input[j+2].type == "string" ) {
	  	      
			}
	  	    else if ( j == i - 1 ) {
	  	  	  outfile << "(" << glob_input[j].tablenum << "," << glob_input[j].column << ")" << endl;
		    }
		    else{
		  	  outfile << "(" << glob_input[j].tablenum << "," << glob_input[j].column << ")";	
		    }
		  }
		}
		start = i + 1;
	  }
    }
  }
  cout << filename << " saved succesfully" << endl;
  outfile.close();
}


int main() {
  string filename;
  int ascii_sum, index;
  
  Set_vector_100();
  Readfile( "Table1", glob_table1, ".table" );
  Readfile( "Table2", glob_table2, ".table" );
  Readfile( "Table3", glob_table3, ".table" );
  Readfile( "Table4", glob_table4, ".table" );

//  Print( glob_table1 );
//  Print( glob_table2 );
//  Print( glob_table3 );
//  Print( glob_table4 );

  cout << "input file name: ";
  cin >> filename;
  cout << endl; 
  
  Scanner( filename, ".txt" );
  Lexical_Analysis();
  Check_type();
  
  for( int i = 0; i < glob_input.size(); i++ ) {
    if ( glob_input[i].type != "token" ) {
      ascii_sum = Cal_ASCII( glob_input[i].token , glob_input[i].type );
      index = ascii_sum % 100 ;
      Hash_Func( index, glob_input[i].type, i );
	}
    else {
      
	}
  }

  Write_file( filename );
  
  return 0;
} // main() 
