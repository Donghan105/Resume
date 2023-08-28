//10724370 馮浩陽, 10520104莊東翰 
#include <iostream>
#include <stdio.h>
#include <string.h> //string型別, strcpy, assign, substr, erase, find, erase, strstr 
#include <vector>  //動態陣列 
#include <stdlib.h> // atoi 
#include <fstream>
#include <iomanip> //cout 小數點後幾位  
#include<time.h>
#include <algorithm> //reverse
using namespace std;

struct CustomType{
	string school_id;	//學校代碼 
	string school_name;	//學校名稱 
	int department_id;	//科系代碼 
	string department_name;	//科系名稱 
	string day;	//日間∕進修別 
	string level;	//等級別 
	int students;	//學生數 
	int teachers;	//教師數 
	int graduates;	//上學年度畢業生數 
	string city;	//縣市名稱 
	string system;	//體系別 
};

struct NodeType{
	CustomType data;
	int tree_height;
	NodeType *left;
	NodeType *right;
	NodeType *parent;
};

struct M3Type{
	NodeType *root_s;
	NodeType *root_g;
};

string readfile();
void clear_nodes(NodeType*);
void clear_all(NodeType*, NodeType*);
void Print_list(vector<CustomType>&);
void Print_tree(NodeType*);
CustomType copy_list(int);
void bulid_tree_graduates(NodeType*);
void bulid_tree_school_name(NodeType*);
void GTE_graduates(NodeType*, int);//Greater Than or Equal
void Same_school_name(NodeType*, string);//Greater Than or Equal

NodeType* find_inorder_successor(NodeType*);
NodeType* del_node(NodeType*);
void del_Same_school_name(NodeType*, string);
NodeType* del_GTE_graduates(NodeType*, int);
NodeType* del_graduates(NodeType*, int, string);
M3Type mission3(NodeType*, NodeType*, string);
void print_tree_height(NodeType *root);
void cal_tree_height(NodeType *, int);

vector<CustomType> SchoolList;
int height; //tree height

string readfile(){
	ifstream infile;//(#include <fstream>), ifstream = fstream中的"in" 
	string filename, str;
	cout << "Please input your file name (ex: 601, 602...): ";
	cin >> filename;
	filename = "input" + filename + ".txt";
	cout << endl;
	infile.open(filename.c_str());	//.c_str()用來將string 轉成 char[XXX], 因為open函數只能讀char[XXX], 但string撰寫比較方便
	
	if(infile==NULL){
		cout << "Fail to open " << filename << endl;
		return "0";
	}
	else{
		cout << "完成開啟" << filename << endl; 
		for(int i=0;i<15;i++){//讀掉垃圾  
			infile >> str;
		}
		
		int flag = 1;
		CustomType element;
		while(infile >> str){	//while( infile >> ch)	//當檔案中還有東西可以讀取, >>表示從檔案將資料傳到變數中
			if(flag==1){
				element.school_id = str; //string to char* to int	
			}	
			else if(flag==2){
				element.school_name = str;
			}
			else if(flag==3){
				element.department_id = atoi(str.c_str()); //string to char* to int
			}
			else if(flag==4){
				element.department_name = str;
			}
			else if(flag==5){
				string tmp;
				tmp = str;
				infile >> str;
				tmp = tmp + " " + str;
				element.day = tmp;
			}
			else if(flag==6){
				string tmp;
				tmp = str;
				infile >> str;
				tmp = tmp + " " + str;
				element.level = tmp;
			}
			else if(flag==7){
				element.students = atoi(str.c_str()); //string to char* to int
			}
			else if(flag==8){
				element.teachers = atoi(str.c_str()); //string to char* to int
			}
			else if(flag==9){
				element.graduates = atoi(str.c_str()); //string to char* to int
			}
			else if(flag==10){
				string tmp;
				tmp = str;
				infile >> str;
				tmp = tmp + " " + str;
				element.city = tmp;
			}
			else if(flag==11){
				string tmp;
				tmp = str;
				infile >> str;
				tmp = tmp + " " + str;
				element.system = tmp;
				SchoolList.push_back(element);
				flag = 0;
			}
			flag++;
		}
		//Print_m2(SchoolList);
		return filename;
	}
}

void clear_nodes(NodeType *cur){
	if(cur->right!=NULL){
		clear_nodes(cur->right);
	}
	
	if(cur->left!=NULL){
		clear_nodes(cur->left);
	}	
	//cout << "delete" << endl;
	//cout << "學校代碼: " << cur->data.school_id << ", 學校名稱: " << cur->data.school_name << ", 科系代碼: " << cur->data.department_id << ", 科系名稱: "  << cur->data.department_name << endl;
	//cout << "日間∕進修別: "  << cur->data.day << ", 等級別: " << cur->data.level << ", 學生數: " << cur->data.students << ", 教師數: " << cur->data.teachers << ", 上學年度畢業生數: " << cur->data.graduates << ", 縣市名稱: " << cur->data.city << ", 體系別: " << cur->data.system << endl;
	delete cur;
}

void clear_all(NodeType *root1, NodeType *root2){
	SchoolList.clear();
	clear_nodes(root1);
	//cout << endl;
	clear_nodes(root2);
	height = 0;
}

void Print_list(vector<CustomType>&List){
	for(int i=0;i<List.size();i++){
		cout << "[" << i+1 << "]\t";
		cout << List[i].school_name << "\t"  << List[i].department_name;
		cout << "\t"  << List[i].day << "\t" << List[i].level << "\t" << List[i].students << "\t" << List[i].teachers << "\t" << List[i].graduates << endl;
		
		//cout << "學校代碼: " << List[i].school_id << ", 學校名稱: " << List[i].school_name << ", 科系代碼: " << List[i].department_id << ", 科系名稱: "  << List[i].department_name << endl;
		//cout << "日間∕進修別: "  << List[i].day << ", 等級別: " << List[i].level << ", 學生數: " << List[i].students << ", 教師數: " << List[i].teachers << ", 上學年度畢業生數: " << List[i].graduates << ", 縣市名稱: " << List[i].city << ", 體系別: " << List[i].system << endl;
	}
}

void Print_tree(NodeType *cur){	//大到小  RAL 遞迴  
	if(cur->right!=NULL){Print_tree(cur->right);}
	
	cout << cur->data.school_name << "\t"  << cur->data.department_name;
	cout << "\t"  << cur->data.day << "\t" << cur->data.level << "\t" << cur->data.students << "\t" << cur->data.teachers << "\t" << cur->data.graduates << endl;
	
	//cout << "學校代碼: " << cur->data.school_id << ", 學校名稱: " << cur->data.school_name << ", 科系代碼: " << cur->data.department_id << ", 科系名稱: "  << cur->data.department_name << endl;
	//cout << "日間∕進修別: "  << cur->data.day << ", 等級別: " << cur->data.level << ", 學生數: " << cur->data.students << ", 教師數: " << cur->data.teachers << ", 上學年度畢業生數: " << cur->data.graduates << ", 縣市名稱: " << cur->data.city << ", 體系別: " << cur->data.system << endl;
	
	if(cur->left!=NULL){Print_tree(cur->left);}
}

CustomType copy_list(int i){
	CustomType tmp;
	
	tmp.city = SchoolList[i].city;
	tmp.day = SchoolList[i].day;
	tmp.department_id = SchoolList[i].department_id;
	tmp.department_name = SchoolList[i].department_name;
	tmp.graduates = SchoolList[i].graduates;
	tmp.level = SchoolList[i].level;
	tmp.school_id = SchoolList[i].school_id;
	tmp.school_name = SchoolList[i].school_name;
	tmp.students = SchoolList[i].students;
	tmp.system = SchoolList[i].system;
	tmp.teachers = SchoolList[i].teachers;
	
	return tmp;
}

void bulid_tree_graduates(NodeType *root){
	int tree_height;
	NodeType *new_node, *cur;
	for(int i=0;i<SchoolList.size();i++){
		if(root->tree_height == 0){
			root->data = copy_list(i);
			root->tree_height = 1;
			root->parent = NULL;
			root->left = NULL;
			root->right = NULL;
			
			tree_height = root->tree_height;// tree height
		}
		else{
			new_node = new NodeType;
			new_node->data = copy_list(i);
			new_node->left = NULL;
			new_node->right = NULL;
			
			cur = root;	//root
			while(1){
				if(new_node->data.graduates < cur->data.graduates){	//<
					if(cur->left==NULL){
						cur->left = new_node;	//add
						new_node->parent = cur;
						new_node->tree_height = cur->tree_height + 1;
						if(new_node->tree_height > tree_height){tree_height = new_node->tree_height;}// tree height
						
						break;	//只break while 
					}
					else{
						cur = cur->left;
					}
				}
				else{	//>=
					if(cur->right==NULL){
						cur->right = new_node;	//add
						new_node->parent = cur;
						new_node->tree_height = cur->tree_height + 1;
						if(new_node->tree_height > tree_height){tree_height = new_node->tree_height;}// tree height
						
						break;	//只break while 
					}
					else{
						cur = cur->right;
					}
				}
			}
		}
	}
	cout << "graduates tree height: " << tree_height << endl;
}

void bulid_tree_school_name(NodeType *root){
	int tree_height;
	NodeType *new_node, *cur;
	for(int i=0;i<SchoolList.size();i++){
		if(root->tree_height == 0){
			root->data = copy_list(i);
			root->tree_height = 1;
			root->parent = NULL;
			root->left = NULL;
			root->right = NULL;
			
			tree_height = root->tree_height;// tree height
		}
		else{
			new_node = new NodeType;
			new_node->data = copy_list(i);
			new_node->left = NULL;
			new_node->right = NULL;
			
			cur = root;	//root
			while(1){
				if(new_node->data.school_name < cur->data.school_name){	// <
					if(cur->left==NULL){
						cur->left = new_node;	//add
						new_node->parent = cur;
						new_node->tree_height = cur->tree_height + 1;
						if(new_node->tree_height > tree_height){tree_height = new_node->tree_height;}// tree height
						
						break;	//只break while 
					}
					else{
						cur = cur->left;
					}
				}
				else{	// >=
					if(cur->right==NULL){
						cur->right = new_node;	//add
						new_node->parent = cur;
						new_node->tree_height = cur->tree_height + 1;
						if(new_node->tree_height > tree_height){tree_height = new_node->tree_height;}// tree height
						
						break;	//只break while 
					}
					else{
						cur = cur->right;
					}
				}
			}
		}
	}
	cout << "school names tree height: " << tree_height << endl;
}

void GTE_graduates(NodeType *cur, int graduates){//Greater Than or Equal
	if(cur->data.graduates >= graduates){
		if(cur->right!=NULL && cur->right->data.graduates >= graduates){GTE_graduates(cur->right, graduates);}
		
		cout << cur->data.school_name << "\t"  << cur->data.department_name;
		cout << "\t"  << cur->data.day << "\t" << cur->data.level << "\t" << cur->data.students << "\t" << cur->data.teachers << "\t" << cur->data.graduates << endl;
		//cout << "學校代碼: " << cur->data.school_id << ", 學校名稱: " << cur->data.school_name << ", 科系代碼: " << cur->data.department_id << ", 科系名稱: "  << cur->data.department_name << endl;
		//cout << "日間∕進修別: "  << cur->data.day << ", 等級別: " << cur->data.level << ", 學生數: " << cur->data.students << ", 教師數: " << cur->data.teachers << ", 上學年度畢業生數: " << cur->data.graduates << ", 縣市名稱: " << cur->data.city << ", 體系別: " << cur->data.system << endl;
		
		if(cur->left!=NULL){GTE_graduates(cur->left, graduates);}
	}
	else if(cur->right!=NULL){GTE_graduates(cur->right, graduates);}

}

void Same_school_name(NodeType *cur, string school_name){//same school name
	int flag = -1; 
	while(1){
		if(cur->data.school_name == school_name){
			flag = 0;
			cout << cur->data.school_name << "\t"  << cur->data.department_name;
			cout << "\t"  << cur->data.day << "\t" << cur->data.level << "\t" << cur->data.students << "\t" << cur->data.teachers << "\t" << cur->data.graduates << endl;
			//cout << "學校代碼: " << cur->data.school_id << ", 學校名稱: " << cur->data.school_name << ", 科系代碼: " << cur->data.department_id << ", 科系名稱: "  << cur->data.department_name << endl;
			//cout << "日間∕進修別: "  << cur->data.day << ", 等級別: " << cur->data.level << ", 學生數: " << cur->data.students << ", 教師數: " << cur->data.teachers << ", 上學年度畢業生數: " << cur->data.graduates << ", 縣市名稱: " << cur->data.city << ", 體系別: " << cur->data.system << endl;
		}
		
		if(cur->data.school_name <= school_name){
			cur = cur->right;	
		}
		else if(cur->data.school_name > school_name){
			cur = cur->left;
		}

		if(cur==NULL){
			break;
		}
	}
	if(flag == -1){
		cout << "學校找不到哦" << endl; 
	}
}

NodeType* find_inorder_successor(NodeType *cur){
	cur = cur->right;
	while(cur->left != NULL){
		cur = cur->left;
	}
	
	return cur;
}

NodeType* del_node(NodeType *cur){ //3 cases
	NodeType *result;
	
	if(cur->right==NULL && cur->left==NULL){	//case 1 leaf, 直接刪除 
		if(cur->parent==NULL){ //刪除最後根結點  
			cur->left = NULL;
			cur->right = NULL;
			cur->parent = NULL;
			cur->tree_height = 0;
			cur->data.city = "dummy";
			cur->data.day = "dummy";
			cur->data.department_id = -1;
			cur->data.department_name = "dummy";
			cur->data.graduates = -1;
			cur->data.level = "dummy";
			cur->data.school_id = "dummy";
			cur->data.school_name = "dummy";
			cur->data.students = -1;
			cur->data.system = "dummy";
			cur->data.teachers = -1;
			return cur;
		}	//root
		else if(cur->parent->left == cur){	//cur在parent左側 
			cout << "1" << endl;
			result = cur->parent;
			cur->parent->left = NULL;
			delete cur;
			return result;
		}
		else{	//cur在parent右側 
			cout << "2" << endl;
			result = cur->parent;
			cur->parent->right = NULL;
			delete cur;
			return result;
		}
	}
	else if(cur->right!=NULL && cur->left!=NULL){	//case 2 two children, find inorder succesor
		NodeType *inorder_successor;
		cout << "3" << endl;
		inorder_successor = find_inorder_successor(cur);
		if(inorder_successor!=cur->right){	//if inorder_successor!=cur->right
			if(inorder_successor->right!=NULL){	//if inorder successor has a child(right)
				if(cur->parent==NULL){
					cout << "4" << endl; 
					cur->right->parent = inorder_successor;
					cur->left->parent = inorder_successor;
					inorder_successor->parent->left = inorder_successor->right;	//這裡與no child不同  
					inorder_successor->right->parent = inorder_successor->parent;//這裡與no child不同  
					
					inorder_successor->parent = NULL;
					inorder_successor->right = cur->right;
					inorder_successor->left = cur->left;	
				}	//root
				else if(cur->parent->left == cur){	//cur在parent左側 
					cout << "5" << endl;
					cur->right->parent = inorder_successor;
					cur->left->parent = inorder_successor;
					cur->parent->left = inorder_successor;
					inorder_successor->parent->left = inorder_successor->right;	//這裡與no child不同  
					inorder_successor->right->parent = inorder_successor->parent;//這裡與no child不同  
					
					inorder_successor->right = cur->right;
					inorder_successor->left = cur->left;
				}
				else{	//cur在parent右側 
					cout << "6" << endl;
					cur->right->parent = inorder_successor;
					cur->left->parent = inorder_successor;
					cur->parent->right = inorder_successor;
					inorder_successor->parent->left = inorder_successor->right;	//這裡與no child不同  
					inorder_successor->right->parent = inorder_successor->parent;//這裡與no child不同  
					
					inorder_successor->right = cur->right;
					inorder_successor->left = cur->left;	
				}
				delete cur;
			}
			else{	//inorder successor dosen't have a child
				if(cur->parent==NULL){
					cout << "7" << endl;
					cur->right->parent = inorder_successor;
					cur->left->parent = inorder_successor;
					inorder_successor->parent->left = NULL; 
					
					inorder_successor->parent = NULL;
					inorder_successor->right = cur->right;
					inorder_successor->left = cur->left;
				}	//root
				else if(cur->parent->left == cur){	//cur在parent左側 
					cout << "8" << endl;
					cur->right->parent = inorder_successor;
					cur->left->parent = inorder_successor;
					cur->parent->left = inorder_successor;
					inorder_successor->parent->left = NULL;
					
					inorder_successor->right = cur->right;
					inorder_successor->left = cur->left;
				}
				else{	//cur在parent右側 
					cout << "9" << endl;
					cur->right->parent = inorder_successor;
					cur->left->parent = inorder_successor;
					cur->parent->right = inorder_successor;
					inorder_successor->parent->left = NULL;
					
					inorder_successor->right = cur->right;
					inorder_successor->left = cur->left;	
				}
				delete cur;
			}	
		}
		else{// if inorder_successor==cur->right, inorder_successor有無child沒差 
			if(cur->parent==NULL){
				cout << "10" << endl;
				cur->left->parent = inorder_successor;
				
				inorder_successor->left = cur->left;
				inorder_successor->parent = NULL;
			}	//root 
			else if(cur->parent->left == cur){	//cur在parent左側 
				cout << "11" << endl;
				cur->left->parent = inorder_successor;
				cur->parent->left = inorder_successor;
					
				inorder_successor->left = cur->left;
			}
			else{	//cur在parent右側 
				cout << "12" << endl;
				cur->left->parent = inorder_successor;
				cur->parent->right = inorder_successor;
					
				inorder_successor->left = cur->left;	
			}
			delete cur;	
		}
		
		result = inorder_successor;
		return result;
	}
	else{	//case 3 one child, 往上接起來 
		if(cur->parent==NULL){
			if(cur->left!=NULL){	//在cur左側 
				cur->left->parent = NULL;	//重新接parent 
				result = cur->left;
				cout << "ohohoh: " << result->data.graduates;
				delete cur;
			}
			else{	//在cur右側
				cur->right->parent = NULL;	//重新接parent 
				result = cur->right;
				cout << "nonooo: " << result->data.graduates;
				delete cur;
			}
			delete cur;
		}	//root
		else if(cur->parent->left == cur){	//cur在parent左側 
			if(cur->left!=NULL){	//在cur左側 
				cout << "13" << endl; 
				cur->parent->left = cur->left;
				cur->left->parent = cur->parent;	//重新接parent 
				result = cur->left;
			}
			else{	//在cur右側
				cout << "14" << endl;
				cur->parent->left = cur->right;
				cur->right->parent = cur->parent;	//重新接parent 
				result = cur->right;
			}
			delete cur;
		}
		else{	//cur在parent右側 
			if(cur->left!=NULL){	//在cur左側
				cout << "15" << endl; 
				cur->parent->right = cur->left;
				cur->left->parent = cur->parent;	//重新接parent 
				result = cur->left;
			}
			else{	//在cur右側
				cout << "16" << endl;
				cur->parent->right = cur->right;
				cur->right->parent = cur->parent;	//重新接parent 
				result = cur->right;
			}
			delete cur;
		}
		
		return result;
	}
}

NodeType* del_GTE_graduates(NodeType *root, int graduates){//delete Greater Than or Equal
	int flag = 0;
	NodeType *cur = root, *tmp;
	while(1){
		cout << "123123" << endl;
		cout << "del: " << cur->data.graduates << endl; 
		if(cur->data.graduates >= graduates){
			if(cur==root){
				cout <<"a" <<endl;
				tmp = del_node(cur);
				cur = tmp;
				root = tmp;
				cout << "root替代node: " << cur->data.graduates << endl;
				if(cur==NULL){break;}
			}
			else{
				cout <<"b" <<endl;
				tmp = del_node(cur);
				cout << "替代node: " << tmp->data.graduates << endl;
				cur = tmp;
				if(cur->right==NULL){break;}
				else{
					cur = root;
				}
			}
		}
		else if(cur->data.graduates < graduates && cur->right==NULL){
			break;
		}
		else{
			cur = cur->right;
		}
		cout << endl;	
	}
	return root;
}

NodeType* del_graduates(NodeType* root, int graduates, string school_name){
	NodeType *cur = root, *tmp;
	while(1){
		cout << "123123" << endl;
		cout << "del: " << cur->data.graduates << endl; 
		if(cur->data.graduates == graduates && cur->data.school_name == school_name){
			if(cur==root){
				cout <<"a" <<endl;
				tmp = del_node(cur);
				root = tmp;
				cout << "root替代node: " << cur->data.graduates << endl;
				break;
			}
			else{
				cout <<"b" <<endl;
				tmp = del_node(cur);
				cout << "替代node: " << tmp->data.graduates << endl;
				cur = tmp;
				break;
			}
		}
		else if(cur->data.graduates < graduates && cur->right==NULL){
			break;
		}
		else if(cur->data.graduates > graduates && cur->left==NULL){
			break;
		}
		else if(cur->data.graduates < graduates){
			cur = cur->right;
		}
		else if(cur->data.graduates > graduates){
			cur = cur->left;
		}
		cout << endl;	
	}
	return root;
	
}

void print_tree_height(NodeType *root){
	NodeType *tmp;
	int tree_height = -1;
	root->tree_height = 1;
	cal_tree_height(root, 1);
	
}

void cal_tree_height(NodeType *cur, int num){	//大到小  RAL 遞迴 
	if(height<num){height = num;}
	 
	if(cur->right!=NULL){cal_tree_height(cur->right, num+1);}
	
	if(cur->left!=NULL){cal_tree_height(cur->left, num+1);}
}

void del_Same_school_name(NodeType *root, string school_name){//delete same school name
	int flag = -1; 
	NodeType *cur = root;
	while(1){
		cout << cur->data.school_name << endl;
		if(cur->data.school_name == school_name){
			flag = 0;
			del_node(cur);
			cur = root;
		}
		else{
			if(cur->data.school_name < school_name){
				cur = cur->right;
				if(cur==NULL){break;}
			}
			else if(cur->data.school_name > school_name){
				cur = cur->left;
				if(cur==NULL){break;}
			}	
		}
	}
	if(flag == -1){
		cout << "學校找不到哦" << endl; 
	}
}


M3Type mission3(NodeType *root_s, NodeType *root_g, string school_name){//delete Greater Than or Equal
	M3Type m3;
	int graduates;
	NodeType *cur = root_s, *tmp;
	while(1){
		cout << "now: " << cur->data.school_name << endl; 
		if(cur->data.school_name == school_name){
			if(cur==root_s){
				cout <<"a" <<endl;
				root_g = del_graduates(root_g, cur->data.graduates, school_name);
				tmp = del_node(cur);
				cur = tmp;
				root_s = tmp;
				cout << "root替代node: " << cur->data.school_name << endl;
			}
			else{
				cout <<"b" <<endl;
				root_g = del_graduates(root_g, cur->data.graduates, school_name);
				tmp = del_node(cur);
				cout << "替代node: " << tmp->data.school_name << endl;
				cur = root_s;

			}
		}
		else if(cur->data.school_name < school_name && cur->right==NULL){
			break;
		}
		else if(cur->data.school_name > school_name && cur->left==NULL){
			break;
		}
		else if(cur->data.school_name < school_name){
			cur = cur->right;
		}
		else if(cur->data.school_name > school_name){
			cur = cur->left;
		}
		cout << endl;	
	}
	
	m3.root_g = root_g;
	m3.root_s = root_s;
	return m3;
}

int main(int argc, char** argv){
	int cmd = -1;
	NodeType *root_g = NULL, *root_s = NULL;
	while(cmd!=0){
		cout << "Input your command(0:quit, 1:任務一, 2:任務二, 3:挑戰):";
		cin >> cmd;
		if(cmd==1){
			if(root_g != NULL && root_s != NULL){
				clear_all(root_g, root_s);
			}
			
			string filename;
			filename = readfile();
			cout << endl;
			
			if(filename != "0"){
				Print_list(SchoolList);
				
				root_g = new NodeType;	//new dummy 
				root_s = new NodeType;	//new dummy
				root_g->tree_height = 0;
				root_s->tree_height = 0;
//				dummy_g->left = NULL;
//				dummy_g->right = NULL;
//				dummy_g->parent = NULL;
//				dummy_g->tree_height = -1;
//				dummy_g->data.city = "dummy";
//				dummy_g->data.day = "dummy";
//				dummy_g->data.department_id = -1;
//				dummy_g->data.department_name = "dummy";
//				dummy_g->data.graduates = -1;
//				dummy_g->data.level = "dummy";
//				dummy_g->data.school_id = "dummy";
//				dummy_g->data.school_name = "dummy";
//				dummy_g->data.students = -1;
//				dummy_g->data.system = "dummy";
//				dummy_g->data.teachers = -1;
//				
//				dummy_s->left = NULL;
//				dummy_s->right = NULL;
//				dummy_s->parent = NULL;
//				dummy_s->tree_height = -1;
//				dummy_s->data.city = "dummy";
//				dummy_s->data.day = "dummy";
//				dummy_s->data.department_id = -1;
//				dummy_s->data.department_name = "dummy";
//				dummy_s->data.graduates = -1;
//				dummy_s->data.level = "dummy";
//				dummy_s->data.school_id = "dummy";
//				dummy_s->data.school_name = "dummy";
//				dummy_s->data.students = -1;
//				dummy_s->data.system = "dummy";
//				dummy_s->data.teachers = -1;
//				cout << endl;
				
				bulid_tree_graduates(root_g);
				//Print_tree(dummy_g->right);
				//cout << endl;
				
				bulid_tree_school_name(root_s);
				//Print_tree(dummy_s->right);
				//cout << endl;
			}
		}
		else if(cmd==2){
			if(root_g==NULL && root_s==NULL){
				cout << "請先執行任務一" << endl;
			}
			
			if(root_g!=NULL){
				int graduates;
				cout << "輸入去年畢業人數:";
				cin >> graduates;
				cout << endl;
				GTE_graduates(root_g, graduates);
				cout << endl;
			}
			if(root_s!=NULL){
				string school_name;
				cout << "輸入學校名稱:";
				cin >> school_name;
				cout << endl;
				Same_school_name(root_s, school_name);
				cout << endl;
			}
		}
		else if(cmd==3){
			if(root_g==NULL && root_s==NULL){
				cout << "請先執行任務一" << endl;
			}
			
//			if(root_g!=NULL){
//				int graduates;
//				cout << "輸入刪除大於等於去年畢業人數:";
//				cin >> graduates;
//				cout << endl;
//				root_g = del_GTE_graduates(root_g, graduates);
//				Print_tree(root_g);
//				cout << endl;
//			}
			if(root_s!=NULL){
				string school_name;
				Print_tree(root_s);
				cout << "輸入刪除學校名稱:";
				cin >> school_name;
				cout << endl;
				//del_Same_school_name(root_s, school_name);
				M3Type m3;
				m3 = mission3(root_s, root_g, school_name);
				root_s = m3.root_s;
				root_g = m3.root_g;
				cout << "school names tree:" << endl;
				Print_tree(root_s);
				cout << endl;
				cout << "graduates tree:" << endl;
				Print_tree(root_g);
				cout << endl;
				
				height = 0;
				print_tree_height(root_s);
				cout << "school names tree height: " << height << endl;
				height = 0;
				print_tree_height(root_g);
				cout << "graduates tree height: " << height << endl;

			}
		}
		else if(cmd==0){
			break;
		}
		else{
			cout << "請輸入0, 1, 2 or 3" << endl;
			cout << endl;
		}

		cout << "continue? (0:No, else:Yes):";
		cin >> cmd;
		cout << endl; 
	}

	clear_all(root_g, root_s);
	system("pause");
	
	return 0;
}


	//tmp.city = A.city;
	//tmp.day = A.day;
	//tmp.department_id = A.department_id;
	//tmp.department_name = A.department_name;
	//tmp.graduates = A.graduates;
	//tmp.level = A.level;
	//tmp.school_id = A.school_id;
	//tmp.school_name = A.school_name;
	//tmp.students = A.students;
	//tmp.system = A.system;
	//tmp.teachers = A.teachers;
	
	//A.city = B.city;
	//A.day = B.day;
	//A.department_id = B.department_id;
	//A.department_name = B.department_name;
	//A.graduates = B.graduates;
	//A.level = B.level;
	//A.school_id = B.school_id;
	//A.school_name = B.school_name;
	//A.students = B.students;
	//A.system = B.system;
	//A.teachers = B.teachers;
	
	//B.city = tmp.city;
	//B.day = tmp.day;
	//B.department_id = tmp.department_id;
	//B.department_name = tmp.department_name;
	//B.graduates = tmp.graduates;
	//B.level = tmp.level;
	//B.school_id = tmp.school_id;
	//B.school_name = tmp.school_name;
	//B.students = tmp.students;
	//B.system = tmp.system;
	//B.teachers = tmp.teachers;
	
