// DS2 ex5 DFS
// 10311245 吳承曄 / 10520104 莊東翰
// 2018 / 05 / 24

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace std;

typedef struct sT{
  char sid1[10];
  char sid2[10];
  float weight;
}studentType;

typedef struct nT{
  char sid[10];
  float weight;
}nodeType;

typedef struct pT{
  bool check;   //紀錄該點是否已查過 
  char sid[10];
  vector<nodeType> node;
}pairsType;

//**Grobal Value
string fileName;
vector<studentType> sSet; // Binary Set 
vector<pairsType> rSet;   // adj Set
vector<pairsType> cSet;   // Connected component Set 

//**Fuction list
int ReadBinaryFile();
void CreateList(); 
void Insert( char sid1[10], char sid2[10], float weight );
void CreateCClist(); 
void DFS(pairsType* CC, int i);
bool cmp_by_weight(nodeType left, nodeType right);  //排序參數 
bool cmp_by_sid(pairsType left, pairsType right);   //排序參數 
bool cmp_by_size(pairsType left, pairsType right);  //排序參數 

int main(int argc, char** argv) {
	
	int read = ReadBinaryFile();
	
	string cmd;
	
	while(read){
	  
    CreateList();   //任務一 
    CreateCClist();  //任務二 
    
    // -- initial -- 
		sSet.clear();
		rSet.clear();
		cSet.clear();
		// -------------
		
		cout << endl << "[0]Quit or [AnyOther]Continue ?" << endl;
		cin >> cmd;
		
		if(!cmd.compare("0")) return 0;
		else read = ReadBinaryFile();
		
  }

	return 0;
}

int ReadBinaryFile() {
  
  cout << endl << "Input the file name: 501, 502 ... [0]Quit" << endl;
	cin >> fileName;
  
  if(!fileName.compare("0")) return 0;	//quit
  
  string ifileName = "pairs" + fileName + ".bin";
  ifstream fin(ifileName.c_str(), ios::binary); 
  
  if(!fin){
		cout << "###  does not exist! ###" << endl;
		return ReadBinaryFile();	//再讀檔一次 
	}
	else{
	  studentType oneS;
	  
	  while(!fin.eof()){
	    fin.read(reinterpret_cast<char*>(&oneS), sizeof(studentType)); // ** reinterpret_cast 為強制轉換型別 
	    if(!fin.eof()) sSet.push_back(oneS);
    }
    
    return 1;
  }

} // ReadBinaryFile()

void CreateList() {
  
  for (int i = 0; i < sSet.size() ; i++ ) {
    
    Insert(sSet[i].sid1, sSet[i].sid2, sSet[i].weight);
    Insert(sSet[i].sid2, sSet[i].sid1, sSet[i].weight);

  }
  
  cout << "<<< " << sSet.size() << " pairs => " << sSet.size() * 2 << " nodes >>>" << endl << endl;
  
  
  //***************************** 輸出 .adj 檔案 **************************** 
  string ofileName = "pairs" + fileName + ".adj";
  ofstream fout(ofileName.c_str());
  
	for (int i = 0; i < rSet.size(); i++){
	  fout << "[ " << i+1 <<" ]" << " " << rSet[i].sid << " :";

	  for(int j = 0; j < rSet[i].node.size(); j++ ){
	    if(j % 5 == 0) fout << endl << "\t";
	    fout << "(" << j+1 << ") " << rSet[i].node[j].sid << ", " << rSet[i].node[j].weight << "\t";
    }
    fout << endl << endl;
  }
	 
	fout << endl << "<<< There are " << sSet.size() * 2 << " nodes in total. >>>" << endl;
	//********************************* END *********************************** 
	
	cout << "Adjacency lists are written into a file..." << endl << endl;
	cout << "<<< There are " << sSet.size() * 2 << " nodes in total. >>>" << endl;
  
} // CreateList()

void Insert( char sid1[10], char sid2[10], float weight ) {
  
  bool check = false;
  int k;
  for( k= 0; k < rSet.size(); k++) {
    if( strcmp(sid1, rSet[k].sid) == 0 ) {
      check = true;
      break;
    }
  }
  
  if(!check) {
    pairsType oneP;
    strcpy(oneP.sid, sid1);
    oneP.check = false;
      
    nodeType n;
    strcpy(n.sid, sid2);
    n.weight = weight;
    oneP.node.push_back(n);
      
    rSet.push_back(oneP);
    sort(rSet.begin(), rSet.end(), cmp_by_sid);   //依學號排序 
  }
  else{
    nodeType n;
    strcpy(n.sid, sid2);
    n.weight = weight;
    rSet[k].node.push_back(n);
    
    sort(rSet[k].node.begin(), rSet[k].node.end(), cmp_by_weight);  //依權重排序  
  }
  
} // Insert()

void CreateCClist() {
  
  for(int i = 0; i < rSet.size(); i++) {
    
    if(rSet[i].check == false){   //如果節點不屬於現有群組 --> 建立新群組 
      pairsType CC;
      DFS(&CC, i);        //用深度搜尋完成群組內容 
      
      cSet.push_back(CC); //完成一個群組 
    }
    
  } // for
  
  sort(cSet.begin(), cSet.end(), cmp_by_size);  //所有群組依照數量排序 
  
  
  //*************** 輸出 .cc 檔案 ( 螢幕顯示 & 輸出檔案 ) ******************* 
  string ofileName = "pairs" + fileName + ".cc";
  ofstream fout(ofileName.c_str());
  
  cout << endl << "---Connected Component---" << endl << endl;
  fout << endl << "---Connected Component---" << endl << endl;
  for(int k = 0; k < cSet.size(); k++){
    cout << "[" << k+1 << "]" <<"Connected Component: size = " << cSet[k].node.size() << ":";
    fout << "[" << k+1 << "]" <<"Connected Component: size = " << cSet[k].node.size() << ":";
    
    for(int j = 0; j < cSet[k].node.size(); j++){
      if(j % 5 == 0){
        cout << endl <<"\t";
        fout << endl <<"\t";
      } 
      cout << "(" << j+1 << ") " << cSet[k].node[j].sid << "\t";
      fout << "(" << j+1 << ") " << cSet[k].node[j].sid << "\t";
    }
      
    cout << endl << endl;
    fout << endl << endl;
  }
	//********************************* END *********************************** 
    
} // CreateCClist 

void DFS(pairsType* CC, int i){
  
  if(rSet[i].check == true) return;   //節點已查過 
  
  else{
    
    rSet[i].check = true; 
    
    nodeType n;
    strcpy(n.sid, rSet[i].sid);
    CC->node.push_back(n);
    for(int k = 0; k < rSet[i].node.size(); k++){   //逐步搜尋相連的節點 
      
      int pos;
      for(int j = 0; j < rSet.size(); j++){   // 找到節點在 rSet 的位置 
        if( strcmp(rSet[j].sid, rSet[i].node[k].sid) == 0) pos = j;
      }
      
      DFS(CC, pos);
    }
    
  } // else
  
} // DFS()

bool cmp_by_weight(nodeType left, nodeType right){
  
  return left.weight > right.weight ;
   
} // cmp_by_weight()

bool cmp_by_sid(pairsType left, pairsType right){
  
  int i = 0;
  while(left.sid[i] == right.sid[i] && i < 9){
    i++;
  }
  
  return left.sid[i] < right.sid[i] ;
   
} // cmp_by_sid()

bool cmp_by_size(pairsType left, pairsType right){
  
  return left.node.size() > right.node.size();
   
} // cmp_by_weight()



