// DS2 ex6 MST
// 10311245 吳承曄 / 10520104 莊東翰
// 2018 / 06 / 10

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
  bool mark;
}nodeType;

typedef struct pT{
  bool check;   //紀錄該點是否已查過 
  char sid[10];
  vector<nodeType> node;
}pairsType;

//**Grobal Value
string fileName;
string ofileName;
vector<studentType> sSet; // Binary Set 
vector<pairsType> rSet;   // adj Set
vector<pairsType> cSet;   // Connected component Set 

vector<nodeType> dSet;

//**Fuction list
int ReadBinaryFile();
void CreateList(); 
void Insert( char sid1[10], char sid2[10], float weight );
void CreateCClist(); 
void DFS(pairsType* CC, int i);
void MST();
void Prim(pairsType cc);
void ShortestPaths();
void Dijkstra(string sid, float accu);
int Sid_Serial_Number(char sid[10]);
int Dijkstra_Serial_Number(char sid[10]);

bool isVisited(vector<string> visited, string sid);
bool cmp_by_weight(nodeType left, nodeType right);  //排序參數 
bool cmp_by_sid(pairsType left, pairsType right);   //排序參數 
bool cmp_by_size(pairsType left, pairsType right);  //排序參數 

int main(int argc, char** argv) {
	
	int read = ReadBinaryFile();
	
	string cmd;
	
	while(read){
	  
    CreateList();   
    CreateCClist();
    MST();  //任務一 
    ShortestPaths();
    
    // -- initial -- 
		sSet.clear();
		rSet.clear();
		cSet.clear();
		dSet.clear();
		// -------------
		
		cout << endl << "[0]Quit or [AnyOther]Continue ?" << endl;
		cin >> cmd;
		
		if(!cmd.compare("0")) return 0;
		else read = ReadBinaryFile();
		
  }

	return 0;
}

int ReadBinaryFile() {
  
  cout << endl << "Input the file name: 601, 602 ... [0]Quit" << endl;
	cin >> fileName;
	ofileName = "pairs" + fileName + ".dis";
  
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
  
  
  //*************** 輸出 .cc 檔案 ( 螢幕顯示 ) ******************* 
  
  cout << endl << "---Connected Components are found!!---" << endl << endl;

  for(int k = 0; k < cSet.size(); k++){
    cout << "[" << k+1 << "]" <<"Connected Component: size = " << cSet[k].node.size() << ":";
    
    for(int j = 0; j < cSet[k].node.size(); j++){
      if(j % 5 == 0){
        cout << endl <<"\t";
      } 
      cout << "(" << j+1 << ") " << cSet[k].node[j].sid << "\t";
    }
      
    cout << endl << endl;
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

void MST(){

  cout << "---Minimum Spanning Tree---" << endl << endl;
  
  for(int i = 0; i < cSet.size(); i++){
    for(int j = 0; j < cSet[i].node.size(); j++){
    	cout << "[ " << j+1 << "]\t" << cSet[i].node[j].sid << "\t" << cSet[i].node[j].weight << endl;
	}
    // 依序找到各群組的最小生成樹 
    cout << "the MST cost of Connected Components {" << i+1 << "} = ";
    Prim(cSet[i]);
    
  } // for 
  
} // MST()

void Prim(pairsType cc){
  
  int count = 0, num1, num2;  // 紀錄已走過的節點個數 
  float cost = 0;
  vector<string> visited;
  
  visited.push_back(cc.node[0].sid); // 加入起始點 
  count++;
  
  while ( count < cc.node.size() ) {  // 持續做直到該群組的節點都找過 
    
    nodeType min;
    min.weight = 1;
    
    for(int i = 0; i < visited.size(); i++){
    
      // 找到節點在 rSet 的位置 
      int pos;
      for(int j = 0; j < rSet.size(); j++){   
        if( rSet[j].sid == visited[i] ) pos = j;
      }
    
      // --------- 找到該點最小權重的連通成分 ----------- 
      
      int n = rSet[pos].node.size()-1;    // 因為權重已經排序過( 大 -> 小 ) 所以只要從最後往前找 就一定是最小 
      bool found = true;
      while( n >= 0 && found ) {
        
        found = isVisited(visited, rSet[pos].node[n].sid); 
        if(found == true) n--;  // 如果節點已經走過 就再往前找 
        
      } // while 
    
      if( !found ) {
        // 如果權重小於當前最小值 就替換min 
        if(rSet[pos].node[n].weight < min.weight){
          strcpy(min.sid, rSet[pos].node[n].sid);
          min.weight = rSet[pos].node[n].weight;
          (num1,num2) = Dijkstra_Serial_Number(rSet[pos].node[n].sid);
          cSet[num1].node[num2].weight = min.weight;
          
        }
        
      }
      // ------------------- END ----------------------- 
    
    } // for

    cost += min.weight;
    visited.push_back(min.sid); // 標示已走過 
    count++;
    
  } // while
  
  printf("%.4f\n\n", cost); // 輸出至小數點第四位 
  
} // Prim()

void ShortestPaths(){
  int num;
  cout << "---Shortest Distances---" << endl << endl;
  
  for(int k = 0; k < rSet.size(); k++){
    if(k % 5 == 0) cout << endl;
    cout << "  " << rSet[k].sid;
  }
  
  string sid;
  cout << endl << "Input a student id from the above :" << endl;
  cin >> sid;
  ofstream fout(ofileName.c_str());
  
  nodeType n;
  strcpy(n.sid, sid.c_str());
  n.weight = 0; 
  n.mark = false;
  dSet.push_back(n);
  
  Dijkstra(sid, 0);
  
  // -------------輸出 -------------
  cout << endl;
  fout << "origin : " << sid << endl;
  fout << endl;
  for(int i = 0 ; i < dSet.size()-1; i++){
    num = Sid_Serial_Number(dSet[i].sid);    
    cout << "VertexSet <-- " << num+1 << ", min. distance = " << dSet[i].weight << endl; 
	cout << endl;   
    fout << "[" << num+1 << "]" << dSet[i].sid << ", " << dSet[i].weight << "\t";
  }
  
  cout << "~~~Shortest Distances are computed! ~~~" << endl;
  
} // ShortestPaths()

void Dijkstra(string sid, float accu){
  
  int k;
  for(int g = 0; g < dSet.size(); g++){
    if(dSet[g].sid == sid){ 
      dSet[g].mark = true;
      k = g;
    }
  } // for
  
  // 找到節點在 rSet 的位置 
  int pos;
  for(int j = 0; j < rSet.size(); j++){   
    if( rSet[j].sid == sid ) pos = j;
  }
      
  // 加入節點並確認權重 
  for(int i = 0; i < rSet[pos].node.size(); i++) {
    bool exist = false;
    int d;
    for(int p = 0; p < dSet.size(); p++){
      if(strcmp(dSet[p].sid, rSet[pos].node[i].sid) == 0 ) {
        d = p;
        exist = true;
      }
    }
        
    if(exist) {
      float new_weight = accu + rSet[pos].node[i].weight;
      if(new_weight < dSet[d].weight) dSet[d].weight = new_weight;  // 更新權重 
    }
    else{
      nodeType n;
      strcpy(n.sid, rSet[pos].node[i].sid);
      n.weight = accu + rSet[pos].node[i].weight; 
      n.mark = false;
      dSet.push_back(n);  // 加入節點 
    }
        
  } // for
      
  sort(dSet.begin(), dSet.end(), cmp_by_weight);  //依權重排序  
  
  for(int p = dSet.size()-1; p >= 0; p--){
    if(dSet[p].mark == false) Dijkstra(dSet[p].sid, dSet[p].weight);
  }
  
  
} // Dijkstra()




bool isVisited(vector<string> visited, string sid){
  
  // 如果 sid 已經走過 => 回傳"true" 
  for(int i = 0; i < visited.size(); i++){
    if(visited[i] == sid) return true;
  }
  return false;
  
} // isVisited() 

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

int Sid_Serial_Number(char sid[10]){
	int num;
	for(int i=0;i<rSet.size(); i++){
		if(strcmp(sid,rSet[i].sid)==0){
			num = i;
			break;
		}
	}
	
	return num;
}

int Dijkstra_Serial_Number(char sid[10]){
	int num1, num2;
	for(int i=0;i<cSet.size(); i++){
		for(int j=0; j < cSet[i].node.size(); j++){
			if(strcmp(sid,cSet[i].node[j].sid)==0){
				num1 = i;
				num2 = j;
				break;
			}
		}
	}
	
	return (num1, num2);
}
