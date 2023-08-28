# include <iostream>
# include <stdio.h>
# include <string> // string���O, strcpy, assign, substr, erase, find, erase, strstr 
# include <vector>  // �ʺA�}�C 
# include <stdlib.h> // atoi 
# include <sstream> // char to string  
#include <fstream>
using namespace std;

struct CountType{
  string page;
  int freq;
};

struct PageType{
  int page_fault;
  int page_replace;
};

void Readfile( string filename );
bool Find_Page( vector<CountType>& tmp, string ref ); // �O�_�bmem����즹page  
PageType FIFO( vector<CountType>& tmp, string ref, int mode, string swap_out, PageType paging ); // ��ݳ̤[�����X   
PageType LRU( vector<CountType>& tmp, string ref, int mode, string swap_out, PageType paging ); // ����J��page����̤W��   
void LFU( int mode, string ref_str, vector<CountType>& memory, string filename ); // �u�����Xcouner�̤֪�page 
void MFU( int mode, string ref_str, vector<CountType>& memory, string filename ); // �u�����Xcouner�̦h��page  

int page_frame = 0; // input  
string reference_string; // input  
vector<CountType> fifo, lru, lfu_fifo, mfu_fifo, lfu_lru, mfu_lru;


void Readfile( string filename ) { // ext:���ɦW  
  ifstream infile;
  string str;
  int number;
  infile.open(filename.c_str());
	
  if ( infile == NULL ) { cout << "Fail to open " << filename << endl; }
  else {
    cout << "�����}��" << filename << endl; 
    infile >> str;
    page_frame = atoi(str.c_str());
    // cout << "page_frame: " << page_frame << endl;
    
    infile >> str;
    reference_string = str;
    // cout << "reference_string: " << reference_string << endl;
  }
}

bool Find_Page( vector<CountType>& tmp, string ref ) { // �O�_�bmem����즹page  
  bool found = false;
  for ( int i = 0; i < tmp.size(); i++ ) {
  	if ( tmp[i].page == ref ) {
  	  found = true;
  	  break;
	}
  }
  
  return found;
}

PageType FIFO( vector<CountType>& tmp, string ref, int mode, string swap_out, PageType paging ) { // ��ݳ̤[�����X, swap_out��LFU or MFU����X�Ӳ��X��page �Ѧ�function���X  
  bool found = false;

  found = Find_Page( tmp, ref );
  
  if ( found == false ) { // �ݭnswap out 
  	if ( mode == 0 ) { // FIFO 
  	  if ( tmp.size() != page_frame ) { // page_frame����  
  	  	paging.page_fault++; // page_fault++  
  	    CountType element;
  	    element.page = ref;
  	    element.freq = 1;
  	    tmp.push_back( element );
	  }
	  else { // page_frame�w��   
	    paging.page_replace++; // page_replace++ 
        paging.page_fault++; // page_fault++  
  	    tmp.erase( tmp.begin() );
  	    CountType element;
  	    element.page = ref;
  	    element.freq = 1;
  	    tmp.push_back( element );
	  }
	}
	else if ( mode == 1 ) { // LFU + FIFO, MFU + FIFO 
      for ( int i = 0; i < tmp.size(); i++ ) { // find swap_out in mem 
        if ( tmp[i].page == swap_out ) {
	      paging.page_replace++; // page_replace++ 
          paging.page_fault++; // page_fault++  
          tmp.erase( tmp.begin() + i ); // swap out  
  	      CountType element;
  	      element.page = ref;
  	      element.freq = 1;
  	      tmp.push_back( element ); // swap in  
		  break; 
	    }
      }
	}
  }
  else { // ���ݭnswap out  
    for ( int i = 0; i < tmp.size(); i++ ) { // find ref in mem 
      if ( tmp[i].page == ref ) {
        tmp[i].freq = tmp[i].freq + 1; // counter++ 
        break;
	  }
    } 
  }
  
  return paging;
} 

PageType LRU( vector<CountType>& tmp, string ref, int mode, string swap_out, PageType paging ) { // ����J��page����̤W��   
  bool found = false;

  found = Find_Page( tmp, ref );
  
  if ( found == false ) { // �ݭnswap out 
  	if ( mode == 0 ) { // LRU 
  	  if ( tmp.size() != page_frame ) { // page_frame����  
  	  	paging.page_fault++; // page_fault++  
  	    CountType element;
  	    element.page = ref;
  	    element.freq = 1;
  	    tmp.push_back( element );
	  }
	  else { // page_frame�w�� 
	    paging.page_replace++; // page_replace++ 
        paging.page_fault++; // page_fault++  
  	    tmp.erase( tmp.begin() ); // swap out  
  	    CountType element;
  	    element.page = ref;
  	    element.freq = 1;
  	    tmp.push_back( element ); // swap in  
	  }
	}
	else if ( mode == 1 ) { // LFU + LRU, MFU + LRU 
      for ( int i = 0; i < tmp.size(); i++ ) { // find swap_out in mem 
        if ( tmp[i].page == swap_out ) {
	      paging.page_replace++; // page_replace++ 
          paging.page_fault++; // page_fault++  
          tmp.erase( tmp.begin() + i ); // swap out  
  	      CountType element;
  	      element.page = ref;
  	      element.freq = 1;
  	      tmp.push_back( element ); // swap in  
		  break; 
	    }
      }
	}
  }
  else { // ���ݭnswap out  
    for ( int i = 0; i < tmp.size(); i++ ) { // find ref in mem 
      if ( tmp[i].page == ref ) { // �̷Ӱ��涶�ǱƧ� �⦹page�Ԧ^�̤W��(�����) 
  	    CountType element;
  	    element.page = ref;
  	    element.freq = tmp[i].freq + 1; // counter++  
  	    tmp.push_back( element );
        tmp.erase( tmp.begin() + i );
        break;
	  }
    }
  }
  return paging;
}

void LFU( int mode, string ref_str, vector<CountType>& memory, string filename ) { // �u�����Xcouner�̤֪�page  
  bool found = false;
  PageType info;
  info.page_fault = 0;
  info.page_replace = 0;
  
  ofstream outfile;
  outfile.open(filename.c_str(), ios::app);
  if ( outfile == NULL ){cout << "Fail to save " << filename << endl;}
  else {
  	if ( mode == 0 ) {
  	  outfile << endl;
  	  outfile << "--------------Least Frequently Used Page Replacement-----------------------" << endl;	
	}
	else if ( mode == 1 ) {
	  // outfile << endl;
	  outfile << "--------------Least Frequently Used LRU Page Replacement-----------------------" << endl;	
	}

    while ( ref_str.size() != 0 ) { // �٨S�����Ҧ�reference 
      char ch = ref_str[0];
      bool F = false; // page fault 
      stringstream char_to_string;

      char_to_string << ch;
      string ref = char_to_string.str(); // str = ch  
      ref_str.erase( 0, 1 );

  	  if ( memory.size() != page_frame ) { // frame�٨S��  
        found = Find_Page( memory, ref );

        if ( found == false ) { // �S����page  
          info.page_fault++; // page_fault++ 
          F = true;
  	      CountType element;
  	      element.page = ref;
  	      element.freq = 1;
  	      memory.push_back( element );
	    }
	    else {
	      if ( mode == 0 ) {
	      	int tmp = info.page_fault;
	  	    info = FIFO( memory, ref, 1, "", info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��  
	  	      F = true;
			}
	      }
	      else if ( mode == 1 ) {
	      	int tmp = info.page_fault;
	  	    info = LRU( memory, ref, 1, "", info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��   
	  	      F = true;
			}
	      }
	    }
	  }
	  else { // frame�w��  
	    found = Find_Page( memory, ref );
	  
	    if ( mode == 0 ) { // LFU + FIFO 
          if ( found == false ) { // �S����page  
		    int min_freq = memory[page_frame - 1].freq;
	        string swap_out = memory[page_frame - 1].page;
		  
		    for ( int i = page_frame - 2; i >= 0; i-- ) { // �M���묹�� 
		  	  if ( memory[i].freq <= min_freq ) {
		  	    min_freq = memory[i].freq;
		  	    swap_out = memory[i].page;
			  }
		    }
			 
		    int tmp = info.page_fault;
		    info = FIFO( memory, ref, 1, swap_out, info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��   
	  	      F = true;
			}
	      }
	      else { // ����page  
	        int tmp = info.page_fault;
            info = FIFO( memory, ref, 1, "", info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��   
	  	      F = true;
			}
	      }
	    }
	    else if ( mode == 1 ) { // LFU + LRU 
          if ( found == false ) { // �S����page  
		    int min_freq = memory[page_frame - 1].freq;
	        string swap_out = memory[page_frame - 1].page;
		  
		    for ( int i = page_frame - 2; i >= 0; i-- ) { // �M���묹�� 
		  	  if ( memory[i].freq <= min_freq ) {
		  	    min_freq = memory[i].freq;
		   	    swap_out = memory[i].page;
		  	  }
		    }
		    int tmp = info.page_fault;
		    info = LRU( memory, ref, 1, swap_out, info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��   
	  	      F = true;
			}
	      }
	      else { // ����page 
		    int tmp = info.page_fault; 
            info = LRU( memory, ref, 1, "", info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��   
	  	      F = true;
			}
	      }
	    }
	  }
	  
	  outfile << ref << "\t";
	  for ( int i = memory.size() - 1; i >= 0; i-- ) {
        outfile << memory[i].page; 
	  }
	  
	  if ( F == true ) {
	  	outfile << "\tF" << endl;
	  }
	  else {
	  	outfile << endl;
	  }
    }
    outfile << "Page Fault = " << info.page_fault << "  Page Replaces = " << info.page_replace << "  Page Frames = " << page_frame << endl;
    cout << filename << " saved successfully" << endl; 
  }
} 


void MFU( int mode, string ref_str, vector<CountType>& memory, string filename ) { // �u�����Xcouner�̦h��page   
  bool found = false;
  PageType info;
  info.page_fault = 0;
  info.page_replace = 0;
  
  ofstream outfile;
  outfile.open(filename.c_str(), ios::app);
  if ( outfile == NULL ){cout << "Fail to save " << filename << endl;}
  else {
  	if ( mode == 0 ) {
  	  outfile << endl;
  	  outfile << "--------------Most Frequently Used Page Replacement -----------------------" << endl;	
	}
	else if ( mode == 1 ) {
	  outfile << endl;
	  outfile << "--------------Most Frequently Used LRU Page Replacement -----------------------" << endl;	
	}

    while ( ref_str.size() != 0 ) { // �٨S�����Ҧ�reference 
      char ch = ref_str[0];
      bool F = false;
      stringstream char_to_string;

      char_to_string << ch;
      string ref = char_to_string.str(); // str = ch  
      ref_str.erase( 0, 1 );

  	  if ( memory.size() != page_frame ) { // frame�٨S��  
        found = Find_Page( memory, ref );

        if ( found == false ) { // �S����page 
		  info.page_fault++; // page_fault++ 
		  F = true;
  	      CountType element;
  	      element.page = ref;
  	      element.freq = 1;
  	      memory.push_back( element );
	    }
	    else {
	      if ( mode == 0 ) {
	      	int tmp = info.page_fault;
	  	    info = FIFO( memory, ref, 1, "", info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��   
	  	      F = true;
			}
	      }
	      else if ( mode == 1 ) {
	      	int tmp = info.page_fault;
	  	    info = LRU( memory, ref, 1, "", info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��   
	  	      F = true;
			}
	      }
	    }
	  }
	  else { // frame�w��  
	    found = Find_Page( memory, ref );
	  
	    if ( mode == 0 ) { // LFU + FIFO 
          if ( found == false ) { // �S����page  
		    int max_freq = memory[page_frame - 1].freq;
	        string swap_out = memory[page_frame - 1].page;
		  
		    for ( int i = page_frame - 2; i >= 0; i-- ) { // �M���묹�� 
		  	  if ( memory[i].freq >= max_freq ) {
		  	    max_freq = memory[i].freq;
		  	    swap_out = memory[i].page;
			  }
		    }
		  
		    int tmp = info.page_fault;
		    info = FIFO( memory, ref, 1, swap_out, info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��   
	  	      F = true;
			}
	      }
	      else { // ����page  
	        int tmp = info.page_fault;
            info = FIFO( memory, ref, 1, "", info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��   
	  	      F = true;
			}
	      }
	    }
	    else if ( mode == 1 ) { // LFU + LRU 
          if ( found == false ) { // �S����page  
		    int max_freq = memory[page_frame - 1].freq;
	        string swap_out = memory[page_frame - 1].page;
		  
		    for ( int i = page_frame - 2; i >= 0; i-- ) { // �M���묹�� 
		  	  if ( memory[i].freq >= max_freq ) {
		  	    max_freq = memory[i].freq;
		  	    swap_out = memory[i].page;
			  }
		    }

		    int tmp = info.page_fault;
		    info = LRU( memory, ref, 1, swap_out, info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��   
	  	      F = true;
			}
	      }
	      else { // ����page  
	        int tmp = info.page_fault;
            info = LRU( memory, ref, 1, "", info );
	  	    if ( tmp != info.page_fault ) { // page_fault�o��   
	  	      F = true;
			}
	      }
	    }
	  }

	  outfile << ref << "\t";
	  for ( int i = memory.size() - 1; i >= 0; i-- ) {
        outfile << memory[i].page; 
	  }
	  
	  if ( F == true ) {
	  	outfile << "\tF" << endl;
	  }
	  else {
	  	outfile << endl;
	  }
    }
    outfile << "Page Fault = " << info.page_fault << "  Page Replaces = " << info.page_replace << "  Page Frames = " << page_frame << endl;
    cout << filename << " saved successfully" << endl; 
  }
}

int main() {
  string filename;

  cout << "input file name(�t���ɦW): ";
  cin >> filename;
  cout << endl; 

  Readfile( filename );

  ofstream outfile, outfile2;
  filename = "out_" + filename;

  outfile.open(filename.c_str());
  if ( outfile == NULL ){cout << "Fail to save " << filename << endl;}
  else {
    PageType info;
    info.page_fault = 0;
    info.page_replace = 0;
    string str = reference_string;
    outfile << "--------------FIFO-----------------------" << endl; 
    while ( str.size() != 0 ) { // FIFO �٨S�����Ҧ�reference 
      bool F = false;
      char ch = str[0];
      stringstream char_to_string;

      char_to_string << ch;
      string ref = char_to_string.str(); // str = ch  
      str.erase( 0, 1 );

      int tmp = info.page_fault;
  	  info = FIFO( fifo, ref, 0, "", info ); // ��ݳ̤[�����X  
	  if ( tmp != info.page_fault ) {
	  	F = true;
	  }
  	  
  	  outfile << ref << "\t";
  	  for ( int i = fifo.size() - 1; i >= 0; i-- ) {
        outfile << fifo[i].page;
	  }

	  if ( F == true ) {
	  	outfile << "\tF" << endl;
	  }
	  else {
	  	outfile << endl;
	  }
    } 
    outfile << "Page Fault = " << info.page_fault << "  Page Replaces = " << info.page_replace << "  Page Frames = " << page_frame << endl;
    cout << filename << " saved successfully" << endl;
  }
  outfile.close();

  outfile2.open(filename.c_str(), ios::app );
  if ( outfile2 == NULL ){cout << "Fail to save " << filename << endl;}
  else {
    PageType info;
    info.page_fault = 0;
    info.page_replace = 0;
    string str = reference_string;
    outfile2 << endl;
    outfile2 << "--------------LRU-----------------------" << endl; 
    while ( str.size() != 0 ) { // LRU �٨S�����Ҧ�reference 
      bool F = false;
      char ch = str[0];
      stringstream char_to_string;

      char_to_string << ch;
      string ref = char_to_string.str(); // str = ch  
      str.erase( 0, 1 );

      int tmp = info.page_fault;
  	  info = LRU( lru, ref, 0, "", info ); // ����J��page����̤W�� 
	  if ( tmp != info.page_fault ) {
	  	F = true;
	  }

      outfile2 << ref << "\t";
  	  for ( int i = lru.size() - 1; i >= 0; i-- ) {
        outfile2 << lru[i].page; 
	  }

	  if ( F == true ) {
	  	outfile2 << "\tF" << endl;
	  }
	  else {
	  	outfile2 << endl;
	  }
    }
    outfile2 << "Page Fault = " << info.page_fault << "  Page Replaces = " << info.page_replace << "  Page Frames = " << page_frame << endl;
	cout << filename << " saved successfully" << endl; 
  }
  outfile2.close();
  
  
  LFU( 0, reference_string, lfu_fifo, filename );
  MFU( 0, reference_string, mfu_fifo, filename );

  LFU( 1, reference_string, lfu_lru, filename );
  MFU( 1, reference_string, mfu_lru, filename );
  
  return 0;
} // main() 

