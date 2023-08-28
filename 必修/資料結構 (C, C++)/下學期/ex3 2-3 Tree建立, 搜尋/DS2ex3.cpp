// DS2 ex3 "2-3 Tree"
// 10311245 �d�Ӿ� / 10520104 ���F��
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

typedef struct data{	//��ƫ��A 
	string value;		//key�� 
	vector<int> seq;	//�Ǹ��C 
}dataType;

typedef struct node{	//�`�I���A 
	
	dataType key[2];	// key��� 
	int keynum;			// key�ƶq 
	node* Parent;		// ���`�I pointer
	node* LeftChild;	// ���l�� pointer 
	node* MiddleChild;	// ���l�� pointer
	node* RightChild;	// �k�l�� pointer
	
} nodeType;

class tree23{
	public:
		
	//vector<collegeType> cSet;	//��󤺮e 
	vector<collegeType> rSet;	//�z�鷺�e 
	nodeType* root;		//�ڸ`�I 
	
	nodeType* lefttmp;	//�Ȧs 
	nodeType* righttmp; //�Ȧs 
	nodeType* n1;		//�Ȧs
	nodeType* n2;		//�Ȧs

	
	//**Fuction list
	int ReadFile(); 				//Ū�� 
	void ShowData();				//��ܥ����� 
	
	void SelectMatchedRecords(); 					//�d�� ( ���ȤG ) ** 
	void Bulid23Tree();								//�إ� 2-3�� ( ���Ȥ@ )
	void Insert(nodeType* now, int i);				//���J�`�I 
	void Spilt(nodeType* now, dataType mData);		//���� 
	void InitNewNode(nodeType* now);				//�s�W�`�I��l�� 
	nodeType* Search(nodeType* now, string value);	//��M�`�I 
	int Compare(nodeType* now, string value);		//��� 
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
		return ReadFile();	//�AŪ�ɤ@�� 
	}
	else{

		fin.ignore(255, '\n');	// �������e�T��r�� 
		fin.ignore(255, '\n');
		fin.ignore(255, '\n');
		
		collegeType oneC;
		
		
		string line;
		string oneStr;
		stringstream iss;
		int check;
		
		while( getline(fin, line, '\n') ){	//�@���^���@�� 
			check = 0;
			iss << line;
			while( getline(iss, oneStr, '\t') ){	//�b�^���X���r����Xtoken 
				
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
			iss.clear();	// *stringstream�ݲM�Ť~����kget�U�@�� 
		}

		fin.close();
		return 1;
	}
	
} // ReadFile()
	
void tree23::ShowData(){
	
	//��X�d�߫��ƶ��X
	cout << endl << "*** There are " << rSet.size() << " matched records, listed as below:" << endl;
	for(int i = 0; i < rSet.size(); i++){
		cout << "[" << i+1 << "]" << "\t" << rSet[i].cName << " " << rSet[i].dName << ", " << rSet[i].type << ", " << rSet[i].level << ", " << rSet[i].total << endl;
	}
	cout << "************************************************************" << endl;
	
} // ShowData()
	
void tree23::Bulid23Tree(){

	for(int i = 0; i < rSet.size(); i++){
		if(i == 0){				
			root = new nodeType;	//�Ĥ@�쪽�����J�ڸ`�I 
			InitNewNode(root);
			root->key[0].value = rSet[i].cName;
			root->key[0].seq.push_back(i);
		}
		else{
			nodeType* now = Search(root, rSet[i].cName);	//�d�ߥi���J���`�I 
			Insert(now, i);
		}

	}
		
	//��X�ڸ`�I���
		
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
			case 1: now->key[1] = now->key[0];	//case 1: �s�W�ƭ� < �`�I key[0]
					now->key[0].value = rSet[i].cName;
					now->key[0].seq.clear();
					now->key[0].seq.push_back(i);
					now->keynum++;
					break;
			case 3: now->key[1].value = rSet[i].cName;	//case 3: �s�W�ƭ� > �`�I key[0] 
					now->key[1].seq.push_back(i);
					now->keynum++;
					break;
			default:now->key[0].seq.push_back(i); //case 0: �s�W�ƭ� == �`�I key[0] 
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
		�Y�`�I�� key �u�� 1 ��: 
			case 1: �s�W�ƭ� < �`�I key[0]
			case 3: �s�W�ƭ� > �`�I key[0] 
			case 0: �s�W�ƭ� == �`�I key[0] 
			
		�Y�`�I�� key �� 2 ��: 
			case 1: �s�W�ƭ� < �`�I key[0]
			case 2: �`�I key[0] < �s�W�ƭ� < �`�I key[1]
			case 3: �s�W�ƭ� > �`�I key[1] 
			case 4: �s�W�ƭ� == �`�I key[1] 
			case 0: �s�W�ƭ� == �`�I key[0] 
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
	else if(node->keynum==1){	//���`�I�u���@��key 
		if(strcmp(goal.c_str(),node->key[0].value.c_str())>0 && node->RightChild!= NULL){	//�Y�j�M���e�j��`�I���e 
			Searching(node->RightChild, goal);	//���k�� 
		}
		else if(strcmp(goal.c_str(),node->key[0].value.c_str())<0 && node->LeftChild!= NULL) {	//�Y�j�M���e�p��`�I���e 
			Searching(node->LeftChild, goal);
		}
		else{cout << goal << " cannot find a match" << endl;}
	} 
	else{		//���`�I�����key
		if(strcmp(goal.c_str(),node->key[0].value.c_str())<0 && node->LeftChild!= NULL){Searching(node->LeftChild, goal);} //�j�M���e < �`�I key[0] 
		else if(strcmp(goal.c_str(),node->key[0].value.c_str())>0 && strcmp(goal.c_str(),node->key[1].value.c_str())<0 && node->MiddleChild!= NULL){Searching(node->MiddleChild, goal);} //case 2: �`�I key[0] < �j�M���e < �`�I key[1]
		else if(strcmp(goal.c_str(),node->key[1].value.c_str())>0 && node->RightChild!= NULL) {Searching(node->RightChild, goal);} //case 3: �j�M���e > �`�I key[1] 
		else if(strcmp(goal.c_str(),node->key[0].value.c_str())==0) {Search_Print(node->key[0].seq);}//case 0: �j�M���e == �`�I key[0]
		else if(strcmp(goal.c_str(),node->key[1].value.c_str())==0){Search_Print(node->key[1].seq);}//case 4: �j�M���e == �`�I key[1] 
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
			
			Tree2_3.Bulid23Tree();			// ���Ȥ@ 
			Tree2_3.SelectMatchedRecords();	//���ȤG 
			
			cout << endl << "[0]Quit or [AnyOther]Continue ?" << endl;
			getline(cin, cmd);
			Tree2_3.rSet.clear();
			
			if(!cmd.compare("0")) return 0;
			else read = Tree2_3.ReadFile();
		}
	}
	
	return 0;
}
