//10724370 馮浩陽, 10520104莊東翰
#include <iostream>
#include <stdio.h>
#include <string.h>	//strcpy, assign, substr, erase, find, erase, strstr
#include <vector> 	//動態陣列 
#include <stdlib.h> // atoi
#include <sstream>
using namespace std;

struct node{
	string data;
	node *next;
};//任務二 印結果用linked-list 

//mission 1
int M1(void);
double exp(void);
double term(void);
double factor(void);
char getPrintableChar(void);
void match(char expectedToken);
void err1(void);
void err2(void);
void err3(void);
void err4(void);

//mission 2
void M2(string);	//mission 2
string Merge_num(vector<string>&);	//切數字token
bool Exist_or_not(string ch, vector<string>&);	//搜尋算式中是否有某符號 
string CAL(vector<string>&);	//最內層括號算式 中序轉後序 

//linked-list
void print_LL(node*);
void del_LL(node*);

int A=0;
char token;
string STR = "";

void print_LL(node* cur){	//print linked-list
	while(cur->next!=NULL){
		cout << cur->data << ", ";
		cur = cur->next;
	}
	cout << cur->data << ", " << endl;
}

void del_LL(node* cur){	//del linked-list
	node* pos;
	while(cur->next!=NULL){
		//cout << "data: " << cur->data << endl;
		pos = cur->next;
		delete cur;
		cur = pos;
	}
	//cout << "data: " << cur->data << endl;
	delete cur;
}

int M1(void)
{int i=0,a=0,b=0;
	double result;
	bool res = true;
	
	cout <<"請輸入你的算式: ";

	while(1)
	{
		token = getPrintableChar();
		
		
		if (token == 'q')
			break;

		result = exp();
		if (token == '\n'){
			printf("Result is: %g\n", result);
			printf("It is a  legitimate infix expression\n");
			break;
		}
		else if(token=='+'||token=='-'||token== '/'||token=='*')
        {
            err3();
            res = false;
            break;
        }
		else if(token==')'||token=='('){
			err2();
			res = false;
			break;
		}
		else if(token=='^')
        {
            err1();
            res = false;
            break;
        }
        else if(token=='9'||token=='0'||token=='1'||token=='2'||token=='3'||token=='4'||token=='5'||token=='6'||token=='7'||token=='8')
        {
            err4();
            res = false;
            break;
        }

	}

	return res;
}

double exp(void)
{
	double temp = term();
	//cout << "temp1: " << token << endl;
	while (token == '+' || token == '-')
		switch(token)
		{
			case '+': match('+');
					  temp += term();
					  //cout << "temp2: " << token << endl;
					  break;
			case '-': match('-');
					  temp -= term();
					  //cout << "temp3: " << token << endl;
					  break;
		}
	return temp;
}

double term(void)
{
	double temp = factor();
	////cout << "temp4: " << token << endl;
	STR = STR + token;
	while (token == '*' || token == '/')
		switch(token)
		{
			case '*': match('*');
					  temp *= factor();
					  ////cout << "temp5: " << token << endl;
					  STR = STR + token;
					  break;
			case '/': match('/');
					  temp /= factor();
					  ////cout << "temp6: " << token << endl;
					  STR = STR + token;
					  break;

		}
	return temp;
}

double factor(void)
{
	double temp;
	if (token == '(')
	{
		////cout << "temp6: " << token << endl;
		STR = STR + token;
		match('(');
		temp = exp();
		match(')');
	}
	else if (isdigit(token))
	{
		ungetc(token, stdin);
		scanf("%lf", &temp);
		////cout << "temp68080: " << temp << endl;
		stringstream ss;
		ss << temp;
		STR = STR + ss.str();
		token = getPrintableChar();
	} 
	else{
		err3();
	}
		
		
	return temp;
}

/*void error(void)
{
	fprintf(stderr, "Error!\n");
	exit(EXIT_FAILURE);
}*/

void err1(void)
{
	fprintf(stderr, "^is not a legitimate character\n");
	//exit(EXIT_FAILURE);
}

void err2(void)
{
printf("there is one extra open parenthesis\n");
	//exit(EXIT_FAILURE);

}

void err3(void)
{printf("there is one extra operator\n");
	//exit(EXIT_FAILURE);
}

void err4(void)
{printf("there is one extra operand\n");
	//exit(EXIT_FAILURE);
}
	
void match(char expectedToken)
{
	if (expectedToken == token)
		token = getPrintableChar();
	else
		err2();
}

char getPrintableChar(void)
{
	char temp;
	do{
		temp = getchar();
	}
	while (isblank(temp));
	
	return temp;
}




string CAL(vector<string>& temp){	//任務二 中序轉後序(每個括號內的後序轉換)
	string str;
	while(Exist_or_not("*", temp)==true || Exist_or_not("/", temp)==true){
		for(int i=0; i<temp.size(); i++){
			//cout << "temp[i]: " << temp[i] << endl;
			if(temp[i]=="*"){
				str = temp[i-1] + " " + temp[i+1] + " " + "@";	//@代替* 
				temp.erase(temp.begin()+i-1);
				temp.erase(temp.begin()+i-1);
				temp.erase(temp.begin()+i-1);
				temp.insert(temp.begin()+i-1, str);
				i = temp.size(); //break
			}
			else if(temp[i]=="/"){
				str = temp[i-1] + " " + temp[i+1] + " " + "#";	//#代替/
				temp.erase(temp.begin()+i-1);
				temp.erase(temp.begin()+i-1);
				temp.erase(temp.begin()+i-1);
				temp.insert(temp.begin()+i-1, str);
				i = temp.size(); //break
			}
		}
	}
	
	while(Exist_or_not("+", temp)==true || Exist_or_not("-", temp)==true){
		for(int i=0; i<temp.size(); i++){
			//cout << "temp[i]: " << temp[i] << endl;
			if(temp[i]=="+"){
				str = temp[i-1] + " " + temp[i+1] + " " + "$";	//$代替+ 
				temp.erase(temp.begin()+i-1);
				temp.erase(temp.begin()+i-1);
				temp.erase(temp.begin()+i-1);
				temp.insert(temp.begin()+i-1, str);
				i = temp.size(); //break
			}
			else if(temp[i]=="-"){
				str = temp[i-1] + " " + temp[i+1] + " " + "%";	//%代替-
				temp.erase(temp.begin()+i-1);
				temp.erase(temp.begin()+i-1);
				temp.erase(temp.begin()+i-1);
				temp.insert(temp.begin()+i-1, str);
				i = temp.size(); //break
			}
		}
	}
	
	return temp[0];	//最後整個陣列只剩一個元素0 
}

bool Exist_or_not(string ch, vector<string>& temp){
	bool result = false;
	for(int i=0;i<temp.size(); i++){
		if(ch == temp[i]){
			result = true;
			break;
		}
	}
	return result;	
}

void M2(string your_input){	//任務二 
	vector<string> Formula;
	vector<string> temp;
	string num, tmp, formula;
	bool strange = false;
	
	for(int i=0;i<your_input.size();i++){
		tmp = your_input[i];
		if(tmp=="(" || tmp==")" || tmp=="+" || tmp=="-" || tmp=="*" || tmp=="/" || tmp==" "){
			if(temp.size()!=0){		//若有需要 切token(暫存區大小 != 0) 
				num = Merge_num(temp);	//切數字token
				Formula.push_back(num);
				temp.clear();
			}
			
			
			if(tmp=="(" || tmp==")"){
				Formula.push_back(tmp);
			}
			else if(tmp=="+" || tmp=="-"){
				Formula.push_back(tmp);
			}
			else if(tmp=="*" || tmp=="/"){
				Formula.push_back(tmp);
			}
		}
		else{
			int asc = (int)your_input[i];
			if(asc >= 48 && asc <= 57){	//為0-9數字 
				temp.push_back(tmp);
			}
			else{
				cout << "有奇怪符號" << endl;
				strange = true;
				Formula.clear();
				temp.clear();
				break; 
			}
		}
		
		if(i==your_input.size()-1 && temp.size()!=0){
			num = Merge_num(temp);	//切數字token
			Formula.push_back(num);
			temp.clear();
		}
	}
	
	if(strange==false){
		//得到最大括號算式(括號法)  
		while(Exist_or_not("(", Formula)==true || Exist_or_not(")", Formula)==true){
			int last_R = -1, last_L = -1;
			vector<string> cal;
			
			for(int i=Formula.size()-1; i>=0; i--){
				if("(" == Formula[i]){last_L = i;}
				else if(")" == Formula[i]){last_R = i;}
				
				if(last_R!=-1 && last_L!=-1){
					for(int j = last_L+1; j<last_R; j++){
						cal.push_back(Formula[j]);
					}
					formula = CAL(cal);
					//cout << "formula: " << formula << endl;
					Formula.insert(Formula.begin()+last_R+1, formula);
					for(int j = last_L; j<=last_R; j++){
						//cout << "delete: " << Formula[last_L] << endl;
						Formula.erase(Formula.begin()+last_L);
					}
					
					for(int j = 0; j<Formula.size(); j++){
						//cout << "nowwww: " << Formula[j] << endl;
					}
				
					i = 0;}	//break
			}
		}
		
		node *start, *end, *cur, *pre;	//LINKED-LIST
		start = new node;
		end = new node;
	
		formula = CAL(Formula);	//處理每個括號內的算式
		 
		for(int i=0; i<formula.size();i++){
			char ch = formula[i];
			if(ch=='['){
				formula[i]='('; 
			}
			else if(ch==']'){
				formula[i]=')';
			}
			else if(ch=='@'){
				formula[i]='*';
			}
			else if(ch=='#'){
				formula[i]='/';
			}
			else if(ch=='$'){
				formula[i]='+';
			}
			else if(ch=='%'){
				formula[i]='-';
			}
		}
		//cout << formula << endl;	//string
		
		
		int found;
		vector<string> equation;
		found = formula.find(" ");
		while(found!=string::npos){	//string內切token 
			string tmp;
			tmp = formula.substr(0, found);
			//cout << "tmp: " << tmp << endl;
			equation.push_back(tmp);
			formula.erase(0, found+1);
			found = formula.find(" ");	
		}
		if(formula.size()>0){
			equation.push_back(formula);
		}
		
		for(int i=0; i<equation.size();i++) {	//放入linked-list 
			if(i==0){
				start->data = equation[i];
				start->next = NULL;
				pre = start;
				//cout << "123: " << equation[i] << endl;
			}
			else{
				cur = new node;
				cur->data = equation[i];
				cur->next = NULL;
				pre->next = cur;
				pre = cur;
				//cout << "456: " << equation[i] << endl;
			}
		}
		print_LL(start);	//印出linked-list 
		del_LL(start);	//刪除linked-list 
	}


}

string Merge_num(vector<string>& temp){	//切數字token 
	string str;
	for(int i=0;i<temp.size();i++){
		str = str + temp[i];
	}
	
	return str;
}


int main(int argc, char** argv){
	string your_input;
	int cmd, contin;
	while(1){
		STR = "";
		
		fflush(stdin);
		cout << "Input your command(0:quit, 1:任務一, 2:任務二):";
		cin >> cmd;
		if(cmd==0){
			break;
		}
		else if(cmd==1){
			fflush(stdin);
			M1();
		}
		else if(cmd==2){
			fflush(stdin);
			if(M1()==true){	//先走m1 沒問題進m2 
				//getline(cin, your_input);	//能讀取空白 
				STR.erase(STR.begin()+STR.size()-1);
				////cout << "原算式: " << STR << endl; 
				M2(STR);
			}
			else{
				cout << "中序式有誤 不做後序轉換" << endl; 
			}
		}
		else{
			cout << "No such command." << endl;
		}
		
		fflush(stdin);
		cout << "continue? (0:No, else:Yes):";
		cin >> contin;
		cout << endl;
		if(contin==0){
			break;
		}
	}
	
	return 0;
}
