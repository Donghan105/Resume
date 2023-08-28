# include <iostream>
# include <stdio.h>
# include <string> // string型別, strcpy, assign, substr, erase, find, erase, strstr 
# include <vector>  // 動態陣列 
# include <stdlib.h> // atoi 
# include <sstream> // char to string  
# include <fstream>
# include <math.h>
# define instruction_length 3
using namespace std;

struct InputType{ 
  int tablenum;
  int column;
  int error;
  string token;
  string type;
}; // 使用者輸入的code 

struct Format134Type{ 
  string instruction;
  string opcode;
};

struct Format2Type{ 
  string instruction;
  string opcode;
  string regs;
};

struct CrossRefType{ 
  string symbol;
  string address;
};

struct LocationType{ 
  int line;
  string location;
  string obj_code;
};

struct SicXEType{ 
  int line;
  int format;
  int error;
  string location;
  string obj_code;
  string label;
  string instruction;
  string opcode;
  string OP1, OP2;
  string comment;
  string n, i, x, b, p, e;
  string at;
  string hash;
  string P;
};

//HW1
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
void Write_file1( string filename );
int NextToken( int i );

//HW2
//SIC 
string Bin_to_Hex( string num );
void Hex_to_Bin( string num );
int Bin_to_Dec( string num );
int Hex_to_Dec( string num );
string Dec_to_Bin( int num );
string Dec_to_Hex( int num );
string Sub_Hex( string a, string b ); // a - b
string Add_Hex( string a, string b ); // a + b
string Twos_Complement( string bin );
string String_or_Interger( string str );
bool Find_duplicate_symbol( string symbol );
string Byte_Objcode( int mode, string str );
string Word_Objcode( int num );
string Fetch_Opcode( string instruction );
string Fetch_Address( string symbol );
string Cal_Objcode( string opcode, string address, int x_bit );
string Fetch_Reg_Num( string reg );
//SIC/XE

void SIC( string filename );
void SIC_Pass1();
void SIC_Pass2();
void SICXE( string filename );
void SICXE_Pass1();
void SICXE_Pass2();
void Write_file2( string filename );
void Write_file3( string filename );

void Readformat134( string filename, vector<Format134Type>& tmp, string ext );
void Readformat2( string filename, vector<Format2Type>& tmp, string ext );

vector <string> glob_table1;
vector <string> glob_table2;
vector <string> glob_table3;
vector <string> glob_table4;
vector <string> glob_table5; // symbol
vector <string> glob_table6; // integer, float
vector <string> glob_table7; // string
vector <InputType> glob_input; // 使用者輸入的code 

vector <Format134Type> glob_format1;
vector <Format134Type> glob_format34;
vector <Format2Type> glob_format2;
vector <CrossRefType> glob_cross_ref; // cross_reference 
vector <LocationType> glob_location;
vector <SicXEType> SIC_XE;

string glob_LOCTR = "0000";
string glob_BASE = "0";



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
        element.error = 0;
          
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
        element.error = 0;

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
      element.error = 0;

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
      for ( int j = start + 1; j < end ; j++ ) { // 兩個/' 之間  
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
      element.error = 0;
      
      glob_input.insert(glob_input.begin() + start + 1, element);
      i++;

      start = -1;
      end = -1;
    } // end if 
    else if ( start != -1 && i == glob_input.size() - 1 ) { // 沒有第二個/"  
      
    } // end else if 
  } // end for
}

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
    else if ( ( int ) str[i] < 48 || ( int ) str[i] > 57 ) { // not 0~9 ascii code, ex: A-Z a-z 
      if ( ( int ) str[i] == 72 && i == str.size() - 1 ) { // H結尾  
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

void Write_file1( string filename ) {
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
	  	  if ( glob_input[j].token == "." ) { // 處理註解  
            j = i;
			outfile << endl; 
		  }
	  	  else if ( glob_input[j].type != "whitespace" ) {
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

string Bin_to_Hex( string num ) {
  vector <string> nibbles;
  // stringstream int_to_string;
  
  if ( num.size() % 4 == 0 ) { // 4bits 切成 1 nibble 
    for ( int i = 0; i < num.size(); i = i + 4 ) {
       string str = "";
       str = str + num[i] + num[i + 1] + num[i + 2] + num[i + 3];
       nibbles.push_back( str );
	}

    for ( int i = 0; i < nibbles.size(); i++ ) { // 計算10進位數 
      int decimal = 0;
      for( int j = 0; j < nibbles[i].size(); j++ ) { // per nibble  
        stringstream char_to_string;
        char ch = nibbles[i][j];
        char_to_string << ch;
        string str = char_to_string.str();
        int integer = atoi( str.c_str() ); // char to int 
        
        decimal = decimal + pow( 2, 3-j ) * integer; // 4bits從左側計算, bin to dec 
	  }
	  
	  if ( decimal < 10 ) {
        stringstream int_to_string;
        
        int_to_string << decimal;
        nibbles[i] = int_to_string.str(); // 此nibble為此10進位數  
	  }
	  else if ( decimal == 10 ) { // hex A,B,C,D,E,F
	  	nibbles[i] = "A";
	  }
	  else if ( decimal == 11 ) {
	  	nibbles[i] = "B";
	  }
	  else if ( decimal == 12 ) {
	  	nibbles[i] = "C";
	  }
	  else if ( decimal == 13 ) {
	  	nibbles[i] = "D";
	  }
	  else if ( decimal == 14 ) {
	  	nibbles[i] = "E";
	  }
	  else if ( decimal == 15 ) {
	  	nibbles[i] = "F";
	  }
	}
	
    num = "";
	for ( int i = 0; i < nibbles.size(); i++ ) {
	  num = num + nibbles[i];
	}
	
  }
  else { // 不為4的倍數 
  	num = "failed";
  }
  
  nibbles.clear();
  
  return num;
}

void Hex_to_Bin( string num ) {
  vector <string> nibbles;
  int decimal = 0;
  for ( int i = 0; i < num.size(); i++ ) { // hex to decimal 
    if ( num[i] == 'A' ) {
      nibbles.push_back( "1010" );
	}
    else if ( num[i] == 'B' ) {
      nibbles.push_back( "1011" );	
	}
    else if ( num[i] == 'C' ) {
      nibbles.push_back( "1100" );	
	}
    else if ( num[i] == 'D' ) {
      nibbles.push_back( "1101" );	
	}
    else if ( num[i] == 'E' ) {
      nibbles.push_back( "1110" );
	}
    else if ( num[i] == 'F' ) {
      nibbles.push_back( "1111" );
	}
	else { // 0-9 
      stringstream char_to_string;
      char ch = num[i];
      char_to_string << ch;
      string str = char_to_string.str();
      int integer = atoi( str.c_str() ); // char to int 

      str = "";
      for ( int i = 3; i >= 0; i-- ) {
      	if ( integer >= pow(2, i) ) {
      	  integer -= pow(2, i);
      	  str += "1";
		}
        else {
          str += "0";	
		}
	  }
	  
	  nibbles.push_back( str );
	}
  }
  
  nibbles.clear();

}

int Bin_to_Dec( string num ) {
  int decimal = 0;
  int integer;

  for ( int i = num.size() - 1; i >= 0; i-- ) { // hex to decimal 
    if ( num[i] == '0' ) {
      integer = 0;
	}
    else if ( num[i] == '1' ) {
      integer = 1;
	}
	
	decimal = decimal + pow(2, (num.size() - 1) - i ) * integer;
  }

  return decimal;
}

int Hex_to_Dec( string num ) {
  int decimal = 0;
  int integer;
  int length = num.size();

  for ( int i = 0; i < num.size(); i++ ) { // hex to decimal 
    if ( num[i] == 'A' ) {
      integer = 10;
	}
    else if ( num[i] == 'B' ) {
      integer = 11;
	}
    else if ( num[i] == 'C' ) {
      integer = 12;	
	}
    else if ( num[i] == 'D' ) {
      integer = 13;
	}
    else if ( num[i] == 'E' ) {
      integer = 14;
	}
    else if ( num[i] == 'F' ) {
      integer = 15;
	}
    else if ( num[i] == '0' ) {
      integer = 0;
	}
    else if ( num[i] == '1' ) {
      integer = 1;
	}
    else if ( num[i] == '2' ) {
      integer = 2;
	}
    else if ( num[i] == '3' ) {
      integer = 3;
	}
    else if ( num[i] == '4' ) {
      integer = 4;
	}
    else if ( num[i] == '5' ) {
      integer = 5;
	}
    else if ( num[i] == '6' ) {
      integer = 6;
	}
    else if ( num[i] == '7' ) {
      integer = 7;
	}
    else if ( num[i] == '8' ) {
      integer = 8;
	}
    else if ( num[i] == '9' ) {
      integer = 9;
	}
	 
	decimal = decimal + pow(16, (num.size() - 1) - i ) * integer;
  }

  return decimal;
}

string Dec_to_Bin( int num ) { // ==================================================================================================
  int j = 0;
  string str = "";

  while ( pow(2, j) < num ) {
    j++;
  }
  
  for ( int i = j; i >= 0; i-- ) { // hex to decimal 
    if ( num >= pow(2, i) ) {
      	num -= pow(2, i);
      	str += "1";
	}
    else {
        str += "0";	
	}
  }

  return str;
}

string Dec_to_Hex( int num ) {
  int i = 0, quo; // quo:商數  
  string hex = "";
  while ( 1 ) {
  	if ( pow(16, i) > num ) {
  	  break;
	}
	i++;
  }
  
  for ( int j = i - 1; j >= 0; j-- ) {
  	if ( pow(16, j) <= num ) {
  	  quo = num / pow(16, j);
  	  num -= quo * pow(16, j);
  	  
  	  if ( quo == 1 ) {
  	  	hex = hex + "1";
	  }
      else if ( quo == 2 ) {
      	hex = hex + "2";
	  }
      else if ( quo == 3 ) {
      	hex = hex + "3";
	  }
      else if ( quo == 4 ) {
      	hex = hex + "4";
	  }
      else if ( quo == 5 ) {
      	hex = hex + "5";
	  }
      else if ( quo == 6 ) {
      	hex = hex + "6";
	  }
      else if ( quo == 7 ) {
      	hex = hex + "7";
	  }
      else if ( quo == 8 ) {
      	hex = hex + "8";
	  }
      else if ( quo == 9 ) {
      	hex = hex + "9";
	  }
      else if ( quo == 10 ) {
      	hex = hex + "A";
	  }
      else if ( quo == 11 ) {
      	hex = hex + "B";
	  }
      else if ( quo == 12 ) {
      	hex = hex + "C";
	  }
      else if ( quo == 13 ) {
      	hex = hex + "D";
	  }
      else if ( quo == 14 ) {
      	hex = hex + "E";
	  }
      else if ( quo == 15 ) {
      	hex = hex + "F";
	  }
	}
	else {
	  hex = hex + "0";
	}
  }
  
  return hex;
}

string Sub_Hex( string a, string b ) { // a - b
  string result;
  int int_a, int_b;

  int_a = Hex_to_Dec( a );
  int_b = Hex_to_Dec( b );

  if ( b > a ) { // a - b < 0, 負數, 計算2's complement  
    int_b -= int_a; // > 0
    result = Dec_to_Bin( int_b ); // 1 0010 
    if ( result.size() <= 12 ) {
      while ( result.size() != 12 ) {
      	result = "0" + result;
	  }
	}

    for ( int i = result.size() - 1; i >= 0; i-- ) { // 1's complement 
      if ( result[i] == '1' ) {
        result[i] = '0';
	  }
      else if ( result[i] == '0' ) {
        result[i] = '1';
	  }
    }
    
    result = Twos_Complement( result ); // 1's complement + 1 , return hex 
  }
  else {
    int_a -= int_b;
    result = Dec_to_Hex( int_a );
  }
  
  return result;
}

string Add_Hex( string a, string b ) { // a + b
  string result;
  int int_a, int_b;

  int_a = Hex_to_Dec( a );
  int_b = Hex_to_Dec( b );
  
  int_a += int_b;
  result = Dec_to_Hex( int_a );
  
  if ( result.size() < 4 ) {
  	while ( result.size() != 4 ) {
  	  result = "0" + result;
	}
  }

  return result;
}

string Twos_Complement( string bin ) { /////////////////////////////////////////////////////////////////////
  int dec;
  dec = Bin_to_Dec( bin ); 
  string hex = Dec_to_Hex( dec );
  
  dec += 1;
  hex = Dec_to_Hex( dec ); // bin轉dec 計算+1 

  

  return hex;
}

string String_or_Interger( string str ) {
  string result = "integer";

  for ( int i = 0; i < str.size() ; i++ ) {
    if ( ( int ) str[i] < 48 || ( int ) str[i] > 57 ) { 
      if ( ( int ) str[i] < 65 || ( int ) str[i] > 70 ) {
        if ( ( int ) str[i] < 97 || ( int ) str[i] > 102 ) {
          result = "string";
          break;
        } // end if 
      } // end if 
    } // end if 
  } // end for  

  return result;
}

bool Find_duplicate_symbol( string symbol ) {
  bool result = false;
  for( int i = 0; i < glob_cross_ref.size(); i++ ) {
  	if ( symbol == glob_cross_ref[i].symbol ) {
  	  result = true;
  	  break;
	}
  }

  return result;
}

string Byte_Objcode( int mode, string str ) { // 0:string 1:integer
  string objcode = "";
  if ( mode == 0 ) {
  	for ( int i = 0; i < str.size() ; i++ ) {
  		objcode = objcode + Dec_to_Hex( ( int ) str[i] );
	  }
  }
  else if ( mode == 1 ) {
  	objcode = str;
  	while ( objcode.size() != 6 ) {
  	  objcode = objcode + " ";	
	}
  }
  
  return objcode;
}

string Word_Objcode( int num ) {
  string objcode = "";
  
  objcode = Dec_to_Hex( num );
  
  while ( objcode.size() != 6 ) {
  	objcode = "0" + objcode;	
  }
  
  return objcode;
}

string Fetch_Opcode( string instruction ) {
  string opcode = "";

  for ( int j = 0; j < instruction.size(); j++) {
      instruction[j] = toupper( instruction[j] ) ; 
  }

  for ( int i = 0; i < glob_format1.size() && opcode == ""; i++ ) {
  	if ( glob_format1[i].instruction == instruction ) {
  	  opcode = glob_format1[i].opcode;
  	  break;
	}
  }

  for ( int i = 0; i < glob_format34.size() && opcode == ""; i++ ) {
  	if ( glob_format34[i].instruction == instruction ) {
  	  opcode = glob_format34[i].opcode;
  	  break;
	}
  }

  for ( int i = 0; i < glob_format2.size() && opcode == ""; i++ ) {
  	if ( glob_format2[i].instruction == instruction ) {
  	  opcode = glob_format2[i].opcode;
  	  break;
	}
  }
  
  return opcode;
}

string Fetch_Address( string symbol ) {
  string address = "-1", objcode = "";
  for( int i = 0; i < glob_cross_ref.size(); i++ ) {
  	if ( symbol == glob_cross_ref[i].symbol ) {
  	  address = glob_cross_ref[i].address;
  	  break;
	}
  }
  
  return address;
}

string Cal_Objcode( string opcode, string address, int x_bit ) {
  string objcode = "";
  if ( address == "" ) {
    // 不需要翻譯  
  }
  else if ( address == "-1" ) {
  	// error, symbol not found
  	objcode = "error: Symbol"; 
  }
  else {
  	if ( x_bit == 1 ) {
  	  string tmp = "";
	  tmp = tmp + address[0];
  	  int third_nibble = Hex_to_Dec( tmp );
  	  third_nibble += 8; // x_bit: 1000 
  	  tmp = Dec_to_Hex( third_nibble );
  	  address.erase( 0, 1 );
  	  objcode = opcode + tmp + address;
	}
	else {
	  objcode = opcode + address;	
	}
  }
  
  return objcode;
}

string Fetch_Reg_Num( string reg ) {
  string num = "";
  for ( int i = 0; i < reg.size(); i++) {
      reg[i] = toupper( reg[i] ) ; 
  }
  
  if ( reg == "A" ) {
  	num = "0";
  }
  else if ( reg == "X" ) {
  	num = "1";
  }
  else if ( reg == "L" ) {
  	num = "2";
  }
  else if ( reg == "B" ) {
  	num = "3";
  }
  else if ( reg == "S" ) {
  	num = "4";
  }
  else if ( reg == "T" ) {
  	num = "5";
  }
  else if ( reg == "F" ) {
  	num = "6";
  }
  else if ( reg == "PC" ) {
  	num = "8";
  }
  else if ( reg == "SW" ) {
  	num = "9";
  }
  
  return num;
}

void SIC( string filename ) {
  SIC_Pass1();
  SIC_Pass2();
  Write_file2( filename );
}

void SIC_Pass1() {
  //string glob_LOCTR;
  //glob_table5;  symbol
  //glob_table6; integer, float
 //glob_table7; string
  int flag = 0, line = 5;
  int start = -1, end = -1, error = 0, comment = 0; // 每行指令的開始 結尾  
  int RESB = -1, RESW = -1, START = -1, BYTE = -1, END = -1, WORD = -1;
  bool tkn = false;
  CrossRefType element;
  LocationType thisline;
  element.symbol = "-1"; // initialize 
  
  for ( int i = 0; i < glob_input.size(); i++ ) {
    if ( glob_input[i].type != "whitespace" ) {
      tkn = true;

      if ( glob_input[i].token == "." ) { // 註解  
        if ( end == -1 ) {
          end = i;
		}
        comment = 1;
	  }
	  else {

	  	if ( start == -1 ) {
	  	  start = i;
		}
	  	
	  	if ( comment == 1 ) {
	  		
		}
        else if ( flag == 0 ) { // 找symbol 
          if ( glob_input[i].tablenum == 5 ) {
          	if ( Find_duplicate_symbol( glob_input[i].token ) == true ) { // ************** 
          	 error = 1;
			}
			else {
              element.symbol = glob_input[i].token; 
			}
          	flag = 1; // 找instruction  
		  }
		  else {
		  	flag = 1; // 找instruction  
		  	i--; //下一回合還是檢查此token  
		  }
		}
	    else if ( flag == 1 ) { // 找instruction  
	      if ( glob_input[i].tablenum == 1 || glob_input[i].tablenum == 2 ) { // 是instruction 

	        string str = glob_input[i].token;
            for ( int j = 0; j < str.size(); j++) {
              str[j] = toupper( str[j] ) ; 
	        }
	        

            if ( str == "START" ) {
              START = i;
			}
            else if ( str == "RESB" ) {
              RESB = i;
		    }
		    else if ( str == "RESW" ) { // 1 word 3 bytes 
              RESW = i;
			}
		    else if ( str == "BYTE" ) {
              BYTE = i;
			}
		    else if ( str == "END" ) {
              END = i;
			}
		    else if ( str == "WORD" ) {
              WORD = i;
			}
			
		  }
		  else { // 錯誤  
            error = 1;
		  }
		  flag = 2;
	    }
	    else if ( flag == 2 ) { // 找literal直到遇到'\n' 

		}
	  }
	}
	else if ( tkn == true && ( glob_input[i].token == "\n" || i == glob_input.size() - 1 ) ) {
      	thisline.obj_code = "";
      	
      	if ( comment == 0 ) { // 沒有註解  
          end = i;
		}
        
        if ( error == 1 ) { // 此行有誤  
          for ( int j = start; j <= end; j++ ) {
          	glob_input[j].error = 1;
		  }
		  thisline.location = "error";
		}
        else if ( comment == 1 ) { // 此行有註解 
          error = 1;
          int have_token = 0;

          for ( int j = start; j < end && error == 1 ; j++ ) {
            if ( glob_input[i].tablenum == 1 || glob_input[i].tablenum == 2 ) { // 是instruction 
              error = 0;
            }
            else if ( glob_input[i].type != "whitespace" ) {
              have_token = 1;
			}
		  }
		  
		  if ( have_token == 0 ) { // 註解以前什麼都沒有  
		    thisline.location = "";
		  }
		  else if ( error == 0 ) {
		  	thisline.location = glob_LOCTR;
			int dec = instruction_length;
			string hex = Dec_to_Hex( dec );
			glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		  }
		  else {
            for ( int j = start; j <= end; j++ ) {
          	  glob_input[j].error = 1;
		    }
		  	thisline.location = "error";
		  }
		}
		else {
		  thisline.location = glob_LOCTR;

          if ( START != -1 ) {
          	int next;
            next = NextToken( START + 1 );
            if ( next <= end && glob_input[next].tablenum == 6 ) { // hex 
              glob_LOCTR = glob_input[next].token;
    
              if ( glob_LOCTR.size() < 4 ) {
  	            while ( glob_LOCTR.size() != 4 ) {
  	              glob_LOCTR = "0" + glob_LOCTR;
	            }
              }

              thisline.location = glob_LOCTR;
			}
			else { // 錯誤  
			  error = 1;
			  thisline.location = "error";
			  for ( int j = start; j <= end; j++ ) {
          	    glob_input[j].error = 1;
		      }
			}
		  }
          else if ( RESB != -1 ) {
          	int next;
          	string hex = "";
            next = NextToken( RESB + 1 );
            if ( next <= end && glob_input[next].tablenum == 6 ) {
              if ( glob_input[next].token[ glob_input[next].token.size() - 1 ] == 'H' ) { // hex
                for ( int j = 0; j < glob_input[next].token.size() - 1; j++ ) {
                  hex += glob_input[next].token[j];
				}
				
				if ( element.symbol != "-1" ) { // 有symbol  
		  		  element.address = glob_LOCTR;
          		  glob_cross_ref.push_back( element );
          	
                  //cout << "hex: " << hex << endl; 
              	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		        }
		        else {
                  //cout << "hex: " << hex << endl; 
              	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
			    }
			  }
			  else {
			  	if ( element.symbol != "-1" ) { // 有symbol  
		  		  element.address = glob_LOCTR;
          		  glob_cross_ref.push_back( element );
          	
			  	  int dec = atoi( glob_input[next].token.c_str() );
			  	  string hex = Dec_to_Hex( dec );
			  	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		        }
		        else {
			  	  int dec = atoi( glob_input[next].token.c_str() );
			  	  string hex = Dec_to_Hex( dec );
			  	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
			    }
			  }
			}
			else { // 錯誤  
			  thisline.location = "error"; 
			  error = 1;
			  for ( int j = start; j <= end; j++ ) {
          	    glob_input[j].error = 1;
		      }
			}
		  }
		  else if ( RESW != -1 ) { // 1 word 3 bytes 
          	int next;
          	string hex = "";
            next = NextToken( RESW + 1 );
            if ( next <= end && glob_input[next].tablenum == 6 ) {
              if ( glob_input[next].token[ glob_input[next].token.size() - 1 ] == 'H' ) { // hex
                for ( int j = 0; j < glob_input[next].token.size() - 1; j++ ) {
                  hex += glob_input[next].token[j];
				}
				
			    if ( element.symbol != "-1" ) { // 有symbol  
		  		  element.address = glob_LOCTR;
          		  glob_cross_ref.push_back( element );
          	
                  //cout << "hex: " << hex << endl; 
                  // 1 word 3 bytes 
              	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
              	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
              	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		        }
		        else {
                  //cout << "hex: " << hex << endl; 
                  // 1 word 3 bytes 
              	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
              	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
              	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
			    }	  
			  }
			  else {
			    if ( element.symbol != "-1" ) { // 有symbol  
		  		  element.address = glob_LOCTR;
          		  glob_cross_ref.push_back( element );
          	
                  int dec = atoi( glob_input[next].token.c_str() ) * 3; // 1 word 3 bytes 
                  string hex = Dec_to_Hex( dec );
                  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		        }
		        else {
                  int dec = atoi( glob_input[next].token.c_str() ) * 3; // 1 word 3 bytes 
                  string hex = Dec_to_Hex( dec );
                  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
			    }
			  }
			}
			else { // 錯誤  
			  thisline.location = "error"; 
			  error = 1;
			  for ( int j = start; j <= end; j++ ) {
          	    glob_input[j].error = 1;
		      }
			}
		  }
		  else if ( BYTE != -1 ) {
            int result = 0;
            for ( int j = BYTE; j < end && result == 0; j++ ) {
              if ( glob_input[j].type == "string" ) {
              	result = j;
			  }
			}
			
            if ( result != 0 ) { 
              string str = glob_input[result].token;
			  str = str.erase( str.size() - 1, 1 );
			  str = str.erase( 0, 1 );
			  int dec;
			  if ( String_or_Interger( str ) == "string" ) {
			  	dec = str.size(); // 減去左右'
			  	thisline.obj_code = Byte_Objcode( 0, str );
			  }
			  else { // integer
			  	dec = str.size() / 2;
			  	thisline.obj_code = Byte_Objcode( 1, str );
			  }
			  
			  if ( element.symbol != "-1" ) { // 有symbol  
		  		element.address = glob_LOCTR;
          		glob_cross_ref.push_back( element );
          	
			    //cout << "dec: " << dec << endl; 
			    string hex = Dec_to_Hex( dec );
			    //cout << "hex123: " << hex << endl;
			    glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		      }
		      else {
			    //cout << "dec: " << dec << endl; 
			    string hex = Dec_to_Hex( dec );
			    //cout << "hex123: " << hex << endl;
			    glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
			  }	  			  
			}
			else { // 錯誤  
			  thisline.location = "error"; 
			  error = 1;
			  for ( int j = start; j <= end; j++ ) {
          	    glob_input[j].error = 1;
		      }
			}
		  }
		  else if ( END != -1 ) {
		    thisline.location = "";
		  }
		  else if ( WORD != -1 ) {
          	int next;
          	string hex = "";
            next = NextToken( WORD + 1 );   	
           	
            if ( next <= end && glob_input[next].tablenum == 6 ) {
              if ( glob_input[next].token[ glob_input[next].token.size() - 1 ] == 'H' ) { // hex
                for ( int j = 0; j < glob_input[next].token.size() - 1; j++ ) {
                  hex += glob_input[next].token[j];
				}
                //cout << "hex: " << hex << endl;
                int dec = Hex_to_Dec( hex );
              	thisline.obj_code = Word_Objcode( dec );
			  }
			  else {
			  	int dec = atoi( glob_input[next].token.c_str() );
                thisline.obj_code = Word_Objcode( dec );
			  }
			  
			  if ( element.symbol != "-1" ) { // 有symbol  
		  		element.address = glob_LOCTR;
          		glob_cross_ref.push_back( element );
          	
          		int dec = instruction_length;
				//cout << "instruction_length: " << dec << endl; 
				string hex = Dec_to_Hex( dec );
			    //cout << "hex instruction: " << hex << endl;
			    glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		      }
		      else {
          		int dec = instruction_length;
				//cout << "instruction_length: " << dec << endl; 
				string hex = Dec_to_Hex( dec );
			    //cout << "hex instruction: " << hex << endl;
			    glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
			  }
			}
			else { // 錯誤  
			  thisline.location = "error"; 
			  error = 1;
			  for ( int j = start; j <= end; j++ ) {
          	    glob_input[j].error = 1;
		      }
			}
		  }
		  else if ( element.symbol != "-1" ) { // 有symbol  
		  	element.address = glob_LOCTR;
          	glob_cross_ref.push_back( element );
          	
          	int dec = instruction_length;
			//cout << "instruction_length: " << dec << endl; 
			string hex = Dec_to_Hex( dec );
			//cout << "hex instruction: " << hex << endl;
			glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		  }
          else {
			int dec = instruction_length;
			string hex = Dec_to_Hex( dec );
			glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		  }
		}

		thisline.line = line;
		//cout << thisline.line << "\t" << thisline.location << "\t" << thisline.obj_code << "\t" << endl;
		glob_location.push_back(thisline);
		
		line += 5;
        flag = 0; // 新的一行 從找symbol開始  
        element.symbol = "-1"; // initialize 	
		error = 0; // initialize 
		start = -1; // initialize 
		end = -1; // initialize 
		comment = 0; // initialize 
		START = -1; // initialize 
		RESB = -1; // initialize 
		RESW = -1; // initialize
		BYTE = -1; // initialize
		END = -1; // initialize
		WORD = -1; // initialize
		tkn = false; // initialize
	  }
  }
}

void SIC_Pass2() {
  //string glob_LOCTR;
  //glob_table5;  symbol
  //glob_table6; integer, float
 //glob_table7; string
  int i, line = 0, flag = 0;
  bool tkn = false;
  CrossRefType element;
  LocationType thisline;
  element.symbol = "-1"; // initialize 

  for ( i = 0; i < glob_input.size(); i++ ) {
    if ( glob_input[i].token != "\n" ) {
      tkn = true;
    }
      
    int j, index_reg = 0, PSEUDO = 0, comment = 0;
    string opcode, address = "";

    for ( j = i; glob_input[j].token != "\n"; j++ ) {
      if ( glob_input[j].type != "whitespace" ) {
        if ( glob_input[j].token == "." ) { // 註解  
          comment = 1;
	    }
	    else if ( comment == 1 ) {
	      address = ""; 
		}
		else if ( PSEUDO == 1 ) {
		  address = ""; 
		}
	    else if ( glob_location[line].obj_code == "" && glob_input[j].error != 1 ) {
          if ( flag == 0 ) { // 找symbol 
            if ( glob_input[j].tablenum == 5 ) {
          	  element.symbol = glob_input[j].token;
          	  flag = 1; // 找instruction  
		    }
		    else {
		  	  flag = 1; // 找instruction  
		  	  j--; //下一回合還是檢查此token  
		    }
	  	  }
	      else if ( flag == 1 ) { // 找instruction  
	        if ( glob_input[j].tablenum == 1 ) { // executable instruction 
	          opcode = Fetch_Opcode( glob_input[j].token );
              address = "0000"; // 先預設address  
		  	  if ( opcode != "" ) {
		  	  	
			  }
		    }
		    else if ( glob_input[j].tablenum == 2 ) { // pseudo instruction 
		      PSEUDO = 1;
			}
		    flag = 2;
	      }
	      else if ( flag == 2 ) { // 找literal直到遇到'\n' 
	        string symbol = glob_input[j].token;
	         
	        for ( int k = 0; k < symbol.size(); k++) {
              symbol[k] = toupper( symbol[k] ) ; 
	        }
	         
            if ( symbol == "X" ) {
              index_reg = 1;
			}
			else if ( symbol == "," ) {
			 	
			}
			else { // fetch address 
			  address = Fetch_Address( glob_input[j].token );
			}
		  }
	    }
	  }
	}
	
	if ( line < glob_location.size() && glob_location[line].obj_code == "" ) {
	  glob_location[line].obj_code = Cal_Objcode( opcode, address, index_reg );
	}

    if ( tkn == true && glob_input[j].token == "\n" ) {
      line++;
      tkn = false;
	}	
	
	PSEUDO = 0; // initialize
	comment = 0; // initialize 
	flag = 0; // initialize 
	index_reg = 0; // initialize 
	i = j; // initialize 
  }
}

void SICXE( string filename ) {
  SICXE_Pass1(); 
  SICXE_Pass2();
  
//  for ( int i = 0 ; i < SIC_XE.size(); i++ ) {
//  	if ( SIC_XE[i].comment == "" ) {
//  	  cout << SIC_XE[i].n << " " << SIC_XE[i].i << " " << SIC_XE[i].x << " " << SIC_XE[i].b << " " << SIC_XE[i].p << " " << SIC_XE[i].e << ", e: " << SIC_XE[i].error << ", f: " << SIC_XE[i].format << " " << SIC_XE[i].location;
//  	  cout << "\t" << SIC_XE[i].line << "\t" << SIC_XE[i].instruction << "\t" << SIC_XE[i].opcode << "\tLable: " << SIC_XE[i].label << "\tOP1: " << SIC_XE[i].OP1 << "\tOP2: " << SIC_XE[i].OP2 << "\tObject_code: " << SIC_XE[i].obj_code << endl;
//	}
//	else {
//	  cout << "C: " << SIC_XE[i].comment << endl;
//	}
//  	
//    // element.obj_code = "";
//  }
//  
//  cout << "cross ref: " << endl; 
//  for ( int i = 0 ; i < glob_cross_ref.size(); i++ ) {
//  	cout << glob_cross_ref[i].symbol << " " << glob_cross_ref[i].address << endl; 
//  } 

  Write_file3( filename );
} 

void SICXE_Pass1() {
  int flag = 0, line = 5;
  int comment = -1; // 每行指令的開始 結尾  
  int RESB = -1, RESW = -1, START = -1, BYTE = -1, END = -1, WORD = -1, PSEUDO = -1, BASE = -1;

  SicXEType element;
  element.line = 0;
  element.format = 0;
  element.location = "";
  element.obj_code = "";
  element.label = "";
  element.instruction = "";
  element.opcode = "";
  element.OP1 = "";
  element.OP2 = "";
  element.comment = "";
  element.n = "1";
  element.i = "1";
  element.x = "0";
  element.b = "0";
  element.p = "0";
  element.e = "0";
  element.error = 0;
  element.P = "";
  element.at = "";
  element.hash = "";
  
  CrossRefType ref;
  ref.symbol = "";
  ref.address = "";
  
  for ( int i = 0; i < glob_input.size(); i++ ) {
    if ( glob_input[i].type != "whitespace" ) {
      if ( element.error == 1 ) {
      	
	  }
	  else if ( comment != -1 ) {
	  	
	  }
	  else if ( glob_input[i].token == "." ) { // 註解  
        comment = i;
	  }
      else if ( flag == 0 ) { // 找symbol  
      	if ( glob_input[i].tablenum == 5 ) {
          if ( Find_duplicate_symbol( glob_input[i].token ) == true ) { // ************** 
            element.error = 1;
		  }
		  else {
		  	element.label = glob_input[i].token;
		  	ref.symbol = glob_input[i].token;
		  }
		}
		else {
		  i--;
		}
        flag = 1; // 找instruction
	  }
	  else if ( flag == 1 ) {
        if ( glob_input[i].tablenum == 4 ) { // + 
          if ( glob_input[i].token == "+" ) { // format 4 
          	element.format = 4;
          	element.e = "1";
          	element.P = "+";
		  }
        }
        else {
          i--;
		}
		flag = 2;
	  }
	  else if ( flag == 2 ) { // @@@
	    if ( glob_input[i].tablenum == 1 || glob_input[i].tablenum == 2 ) { // 是instruction 
          element.instruction = glob_input[i].token;
		  string instruction = glob_input[i].token;

          for ( int j = 0; j < instruction.size(); j++) {
            instruction[j] = toupper( instruction[j] ) ; 
          }

          for ( int j = 0; j < glob_format1.size() && element.opcode == ""; j++ ) {
  	        if ( glob_format1[j].instruction == instruction ) {
  	          element.opcode = glob_format1[j].opcode;
  	          if ( element.format == 4 ) { // error 
  	          	element.error = 1;
			  }
			  else {
			  	element.format = 1;
			  }
  	          break;
	        }
          }

          for ( int j = 0; j < glob_format34.size() && element.opcode == ""; j++ ) {
  	        if ( glob_format34[j].instruction == instruction ) {
  	          element.opcode = glob_format34[j].opcode;
  	          if ( element.format == 0 ) { // is format 3 
  	          	element.e = "0";
  	          	element.format = 3;
			  }
  	          break;
	        }
          }

          for ( int j = 0; j < glob_format2.size() && element.opcode == ""; j++ ) {
  	        if ( glob_format2[j].instruction == instruction ) {
  	          element.opcode = glob_format2[j].opcode;
  	          if ( element.format == 4 ) { // error 
  	          	element.error = 1;
			  }
			  else {
			  	element.format = 2;
			  }
  	          break;
	        }
          }
	        

          if ( instruction == "START" ) {
            START = i;
		  }
          else if ( instruction == "RESB" ) {
            RESB = i;
		  }
		  else if ( instruction == "RESW" ) { // 1 word 3 bytes 
            RESW = i;
		  }
		  else if ( instruction == "BYTE" ) {
            BYTE = i;
		  }
		  else if ( instruction == "END" ) {
            END = i;
		  }
		  else if ( instruction == "WORD" ) {
            WORD = i;
		  }
		  else if ( instruction == "BASE" ) {
            BASE = i;
		  }
		  else if ( instruction == "EQU" || instruction == "LTORG" ) {
            PSEUDO = i;
		  }
		}
		else { // 錯誤  
          element.error = 1;
		}
		flag = 3;
	  }
	  else if ( flag == 3 ) { // 找label  
	    string symbol = glob_input[i].token;
	         
	    for ( int k = 0; k < symbol.size(); k++) {
          symbol[k] = toupper( symbol[k] ) ; 
	    }
	         
        if ( symbol == "X" ) {
          element.x = "1";
		}
		else if ( symbol == "," ) {
			 	
		}
		else if ( symbol == "@" ) { // indirect 
		  if ( element.n == "0" && element.i == "1" ) {
            element.error = 1;
		  }
		  else {
            element.at = "@";
            element.n = "1";
            element.i = "0";
		  }
		}
		else if ( symbol == "#" ) { // immediate 
		  if ( element.n == "1" && element.i == "0" ) {
            element.error = 1;
		  }
		  else {
            element.hash = "#";
            element.n = "0";
            element.i = "1";
		  }
		}
		else {
		  if ( element.OP1 == "" ) {
		  	element.OP1 = glob_input[i].token;
		  }
		  else if ( element.OP2 == "" ) {
		  	element.OP2 = glob_input[i].token;
		  } 
		} 
	  }
	}
	else if ( glob_input[i].token == "\n" ) {
	  element.location = glob_LOCTR; // 設定location ******************************  

      if ( element.error == 1 ) {
      	element.obj_code = "error";
	  }
	  else if ( comment != -1 ) {
	  	string str = "";
	  	for ( int j = comment; j < i; j++ ) {
	  	  str = str + glob_input[j].token;
		}
		
		element.comment = str;
	  }
      else if ( START != -1 ) {
        int next;
        next = NextToken( START + 1 );
        if ( next <= i && glob_input[next].tablenum == 6 ) { // hex 
          glob_LOCTR = glob_input[next].token;
          
          if ( glob_LOCTR.size() < 4 ) {
  	        while ( glob_LOCTR.size() != 4 ) {
  	          glob_LOCTR = "0" + glob_LOCTR;
	        }
          }

          element.location = glob_LOCTR;
		}
		else { // 錯誤  
		  element.error = 1;
          element.obj_code = "error";
		}
	  }
      else if ( RESB != -1 ) {
        int next;
        string hex = "";
        next = NextToken( RESB + 1 );
        if ( next <= i && glob_input[next].tablenum == 6 ) {
          if ( glob_input[next].token[ glob_input[next].token.size() - 1 ] == 'H' ) { // hex
            for ( int j = 0; j < glob_input[next].token.size() - 1; j++ ) {
              hex += glob_input[next].token[j];
			}
				
			if ( element.label != "" ) { // 有symbol  
			  ref.symbol = element.label;
		  	  ref.address = glob_LOCTR;
          	  glob_cross_ref.push_back( ref );
          	
              //cout << "hex: " << hex << endl; 
              glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		    }
		    else {
              //cout << "hex: " << hex << endl; 
              glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
			}
		  }
		  else {
			if ( element.label != "" ) { // 有symbol  
			  ref.symbol = element.label;
		  	  ref.address = glob_LOCTR;
          	  glob_cross_ref.push_back( ref );
          	
			  int dec = atoi( glob_input[next].token.c_str() );
			  // dec = 9; // ******************************************************************************************* 
			  string hex = Dec_to_Hex( dec );
			  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		    }
		    else {
			  int dec = atoi( glob_input[next].token.c_str() );
			  // dec = 9; // ******************************************************************************************* 
			  string hex = Dec_to_Hex( dec );
			  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
			}
		  }
		}
		else { // 錯誤  
		  element.error = 1;
		  element.obj_code = "error";
		}
	  }
	  else if ( RESW != -1 ) { // 1 word 3 bytes 
        int next;
        string hex = "";
        next = NextToken( RESW + 1 );
        if ( next <= i && glob_input[next].tablenum == 6 ) {
          if ( glob_input[next].token[ glob_input[next].token.size() - 1 ] == 'H' ) { // hex
            for ( int j = 0; j < glob_input[next].token.size() - 1; j++ ) {
              hex += glob_input[next].token[j];
			}
				
			if ( element.label != "" ) { // 有symbol 
			  ref.symbol = element.label;
		  	  ref.address = glob_LOCTR;
          	  glob_cross_ref.push_back( ref );
          	
              //cout << "hex: " << hex << endl; 
              // 1 word 3 bytes 
              glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
              glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
              glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		    }
		    else {
              //cout << "hex: " << hex << endl; 
              // 1 word 3 bytes 
              glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
              glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
              glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
			}	  
		  }
		  else {
		    if ( element.label != "" ) { // 有symbol  
			  ref.symbol = element.label;
		  	  ref.address = glob_LOCTR;
          	  glob_cross_ref.push_back( ref );
          	
              int dec = atoi( glob_input[next].token.c_str() ) * 3; // 1 word 3 bytes 
              string hex = Dec_to_Hex( dec );
              glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		    }
		    else {
              int dec = atoi( glob_input[next].token.c_str() ) * 3; // 1 word 3 bytes 
              string hex = Dec_to_Hex( dec );
              glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
			}
		  }
		}
		else { // 錯誤  
		  element.error = 1;
		  element.obj_code = "error";
		}
	  }
	  else if ( BYTE != -1 ) {
        int result = 0;
        for ( int j = BYTE; j < i && result == 0; j++ ) {
          if ( glob_input[j].type == "string" ) {
            result = j;
		  }
		}
			
        if ( result != 0 ) { 
          string str = glob_input[result].token;
		  str = str.erase( str.size() - 1, 1 );
		  str = str.erase( 0, 1 );
		  int dec;
		  if ( String_or_Interger( str ) == "string" ) {
		    dec = str.size(); // 減去左右'
		    element.obj_code = Byte_Objcode( 0, str );
		  }
		  else { // integer
			dec = str.size() / 2;
			element.obj_code = Byte_Objcode( 1, str );
		  }
		
		  if ( element.label != "" ) { // 有symbol  
		    ref.symbol = element.label;
		  	ref.address = glob_LOCTR;
          	glob_cross_ref.push_back( ref );
          	
			//cout << "dec: " << dec << endl; 
			string hex = Dec_to_Hex( dec );
			//cout << "hex123: " << hex << endl;
			glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		  }
		  else {
			//cout << "dec: " << dec << endl; 
			string hex = Dec_to_Hex( dec );
			//cout << "hex123: " << hex << endl;
			glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		  }	  			  
	    }
		else { // 錯誤  
		  element.error = 1;
		  element.obj_code = "error";
		}
	  }
	  else if ( END != -1 ) {
        element.location = "";
	  }
	  else if ( WORD != -1 ) {
        int next;
        string hex = "";
        next = NextToken( WORD + 1 );   	
           	
        if ( next <= i && glob_input[next].tablenum == 6 ) {
          if ( glob_input[next].token[ glob_input[next].token.size() - 1 ] == 'H' ) { // hex
            for ( int j = 0; j < glob_input[next].token.size() - 1; j++ ) {
              hex += glob_input[next].token[j];
			}
            //cout << "hex: " << hex << endl;
            int dec = Hex_to_Dec( hex );
            element.obj_code = Word_Objcode( dec );
		  }
		  else {
			int dec = atoi( glob_input[next].token.c_str() );
            element.obj_code = Word_Objcode( dec );
		  }
			  
		  if ( element.label != "" ) { // 有symbol 
		    ref.symbol = element.label;
		  	ref.address = glob_LOCTR;
          	glob_cross_ref.push_back( ref );
          	
          	int dec = instruction_length;
			//cout << "instruction_length: " << dec << endl; 
			string hex = Dec_to_Hex( dec );
			//cout << "hex instruction: " << hex << endl;
			glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		  }
		  else {
          	int dec = instruction_length;
			//cout << "instruction_length: " << dec << endl; 
			string hex = Dec_to_Hex( dec );
			//cout << "hex instruction: " << hex << endl;
		    glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		  }
		}
		else { // 錯誤  
		  element.error = 1;
		  element.obj_code = "error";
		}
	  }
	  else if ( BASE != -1 ) {
        element.location = "";
		if ( element.OP1 != "" ) { // 有operand 
		  if ( Fetch_Address( SIC_XE[i].OP1 ) != "-1" ) {
		  	glob_BASE = Fetch_Address( SIC_XE[i].OP1 );
		  }
		  else {
		    element.error = 1;
		    element.obj_code = "error: operand";
		  }
		}
		else {
		  element.error = 1;
		  element.obj_code = "error: operand";
		}
	  }
	  else if ( PSEUDO != -1 ) { // 還不知道lTORG怎翻 只翻EQU 	
        int next;
        string hex = "";
        next = NextToken( PSEUDO + 1 );
		
        if ( next <= i && glob_input[next].tablenum == 6 ) {
          if ( glob_input[next].token[ glob_input[next].token.size() - 1 ] == 'H' ) { // hex
            for ( int j = 0; j < glob_input[next].token.size() - 1; j++ ) {
              hex += glob_input[next].token[j];
			}
            //cout << "hex: " << hex << endl;
            int dec = Hex_to_Dec( hex );
			string hex = Dec_to_Hex( dec );
			glob_LOCTR = Add_Hex( hex, "0" ); // a + b
		  }
		  else {
			int dec = atoi( glob_input[next].token.c_str() );
			string hex = Dec_to_Hex( dec );
			glob_LOCTR = Add_Hex( hex, "0" ); // a + b
		  }
		  
		  element.location = glob_LOCTR;
		}

		if ( element.label != "" ) { // 有symbol  
		  ref.symbol = element.label;
		  ref.address = glob_LOCTR;
          glob_cross_ref.push_back( ref );
          	
		  string hex = Dec_to_Hex( instruction_length );
	  	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		}
		else {
		  string hex = Dec_to_Hex( instruction_length );
	  	  glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
		}
	  }
	  else if ( element.label != "" ) {
	  	ref.symbol = element.label;
	  	ref.address = glob_LOCTR;
        glob_cross_ref.push_back( ref );
        
		//cout << "instruction_length: " << dec << endl; 
		string hex = Dec_to_Hex( element.format );
	  	glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
	  }
	  else {
		//cout << "instruction_length: " << dec << endl; 
		string hex = Dec_to_Hex( element.format );
	  	glob_LOCTR = Add_Hex( hex, glob_LOCTR ); // a + b
	  }
	  
	  element.line = line;
	  line += 5;
	  SIC_XE.push_back( element );
	  comment = -1; // initialize 
      flag = 0; // initialize 
      RESB = -1, RESW = -1, START = -1, BYTE = -1, END = -1, WORD = -1, PSEUDO = -1, BASE = -1;
      
      element.line = 0;
      element.format = 0;
      element.location = "";
      element.obj_code = "";
      element.label = "";
      element.instruction = "";
      element.opcode = "";
      element.OP1 = "";
      element.OP2 = "";
      element.comment = "";
      element.n = "1";
      element.i = "1";
      element.x = "0";
      element.b = "0";
      element.p = "0";
      element.e = "0";
      element.P = "";
      element.at = "";
      element.hash = "";
      element.error = 0;
      ref.symbol = "";
      ref.address = "";
	}
	
	if ( glob_input[i].token != "\n" && i == glob_input.size() - 1 ) { // 少執行最後一行 
	  InputType tmp;
	  tmp.tablenum = 0;
	  tmp.column = 0;
	  tmp.error = 0;
	  tmp.token = "\n";
	  tmp.type = "whitespace";
      glob_input.push_back( tmp );
	} 
	
  }

}

void SICXE_Pass2() {
  for ( int i = 0; i < SIC_XE.size(); i++ ) {
  	if ( SIC_XE[i].format == 0 ) { // PSEUDO 不用翻 or pass 1已翻好  

	}
  	else if ( SIC_XE[i].format == 1 ) {
  	  SIC_XE[i].obj_code = SIC_XE[i].opcode;
	}
  	else if ( SIC_XE[i].format == 2 ) {
  	  string instruction = SIC_XE[i].instruction;
  	  for ( int j = 0; j < instruction.size(); j++) {
        instruction[j] = toupper( instruction[j] ); 
	  }
	  
      for ( int j = 0; j < glob_format2.size(); j++ ) {
  	    if ( glob_format2[j].instruction == instruction ) {
  	      if ( glob_format2[j].regs == "r1" || glob_format2[j].regs == "n" ) { // 只要一個operand  
            if ( SIC_XE[i].OP1 == "" || SIC_XE[i].OP2 != "" ) {
              SIC_XE[i].error = 1;
              SIC_XE[i].obj_code = "error: operand";
			}
			else if ( SIC_XE[i].OP1 != "" ) {
			  if ( Fetch_Reg_Num( SIC_XE[i].OP1 ) != "" ) {
			  	SIC_XE[i].obj_code = SIC_XE[i].opcode + Fetch_Reg_Num( SIC_XE[i].OP1 ) + "0";
			  }
			  else {
			  	SIC_XE[i].error = 1;
			  	SIC_XE[i].obj_code = "error: no such reg";
			  }
			}
		  }
		  else if ( glob_format2[j].regs == "r1,r2" ) { // 2 operands 
            if ( SIC_XE[i].OP1 == "" || SIC_XE[i].OP2 == "" ) {
              SIC_XE[i].error = 1;
              SIC_XE[i].obj_code = "error: operand";
			}
			else if ( SIC_XE[i].OP2 != "" ) {
			  if ( Fetch_Reg_Num( SIC_XE[i].OP1 ) != "" && Fetch_Reg_Num( SIC_XE[i].OP2 ) != "" ) {
			  	SIC_XE[i].obj_code = SIC_XE[i].opcode + Fetch_Reg_Num( SIC_XE[i].OP1 ) + Fetch_Reg_Num( SIC_XE[i].OP2 );
			  }
			  else {
			  	SIC_XE[i].error = 1;
			  	SIC_XE[i].obj_code = "error: no such reg";
			  }
			}
		  }
  	      break;
	    }
      }
	}
  	else if ( SIC_XE[i].format == 3 ) {
      if ( SIC_XE[i].OP1 == "" ) {
  	  	SIC_XE[i].error = 1;
  	  	SIC_XE[i].obj_code = "error: operand";
	  }
	  else { // a = b + c, 求c 
	    string a = Fetch_Address( SIC_XE[i].OP1 );
	    string b, c;
	    
	  	if ( a == "-1" ) { // 是常數嗎 
          if ( Is_IDENT( SIC_XE[i].OP1 ) != true ) { // string or int 
            if ( String_or_Interger( SIC_XE[i].OP1 ) == "string" ) {
              a = "";
  	          for ( int j = 0; j < SIC_XE[i].OP1.size(); j++ ) {
  		        a = a + Dec_to_Hex( ( int ) SIC_XE[i].OP1[j] );
	          }
	        }
	        else {
	  	      a = Dec_to_Hex( atoi( SIC_XE[i].OP1.c_str() ) );
	  	      while ( a.size() < 3 ) {
      	        a = "0" + a;
	          }
	        }
	        
	        c = a;
		  	SIC_XE[i].b = "0";
		  	SIC_XE[i].p = "0";
		  	SIC_XE[i].obj_code = Dec_to_Bin( Hex_to_Dec( SIC_XE[i].opcode ) );
		  	SIC_XE[i].obj_code.erase( SIC_XE[i].obj_code.size() - 2, 2 );
		  	SIC_XE[i].obj_code += SIC_XE[i].n + SIC_XE[i].i + SIC_XE[i].x + SIC_XE[i].b + SIC_XE[i].p + SIC_XE[i].e;
		  	SIC_XE[i].obj_code =  Dec_to_Hex( Bin_to_Dec( SIC_XE[i].obj_code ) );
		  	SIC_XE[i].obj_code += c;
	      }
		  else {
  	  	    SIC_XE[i].error = 1;
  	  	    SIC_XE[i].obj_code = "error: operand";
		  } 	
        }
        else { //先嘗試 p = 1 
	  	  if ( i + 1 < SIC_XE.size() ) {
	  	    b = SIC_XE[i + 1].location;
	  	    c = Sub_Hex( a, b ); // c = a - b 
		    if ( c.size() <= 3 ) {
		  	  SIC_XE[i].b = "0";
		  	  SIC_XE[i].p = "1";
		  	  SIC_XE[i].obj_code = Dec_to_Bin( Hex_to_Dec( SIC_XE[i].opcode ) );
		  	  SIC_XE[i].obj_code.erase( SIC_XE[i].obj_code.size() - 2, 2 );
		  	  SIC_XE[i].obj_code += SIC_XE[i].n + SIC_XE[i].i + SIC_XE[i].x + SIC_XE[i].b + SIC_XE[i].p + SIC_XE[i].e;
		  	  SIC_XE[i].obj_code =  Dec_to_Hex( Bin_to_Dec( SIC_XE[i].obj_code ) );
		  	  SIC_XE[i].obj_code += c;
		    }
		    else { //再嘗試 b = 1 
		  	  b = glob_BASE;
		  	  c = Sub_Hex( a, b ); // c = a - b 
		  	  if ( c.size() <= 3 ) {
		  	    SIC_XE[i].b = "1";
		  	    SIC_XE[i].p = "0";
		  	    SIC_XE[i].obj_code = Dec_to_Bin( Hex_to_Dec( SIC_XE[i].opcode ) );
		  	    SIC_XE[i].obj_code.erase( SIC_XE[i].obj_code.size() - 2, 2 );
		  	    SIC_XE[i].obj_code += SIC_XE[i].n + SIC_XE[i].i + SIC_XE[i].x + SIC_XE[i].b + SIC_XE[i].p + SIC_XE[i].e;
		  	    SIC_XE[i].obj_code =  Dec_to_Hex( Bin_to_Dec( SIC_XE[i].obj_code ) );
		  	    SIC_XE[i].obj_code += c;
			  }
			  else {
			    SIC_XE[i].error = 1;
			    SIC_XE[i].obj_code = "error: disp";	
			  }
		    }
		  }
		  else { //再嘗試 b = 1 
		    b = glob_BASE;
		    c = Sub_Hex( a, b ); // c = a - b  
		    if ( c.size() <= 3 ) {
		  	  SIC_XE[i].b = "1";
		  	  SIC_XE[i].p = "0";
		  	  SIC_XE[i].obj_code = Dec_to_Bin( Hex_to_Dec( SIC_XE[i].opcode ) );
		  	  SIC_XE[i].obj_code.erase( SIC_XE[i].obj_code.size() - 2, 2 );
		  	  SIC_XE[i].obj_code += SIC_XE[i].n + SIC_XE[i].i + SIC_XE[i].x + SIC_XE[i].b + SIC_XE[i].p + SIC_XE[i].e;
		  	  SIC_XE[i].obj_code =  Dec_to_Hex( Bin_to_Dec( SIC_XE[i].obj_code ) );
		  	  SIC_XE[i].obj_code += c;
		    }
		    else {
			  SIC_XE[i].error = 1;
			  SIC_XE[i].obj_code = "error: disp";	
		    }
		  }
		}
	  }
	}
  	else if ( SIC_XE[i].format == 4 ) {
  	  if ( SIC_XE[i].x == "1" ) {
  	  	SIC_XE[i].error = 1;
  	  	SIC_XE[i].obj_code = "error: no need index addressing";
	  }
  	  else if ( SIC_XE[i].OP1 == "" || SIC_XE[i].OP2 != "" ) {
  	  	SIC_XE[i].error = 1;
  	  	SIC_XE[i].obj_code = "error: operand";
	  }
	  else {
	  	SIC_XE[i].obj_code = Dec_to_Bin( Hex_to_Dec( SIC_XE[i].opcode ) );
	  	SIC_XE[i].obj_code.erase( SIC_XE[i].obj_code.size() - 2, 2 );
	  	SIC_XE[i].obj_code += SIC_XE[i].n + SIC_XE[i].i + SIC_XE[i].x + SIC_XE[i].b + SIC_XE[i].p + SIC_XE[i].e;
	  	SIC_XE[i].obj_code = Dec_to_Hex( Bin_to_Dec( SIC_XE[i].obj_code ) );
	  	SIC_XE[i].obj_code = SIC_XE[i].obj_code + "0" + Fetch_Address( SIC_XE[i].OP1 );
	  }
	}
  }
}

void Write_file2( string filename ) {
  int index;
  int last = -1, next;
  bool tkn = false;
  int start;
  ofstream outfile;
  
  if ( filename.find( "_input" ) != -1 ) {
  	index = filename.find( "_input" );
  	filename.erase( index, filename.size() - index );
  }
  
  filename = "Assembler_" + filename + "_output" + ".txt";

  outfile.open( filename.c_str() );

  if ( outfile == NULL ) {cout << "Fail to save " << filename << endl;}
  else {
  	start = 0;
  	int i, line = 0;
  	
  	outfile << "Line    Loc    Source statement                         Object code" << endl; 
  	outfile << "----  -------- -------------------------                -----------" << endl; 
  	
    for ( i = 0; i < glob_input.size(); i++ ) {
      if ( glob_input[i].token != "\n" ) {
        tkn = true;
      }
      
      if ( tkn == true ) {
      	outfile << glob_location[line].line << "\t" << glob_location[line].location << "\t";
	  }
      
      int j, last_token;
      for ( j = i; glob_input[j].token != "\n"; j++ ) {
      	if ( glob_input[j].type == "string" ) {
      	  for ( int k = 1; k < glob_input[j].token.size() - 1; k++ ) {
      	  	outfile << glob_input[j].token[k];
		  }
		}
		else {
		  outfile << glob_input[j].token;	
		}

		last_token = j;
	  }

      if ( tkn == true && glob_input[j].token == "\n" ) {
      	if ( glob_input[last_token].tablenum == 1 || glob_input[last_token].tablenum == 2 ) { //沒有symbol  
      	  outfile << "\t";
		}
      	outfile << "\t\t\t" << glob_location[line].obj_code << endl;
      	line++;
      	tkn = false;
	  }	
	  
	  i = j;
    }
  }
  cout << filename << " saved succesfully" << endl;
  outfile.close();
}

void Write_file3( string filename ) {
  int index;
  int last = -1, next;
  bool tkn = false;
  int start;
  ofstream outfile;
  
  if ( filename.find( "_input" ) != -1 ) {
  	index = filename.find( "_input" );
  	filename.erase( index, filename.size() - index );
  }
  
  filename = "Assembler_" + filename + "_output" + ".txt";

  outfile.open( filename.c_str() );

  if ( outfile == NULL ) {cout << "Fail to save " << filename << endl;}
  else {
  	start = 0;
  	int i, line = 0;
  	
  	outfile <<  "Line    Location\tSource code\t\t\tObject code" << endl; 
  	outfile << "----    --------\t-------------------------\t-----------" << endl; 
  	
    for ( int i = 0; i < SIC_XE.size(); i++ ) {
  	  if ( SIC_XE[i].comment != "" ) {
  		outfile << SIC_XE[i].line << "\t\t\t\t" << SIC_XE[i].comment << endl;
	  }
	  else {
	    outfile << SIC_XE[i].line << "\t" << SIC_XE[i].location << "\t\t" << SIC_XE[i].label << "\t";
	    
	    if ( SIC_XE[i].P != "" ) {
	  	  outfile << "+";
	    }	

		outfile << SIC_XE[i].instruction << "\t";
		
		if ( SIC_XE[i].at != "" ) {
	  	  outfile << "@";
	    }
		else if ( SIC_XE[i].hash != "" ) {
		  outfile << "#";
		}	

        outfile << SIC_XE[i].OP1;

	    if ( SIC_XE[i].OP2 != "" ) {
	  	  outfile << "," << SIC_XE[i].OP2;
	    }
	    outfile << "\t\t" << SIC_XE[i].obj_code << endl;
	  }
    }
    cout << filename << " saved succesfully" << endl;
  }
  outfile.close();
}

void Readformat134( string filename, vector<Format134Type>& tmp, string ext ) { // ext:副檔名  
  ifstream infile;
  filename = filename + ext;
  string str;
  infile.open(filename.c_str());	//.c_str()用來將string 轉成 char[XXX], 因為open函數只能讀char[XXX], 但string撰寫比較方便
	
  if ( infile == NULL ) { cout << "Fail to open " << filename << endl; } //NULL為"空"的意思 
  else {
    cout << "完成開啟" << filename << endl;
    Format134Type element;
	while ( infile >> str != 0 ) {	//while( infile >> ch)	//當檔案中還有東西可以讀取, >>表示從檔案將資料傳到變數中 
	  element.instruction = str;

	  if ( infile >> str != 0 ) {
	  	element.opcode = str;
	  	tmp.push_back(element);
	  }

	}
  }
}

void Readformat2( string filename, vector<Format2Type>& tmp, string ext ) {
  ifstream infile;
  filename = filename + ext;
  string str;
  infile.open(filename.c_str());	//.c_str()用來將string 轉成 char[XXX], 因為open函數只能讀char[XXX], 但string撰寫比較方便
	
  if ( infile == NULL ) { cout << "Fail to open " << filename << endl; } //NULL為"空"的意思 
  else {
    cout << "完成開啟" << filename << endl;
    Format2Type element;
	while ( infile >> str != 0 ) {	//while( infile >> ch)	//當檔案中還有東西可以讀取, >>表示從檔案將資料傳到變數中 
	  element.instruction = str;

	  if ( infile >> str != 0 ) {
	  	element.opcode = str;
	  	if ( infile >> str != 0 ) {
	  	  element.regs = str;
	  	  tmp.push_back(element);
		}
	  }
	}
  }
}


int main() {
  string filename;
  int ascii_sum, index, mode;
  
  // HW1
  Set_vector_100();
  Readfile( "Table1", glob_table1, ".table" );
  Readfile( "Table2", glob_table2, ".table" );
  Readfile( "Table3", glob_table3, ".table" );
  Readfile( "Table4", glob_table4, ".table" );

  //Print( glob_table1 );
  //Print( glob_table2 );
  //Print( glob_table3 );
  //Print( glob_table4 );
  
  cout << "0: SIC, 1: SIC/XE, select: ";
  cin >> mode;

  cout << "input file name: ";
  cin >> filename;
  cout << endl; 
  
  Scanner( filename, ".txt" );
  
  if ( glob_input[ glob_input.size() - 1 ].token != "\n" ) { // 少執行最後一行 
	  InputType tmp;
	  tmp.tablenum = 0;
	  tmp.column = 0;
	  tmp.error = 0;
	  tmp.token = "\n";
	  tmp.type = "whitespace";
      glob_input.push_back( tmp );
  } 
  
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
  

  Write_file1( filename );
  
  //HW2
//  string str = Bin_to_Hex( "110100101010" );
//  Hex_to_Bin( str );
//  
//  Hex_to_Dec("1101");
//  Bin_to_Dec("1101");
  
  Readformat134( "format1", glob_format1, ".txt" );
  Readformat134( "format3&4", glob_format34, ".txt" );
  Readformat2( "format2", glob_format2, ".txt" );

  if ( mode == 0 ) {
  	SIC( filename );
  }
  else if ( mode == 1 ) {
  	SICXE( filename );
  }

  
//  for ( int i = 0; i < glob_format1.size(); i++ ) {
//  	cout << glob_format1[i].instruction << " " << glob_format1[i].opcode << endl;
//  }
//  cout << endl;
//  for ( int i = 0; i < glob_format34.size(); i++ ) {
//  	cout << glob_format34[i].instruction << " " << glob_format34[i].opcode << endl;
//  }
//  cout << endl;
//  for ( int i = 0; i < glob_format2.size(); i++ ) {
//  	cout << glob_format2[i].instruction << " " << glob_format2[i].opcode << " " << glob_format2[i].regs << endl;
//  }
  
  
  return 0;
} // main() 
