// DS2 ex2 
// 10311245 吳承曄 / 10520104 莊東翰
// 2018 / 03 / 22 

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>

using namespace std;

typedef struct cT{
	string cName;
	string dName;
	string type;
	string level;
	int total;
} collegeType;

typedef struct hT{
	int rid;
	int value;
} heapType;

typedef enum {MIN, MAX} whichHeap;

//**Grobal Value 
vector<collegeType> cSet;	//文件內容 
vector<collegeType> rSet;	//篩選內容 
vector<heapType> hSet;		//Heap

//**Fuction list
int ReadFile(); 					// 讀檔
void SelectMatchedRecords(); 		// 查詢 
void Build_MinMaxHeap();			// 建立 Min-max Heap (任務一) 
void SwapWithParent(int current);	// 確認父節點 
void MaxHeapify(int current);
void MinHeapify(int current);
void FindKMax();					// 查詢 K 個最大值 (任務二) 
void SwapWithChild(int current);	// 與子節點交換 
void ReHeapify(int current); 


int main(int argc, char** argv) {
	
	int read = ReadFile();
	
	string cmd;
	if(read){
		while(1){
			SelectMatchedRecords();
			system("pause");
			
			Build_MinMaxHeap();	//任務一
			system("pause");
			
			FindKMax();			//任務二 
			
			cout << endl << "[0]Quit or [AnyOther]Continue ?" << endl;
			getline(cin, cmd);
			//cin >> cmd;
			rSet.clear();
			hSet.clear();
			if(!cmd.compare("0")) return 0;
		}
	}
	
	
	return 0;
}

int ReadFile(){
	
	string fileName;
	cout << endl << "Input the file name: 201, 202 ... [0]Quit" << endl;
	getline(cin, fileName);
	//cin >> fileName;
	
	if(!fileName.compare("0")) return 0;	//quit
	
	fileName = "input" + fileName + ".txt";
	fstream fin(fileName.c_str());
	
	if(!fin){
		cout << "### " << fileName << " does not exist! ###" << endl;
		return ReadFile();	//再讀檔一次 
	}
	else{

		fin.ignore(255, '\n');	// 忽略文件前三行字串 
		fin.ignore(255, '\n');
		fin.ignore(255, '\n');
		
		collegeType oneC;
		
		
		string line;
		string oneStr;
		stringstream iss;
		int check;
		
		while( getline(fin, line, '\n') ){	//一次擷取一行 
			check = 0;
			iss << line;
			while( getline(iss, oneStr, '\t') ){	//在擷取出的字串分出token 
				
				switch(check){
					case 1: oneC.cName = oneStr;
							break;
					case 3: oneC.dName = oneStr;
							break;
					case 4: oneC.type = oneStr;
							break;
					case 5: oneC.level = oneStr; 
							break;
					case 8: oneC.total = atoi(oneStr.c_str());
							break;
					default: break;
				}
				check++;
				if(check == 11) cSet.push_back(oneC);
				
			}
			iss.clear();	// *stringstream需清空才有辦法get下一行 
		}

		fin.close();
		return 1;
	}
	
} // ReadFile()

void SelectMatchedRecords(){

	string coName, depName, coType, stLevel;
	
	cout << endl << "************************************************************" << endl;
	cout << "*** Mission One: Select Matched Records from a Text File ***" << endl;
 	cout << "************************************************************" << endl;
 	cout << endl << "Enter a keyword of 學校名稱: [*]for all" << endl;
 	getline(cin, coName);
 	//cin >> coName;
 	cout << endl << "Enter a keyword of 科系名稱: [*]for all" << endl;
 	getline(cin, depName);
	//cin >> depName;
 	cout << endl << "Enter a keyword of 日夜別: [*]for all" << endl;
 	getline(cin, coType);
 	//cin >> coType;
 	cout << endl << "Enter a keyword of 等級別: [*]for all" << endl;
 	getline(cin, stLevel);
 	//cin >> stLevel;
	
	if(coName == "*") coName = "";
	if(depName == "*") depName = "";
	if(coType == "*") coType = "";
	if(stLevel == "*") stLevel = "";

	//查詢比對		 *特別注意: [string.find()]有找到符合字串時 回傳值為"找到的字串位置" 
	for(int i = 0; i < cSet.size(); i++){	
		if( cSet[i].cName.find(coName) != string::npos && cSet[i].dName.find(depName) != string::npos && cSet[i].type.find(coType) != string::npos && cSet[i].level.find(stLevel) != string::npos){	
			rSet.push_back(cSet[i]);
		}
	}
	
	//輸出查詢後資料集合
	cout << endl << "*** There are " << rSet.size() << " matched records, listed as below:" << endl;
	for(int i = 0; i < rSet.size(); i++){
		cout << "[" << i+1 << "]" << "\t" << rSet[i].cName << " " << rSet[i].dName << ", " << rSet[i].type << ", " << rSet[i].level << ", " << rSet[i].total << endl;
	}
	cout << "************************************************************" << endl;
	
} // SelectMatchedRecords()


void Build_MinMaxHeap(){
	
	if(rSet.size() == 0) return ;
	
	int i = 0;
	heapType oneC;
	while(i < rSet.size()){		//逐筆加入 
		
		oneC.rid = i+1;
		oneC.value = rSet[i].total;		
		if(hSet.size() == 0){		 
			hSet.push_back(oneC);		//Heap從 1 的位置開始放 (在0的位置隨便放一個東西卡位) 
			hSet.push_back(oneC);
		}
		else{
			hSet.push_back(oneC);
			SwapWithParent(hSet.size()-1);	// 加入後第一步為"確認父節點" 
		}
		
		i++;
	}
	
	
	// 輸出MaxHeap資料
	cout << endl << "========================================================" << endl;
	cout << " Mission One: Min-max Heap Construction " << endl;
	cout << "========================================================" << endl;

	cout << "bottom = " << "[" << hSet[hSet.size()-1].rid << "] " << hSet[hSet.size()-1].value << endl;
	
	int k = 1;
	while(hSet.size() > k*2){
		k = k * 2;
	}
	cout << "leftmost bottom = " << "[" << hSet[k].rid << "] " << hSet[k].value << endl;
	
} // Build_MinMaxHeap()

void SwapWithParent(int current){
	
	// decide which heap
	int pos = 1;	// "1" 為陣列開始位置 
	int count = 0;	// 從2的 "0" 次方累加 
	while(pos * 2 <= current){
		pos = pos * 2;
		count++;
	}
	whichHeap level = (count % 2)? MAX:MIN;
	
	// 確認是否需要與父節點交換 
	int parent = current / 2;
	if(level == MAX){	
		// MAX
		if(hSet[current].value < hSet[parent].value){
			
			heapType tmp;
			tmp = hSet[current];
			hSet[current] = hSet[parent];
			hSet[parent] = tmp;
			
			MinHeapify(parent); //swap 
		}
		else{
			MaxHeapify(current); //nonswap
		}
	}
	else{	
		// MIN
		if(hSet[current].value > hSet[parent].value){
			
			heapType tmp;
			tmp = hSet[current];
			hSet[current] = hSet[parent];
			hSet[parent] = tmp;
			
			MaxHeapify(parent); //swap 
		}
		else{
			MinHeapify(current); //nonswap
		}
	}
	
} // SwapWithParent()

void MaxHeapify(int current){
	
	int grandparent = current / 4;
	if(grandparent >= 1){
		
		if(hSet[current].value > hSet[grandparent].value){
			
			heapType tmp;
			tmp = hSet[current];
			hSet[current] = hSet[grandparent];
			hSet[grandparent] = tmp;
			
			MaxHeapify(grandparent);
		}
		
	}
	
} // MaxHeapify()

void MinHeapify(int current){
	
	int grandparent = current / 4;
	if(grandparent >= 1){
		
		if(hSet[current].value < hSet[grandparent].value){
			
			heapType tmp;
			tmp = hSet[current];
			hSet[current] = hSet[grandparent];
			hSet[grandparent] = tmp;
			
			MinHeapify(grandparent);
		}

	}
	
} // MinHeapify()


void FindKMax(){
	
	string K;
	int k;
	cout << "Enter the value of K (0 - "<< hSet.size()-1 << "):";
	getline(cin, K);	
	
	k = atoi(K.c_str());	//**getline不可用於 int 所以必須轉值 
	
	cout << "========================================================" << endl;
	cout << "Mission 2: Top-K maximums from min-max heap (K=" << k << ")" << endl;
	cout << "========================================================" << endl;

	
	for(int count = 1; count <= k; count++){
		
		//找到最大值 
		int max;
		if(hSet.size() < 3)
			max = 1;
		else{
			max = 2;
			if(hSet[max+1].value > hSet[max].value) max = max+1;
		} 
		
		//輸出最大值資料 
		int target = hSet[max].rid - 1;
		cout << "TOP  " << count << ": [" << hSet[max].rid << "] " << rSet[target].cName << " " << rSet[target].dName << ", " << rSet[target].type << ", " << rSet[target].level << ", " << rSet[target].total << endl; 
	
		//移除最大值 
		heapType tmp;
		tmp = hSet[max];
		hSet[max] = hSet[hSet.size()-1];
		hSet[hSet.size()-1] = tmp;
		hSet.pop_back();
	
	
	
		SwapWithChild(max);	//與子節點比較 
		ReHeapify(max);		//最大堆積化 
		
	}
	
} // FindKMax() 

void SwapWithChild(int current){
	
	int child1 = current * 2;
	int child2 = (current * 2) + 1;
	
	if(child1 < hSet.size()){	//子節點是否存在 
	
		//找子節點最大者
		int target = child1;
		if( child2 < hSet.size() && hSet[child1].value < hSet[child2].value) 
			target = child2;
			
		// 確認是否需要與子節點交換 
		if(hSet[current].value < hSet[target].value){
			
			heapType tmp;
			tmp = hSet[current];
			hSet[current] = hSet[target];
			hSet[target] = tmp;
	
		} // swap 
			
	}
	
} // SwapWithChild()

void ReHeapify(int current){
	
	int child1 = (current * 2 * 2);
	int child2 = (current * 2 * 2) + 1;
	int child3 = ((current * 2 + 1) * 2);
	int child4 = ((current * 2 + 1) * 2) + 1;

	
	if(child1 < hSet.size()){
		
		int max = child1;
		if(child2 < hSet.size() && hSet[child2].value > hSet[max].value){
			max = child2;
		}
		if(child3 < hSet.size() && hSet[child3].value > hSet[max].value){
			max = child3;
		}
		if(child4 < hSet.size() && hSet[child4].value > hSet[max].value){
			max = child4;
		}
		
		//swap
		if(hSet[current].value < hSet[max].value){
		
			heapType tmp;
			tmp = hSet[current];
			hSet[current] = hSet[max];
			hSet[max] = tmp;
		
			ReHeapify(max);
		}
		
	}
	else{
		SwapWithChild(current);	//若沒有祖孫節點 則必須比較子節點 
	}
	
} // ReHeapify() 


