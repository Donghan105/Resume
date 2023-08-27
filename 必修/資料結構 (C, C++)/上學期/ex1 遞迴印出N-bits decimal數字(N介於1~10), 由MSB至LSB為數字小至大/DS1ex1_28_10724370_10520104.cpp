//10724370 ���E��, 10520104���F�� 
#include <iostream>
#include <stdio.h>
#include <string.h> //string���O, strcpy, assign, substr, erase, find, erase, strstr
#include <vector>  //�ʺA�}�C
#include <time.h>	//CLOCKS_PER_SEC
#include <math.h>	//sqrt
#include <stdlib.h> // atoi
#include <sstream>
#define MAX_SIZE 92
using namespace std;	//�ٲ�std::

void Recursion(int, string);

vector<string> result;

void Recursion(int N, string num){
	string last_char;
	last_char = num[num.size()-1];
	
	if(num.size()==N){
		result.push_back(num);	//push result into the vector
	}
	else if(strcmp(last_char.c_str(), "9")==0){ //last_char == "9"
		num = "@failed";
	}
	else if(strcmp(last_char.c_str(), "8")==0){ //last_char == "8"
		Recursion(N, num + "9");
	}
	else if(strcmp(last_char.c_str(), "7")==0){ //last_char == "7"
		Recursion(N, num + "8");
		Recursion(N, num + "9");
	}
	else if(strcmp(last_char.c_str(), "6")==0){ //last_char == "6"
		Recursion(N, num + "7");
		Recursion(N, num + "8");
		Recursion(N, num + "9");
	}
	else if(strcmp(last_char.c_str(), "5")==0){ //last_char == "5"
		Recursion(N, num + "6");
		Recursion(N, num + "7");
		Recursion(N, num + "8");
		Recursion(N, num + "9");
	}
	else if(strcmp(last_char.c_str(), "4")==0){ //last_char == "4"
		Recursion(N, num + "5");
		Recursion(N, num + "6");
		Recursion(N, num + "7");
		Recursion(N, num + "8");
		Recursion(N, num + "9");
	}
	else if(strcmp(last_char.c_str(), "3")==0){ //last_char == "3"
		Recursion(N, num + "4");
		Recursion(N, num + "5");
		Recursion(N, num + "6");
		Recursion(N, num + "7");
		Recursion(N, num + "8");
		Recursion(N, num + "9");
	}
	else if(strcmp(last_char.c_str(), "2")==0){ //last_char == "2"
		Recursion(N, num + "3");
		Recursion(N, num + "4");
		Recursion(N, num + "5");
		Recursion(N, num + "6");
		Recursion(N, num + "7");
		Recursion(N, num + "8");
		Recursion(N, num + "9");
	}
	else if(strcmp(last_char.c_str(), "1")==0){ //last_char == "1"
		Recursion(N, num + "2");
		Recursion(N, num + "3");
		Recursion(N, num + "4");
		Recursion(N, num + "5");
		Recursion(N, num + "6");
		Recursion(N, num + "7");
		Recursion(N, num + "8");
		Recursion(N, num + "9");
	}
	else if(strcmp(last_char.c_str(), "0")==0){ //last_char == "0"
		Recursion(N, num + "1");
		Recursion(N, num + "2");
		Recursion(N, num + "3");
		Recursion(N, num + "4");
		Recursion(N, num + "5");
		Recursion(N, num + "6");
		Recursion(N, num + "7");
		Recursion(N, num + "8");
		Recursion(N, num + "9");
	}
	else{
		num = "error";
	}
	
}


int main(int argc, char** argv)
{
	int N, contin;
	float START,END;
	while(1){
		cout << "�п�J�A��N��: "; 
		cin >> N;
		START = clock();
		Recursion(N, "0");
		Recursion(N, "1");
		Recursion(N, "2");
		Recursion(N, "3");
		Recursion(N, "4");
		Recursion(N, "5");
		Recursion(N, "6");
		Recursion(N, "7");
		Recursion(N, "8");
		Recursion(N, "9");
		END = clock();
		for(int i=0;i<result.size();i++){
			cout << "[" << i+1 << "]\t" << result[i] << endl;
		}
		cout << "�Ӯ�: " << (END - START) / CLOCKS_PER_SEC << " seconds" << endl;
		result.clear();
		
		cout << "�O�_�~�����?(0:�_, 1:�O): "; 
		cin >> contin;
		if(contin==0){break;}
		else{cout << endl;}
	}
	return 0;
}

