// DS2 ex3 "2-3 Tree"
// 10311245 吳承曄 / 10520104 莊東翰
// 2018 / 04 / 11 

#include <iostream>
#include <string.h>
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

typedef struct data{	//資料型態 
	string value;		//key值 
	vector<int> seq;	//序號列 
}dataType;

typedef struct node{	//節點型態 
	
	dataType key[2];	// key資料 
	int keynum;			// key數量 
	node* Parent;		// 父節點 pointer
	node* LeftChild;	// 左子樹 pointer 
	node* MiddleChild;	// 中子樹 pointer
	node* RightChild;	// 右子樹 pointer
	
} nodeType;

class tree23{
	public:
		
	//vector<collegeType> cSet;	//文件內容 
	vector<collegeType> rSet;	//篩選內容 
	nodeType* root;		//根節點 
	
	nodeType* lefttmp;	//暫存 
	nodeType* righttmp; //暫存 
	nodeType* n1;		//暫存
	nodeType* n2;		//暫存

	
	//**Fuction list
	int ReadFile(); 				//讀檔 
	void ShowData();				//顯示全體資料 
	
	void SelectMatchedRecords(); 					//查詢 ( 任務二 ) ** 
	void Bulid23Tree();								//建立 2-3樹 ( 任務一 )
	void Insert(nodeType* now, int i);				//插入節點 
	void Spilt(nodeType* now, dataType mData);		//分裂 
	void InitNewNode(nodeType* now);				//新增節點初始化 
	nodeType* Search(nodeType* now, string value);	//找尋節點 
	int Compare(nodeType* now, string value);		//比較 
	nodeType* Searching(nodeType* node,string goal);
	void Search_Print(vector<int> seq);
};
	
	
int tree23::ReadFile(){
	
	string fileName;
	cout << endl << "Input the file name: 301, 302 ... [0]Quit" << endl;
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
				if(check == 11) rSet.push_back(oneC);
				
			}
			iss.clear();	// *stringstream需清空才有辦法get下一行 
		}

		fin.close();
		return 1;
	}
	
} // ReadFile()
	
void tree23::ShowData(){
	
	//輸出查詢後資料集合
	cout << endl << "*** There are " << rSet.size() << " matched records, listed as below:" << endl;
	for(int i = 0; i < rSet.size(); i++){
		cout << "[" << i+1 << "]" << "\t" << rSet[i].cName << " " << rSet[i].dName << ", " << rSet[i].type << ", " << rSet[i].level << ", " << rSet[i].total << endl;
	}
	cout << "************************************************************" << endl;
	
} // ShowData()
	
void tree23::Bulid23Tree(){

	for(int i = 0; i < rSet.size(); i++){
		if(i == 0){				
			root = new nodeType;	//第一位直接插入根節點 
			InitNewNode(root);
			root->key[0].value = rSet[i].cName;
			root->key[0].seq.push_back(i);
		}
		else{
			nodeType* now = Search(root, rSet[i].cName);	//查詢可插入的節點 
			Insert(now, i);
		}

	}
		
	//輸出根節點資料
		
	cout << "########################################################" << endl;
	cout << "Mission 1: Build a 2-3 tree" << endl;
	cout << "########################################################" << endl; 
	
	if(root->keynum == 1){
		cout << root->key[0].value << endl;
		for(int k = 0; k < root->key[0].seq.size(); k++){
			cout << " " << k+1 << ":" ;
			cout << "[" << root->key[0].seq[k]+1 << "]" << "\t" << rSet[root->key[0].seq[k]].cName << " " 
				 << rSet[root->key[0].seq[k]].dName << ", " << rSet[root->key[0].seq[k]].type 
				 << ", " << rSet[root->key[0].seq[k]].level << ", " << rSet[root->key[0].seq[k]].total << endl;
		}
	}
	else{
		cout << root->key[0].value << endl;
		for(int k = 0; k < root->key[0].seq.size(); k++){
			cout << " " << k+1 << ":" ;
			cout << "[" << root->key[0].seq[k]+1 << "]" << "\t" << rSet[root->key[0].seq[k]].cName << " " 
				 << rSet[root->key[0].seq[k]].dName << ", " << rSet[root->key[0].seq[k]].type 
				 << ", " << rSet[root->key[0].seq[k]].level << ", " << rSet[root->key[0].seq[k]].total << endl;
		}
		cout << root->key[1].value << endl;
		for(int k = 0; k < root->key[1].seq.size(); k++){
			cout << " " << k+1 << ":" ;
			cout << "[" << root->key[1].seq[k]+1 << "]" << "\t" << rSet[root->key[1].seq[k]].cName << " " 
				 << rSet[root->key[1].seq[k]].dName << ", " << rSet[root->key[1].seq[k]].type 
				 << ", " << rSet[root->key[1].seq[k]].level << ", " << rSet[root->key[1].seq[k]].total << endl;
		}
	}
	
} // Bulid23Tree()
	
void tree23::Insert(nodeType* now, int i){
	int type = Compare(now, rSet[i].cName);
	if(now->keynum == 1){ // 2node
		switch(type){
			case 1: now->key[1] = now->key[0];	//case 1: 新增數值 < 節點 key[0]
					now->key[0].value = rSet[i].cName;
					now->key[0].seq.clear();
					now->key[0].seq.push_back(i);
					now->keynum++;
					break;
			case 3: now->key[1].value = rSet[i].cName;	//case 3: 新增數值 > 節點 key[0] 
					now->key[1].seq.push_back(i);
					now->keynum++;
					break;
			default:now->key[0].seq.push_back(i); //case 0: 新增數值 == 節點 key[0] 
					break;
		}
	}
	else{ // 3node
		dataType mData;
		switch(type){
			case 1: mData = now->key[0];
					now->key[0].value = rSet[i].cName;
					now->key[0].seq.push_back(i);
											
					lefttmp = new nodeType;
					InitNewNode(lefttmp);
					lefttmp->key[0] = now->key[0];
					righttmp = new nodeType;
					InitNewNode(righttmp);
					righttmp->key[0] = now->key[1];
	
					Spilt(now, mData);
					break;
						
			case 2: mData.value = rSet[i].cName;
					mData.seq.push_back(i);
					
					lefttmp = new nodeType;
					InitNewNode(lefttmp);
					lefttmp->key[0] = now->key[0];
					righttmp = new nodeType;
					InitNewNode(righttmp);
					righttmp->key[0] = now->key[1];
					
					Spilt(now, mData);
					break;
						
			case 3: mData = now->key[1];
					now->key[1].value = rSet[i].cName;
					now->key[1].seq.clear();
					now->key[1].seq.push_back(i);
				
					lefttmp = new nodeType;
					InitNewNode(lefttmp);
					lefttmp->key[0] = now->key[0];
					righttmp = new nodeType;
					InitNewNode(righttmp);
					righttmp->key[0] = now->key[1];
				
					Spilt(now, mData);
					break;
			case 4: now->key[1].seq.push_back(i);
					break;
			default:now->key[0].seq.push_back(i);
					break;
		}
	}
} // Insert()
	
void tree23::Spilt(nodeType* now, dataType mData){
	nodeType* parent = now->Parent;
	
	if(parent == NULL){	//new root
		parent = new nodeType;
		InitNewNode(parent);
		parent->key[0] = mData;
		lefttmp->Parent = parent;
		righttmp->Parent = parent;
		parent->LeftChild = lefttmp;
		parent->RightChild = righttmp;
			
		root = parent; // update root 
	} // if
	else{
		
		if(parent->keynum == 1){ //2node
			int type = Compare(parent, mData.value);
			switch(type){
				case 1: parent->key[1] = parent->key[0];
						parent->key[0] = mData;
						parent->keynum++;
						
						lefttmp->Parent = parent;
						righttmp->Parent = parent;
						parent->LeftChild = lefttmp;
						parent->MiddleChild = righttmp;
						break;
				case 3: parent->key[1] = mData;
						parent->keynum++;
						
						lefttmp->Parent = parent;
						righttmp->Parent = parent;
						parent->MiddleChild = lefttmp;
						parent->RightChild = righttmp;
						break;
					
				default:break;
			}
		} // end 2node
		else{ // 3node
			int type = Compare(parent, mData.value);
			dataType tmp;
			switch(type){
				case 1:	tmp = parent->key[0];
						parent->key[0] = mData;
						mData = tmp;
							
						n1 = new nodeType;
						InitNewNode(n1);
						n1->key[0] = parent->key[0];
						lefttmp->Parent = n1;
						righttmp->Parent = n1;
						n1->LeftChild = lefttmp;
						n1->RightChild = righttmp;
							
						n2 = new nodeType;
						InitNewNode(n2);
						n2->key[0] = parent->key[1];
						parent->MiddleChild->Parent = n2;
						parent->RightChild->Parent = n2;
						n2->LeftChild = parent->MiddleChild;
						n2->RightChild = parent->RightChild;
						
						lefttmp = n1;
						righttmp = n2;
						Spilt(parent, mData);
						break;
							
				case 2: mData = mData;
						n1 = new nodeType;
						InitNewNode(n1);
						n1->key[0] = parent->key[0];
						parent->LeftChild->Parent = n1;
						lefttmp->Parent = n1;
						n1->LeftChild = parent->LeftChild;
						n1->RightChild = lefttmp;
						
							
						n2 = new nodeType;
						InitNewNode(n2);
						n2->key[0] = parent->key[1];
						righttmp->Parent = n2;
						parent->RightChild->Parent = n2;
						n2->LeftChild = righttmp;
						n2->RightChild = parent->RightChild;
							
						lefttmp = n1;
						righttmp = n2;
						Spilt(parent, mData);
						break;
							
				case 3: tmp = parent->key[1];
						parent->key[1] = mData;
						mData = tmp;
						
						n1 = new nodeType;
						InitNewNode(n1);
						n1->key[0] = parent->key[0];
	
						parent->LeftChild->Parent = n1;
						parent->MiddleChild->Parent = n1;
						n1->LeftChild = parent->LeftChild;
						n1->RightChild = parent->MiddleChild;
	
						n2 = new nodeType;
						InitNewNode(n2);
						n2->key[0] = parent->key[1];
						lefttmp->Parent = n2;
						righttmp->Parent = n2;
						n2->LeftChild = lefttmp;
						n2->RightChild = righttmp;
						
						lefttmp = n1;
						righttmp = n2;
						
						Spilt(parent, mData);
						break;
					
				default:break;
			}
		} // end 3node
			
	} // else
	
} // Split()
	
void tree23::InitNewNode(nodeType* now){
	now->keynum = 1;
	now->Parent = NULL;
	now->LeftChild = NULL;
	now->MiddleChild = NULL;
	now->RightChild = NULL;
} // InitNewNode()
	
nodeType* tree23::Search(nodeType* now, string value){
		
	nodeType* pos = now;
	int type = Compare(now, value);
	
	switch(type){
		case 1: if(now->LeftChild != NULL) pos = Search(now->LeftChild, value);
				break;
		case 2: if(now->MiddleChild != NULL) pos = Search(now->MiddleChild, value);
				break;
		case 3: if(now->RightChild != NULL) pos = Search(now->RightChild, value);
				break;
		default: break;
	}
		
	return pos;
} // Search()
	
int tree23::Compare(nodeType* now, string value){
		
	/*			
		若節點的 key 只有 1 個: 
			case 1: 新增數值 < 節點 key[0]
			case 3: 新增數值 > 節點 key[0] 
			case 0: 新增數值 == 節點 key[0] 
			
		若節點的 key 有 2 個: 
			case 1: 新增數值 < 節點 key[0]
			case 2: 節點 key[0] < 新增數值 < 節點 key[1]
			case 3: 新增數值 > 節點 key[1] 
			case 4: 新增數值 == 節點 key[1] 
			case 0: 新增數值 == 節點 key[0] 
	*/
		
	if(now->keynum == 1){
		if(value < now->key[0].value) return 1;
		else if(value > now->key[0].value) return 3;
		else return 0;
	}
	else{
		if(value < now->key[0].value) return 1;
		else if(value > now->key[0].value && value < now->key[1].value) return 2;
		else if(value > now->key[1].value) return 3;
		else if (value == now->key[0].value) return 0;
		else return 4;
	}
			
} // Compare()
	
void tree23::SelectMatchedRecords(){
	cout << endl;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << "Mission 2: Search a 2-3 tree (Search key is college name)" << endl;	
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	
	string goal;
	cout << "Enter a name to search: " << endl; 
	cin >> goal;
	Searching(root,goal);
		// to do 
	
} // SelectMatchedRecords()

nodeType* tree23::Searching(nodeType* node, string goal){
	if(node->keynum==1 && strcmp(goal.c_str(),node->key[0].value.c_str())==0){
			Search_Print(node->key[0].seq);
	}
	else if(node->keynum==1){	//此節點只有一個key 
		if(strcmp(goal.c_str(),node->key[0].value.c_str())>0 && node->RightChild!= NULL){	//若搜尋內容大於節點內容 
			Searching(node->RightChild, goal);	//往右走 
		}
		else if(strcmp(goal.c_str(),node->key[0].value.c_str())<0 && node->LeftChild!= NULL) {	//若搜尋內容小於節點內容 
			Searching(node->LeftChild, goal);
		}
		else{cout << goal << " cannot find a match" << endl;}
	} 
	else{		//此節點有兩個key
		if(strcmp(goal.c_str(),node->key[0].value.c_str())<0 && node->LeftChild!= NULL){Searching(node->LeftChild, goal);} //搜尋內容 < 節點 key[0] 
		else if(strcmp(goal.c_str(),node->key[0].value.c_str())>0 && strcmp(goal.c_str(),node->key[1].value.c_str())<0 && node->MiddleChild!= NULL){Searching(node->MiddleChild, goal);} //case 2: 節點 key[0] < 搜尋內容 < 節點 key[1]
		else if(strcmp(goal.c_str(),node->key[1].value.c_str())>0 && node->RightChild!= NULL) {Searching(node->RightChild, goal);} //case 3: 搜尋內容 > 節點 key[1] 
		else if(strcmp(goal.c_str(),node->key[0].value.c_str())==0) {Search_Print(node->key[0].seq);}//case 0: 搜尋內容 == 節點 key[0]
		else if(strcmp(goal.c_str(),node->key[1].value.c_str())==0){Search_Print(node->key[1].seq);}//case 4: 搜尋內容 == 節點 key[1] 
		else{cout << goal << " cannot find a match" << endl;}
	}
	
} 

void tree23::Search_Print(vector<int> seq){
	for(int i=0;i<seq.size();i++){
		cout << "[" << i+1 << "]" << "\t" << rSet[seq[i]].cName << " " << rSet[seq[i]].dName << ", " << rSet[seq[i]].type << ", " << rSet[seq[i]].level << ", " << rSet[seq[i]].total << endl;
	}
}
		

int main(int argc, char** argv) {	
	tree23 Tree2_3;
	int read = Tree2_3.ReadFile();
 
	string cmd;
	if(read){
		while(1){
			Tree2_3.ShowData();
			system("pause");
			
			Tree2_3.Bulid23Tree();			// 任務一 
			Tree2_3.SelectMatchedRecords();	//任務二 
			
			cout << endl << "[0]Quit or [AnyOther]Continue ?" << endl;
			getline(cin, cmd);
			Tree2_3.rSet.clear();
			
			if(!cmd.compare("0")) return 0;
			else read = Tree2_3.ReadFile();
		}
	}
	
	return 0;
}
