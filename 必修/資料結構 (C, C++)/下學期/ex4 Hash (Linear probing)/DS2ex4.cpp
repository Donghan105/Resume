// DS2 ex4 hash
// 10311245 吳承曄 / 10520104 莊東翰
// 2018 / 05 / 11

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <math.h>

using namespace std;

string ofileName;

typedef struct sT{
	char sid[10];
	char sname[10];
	unsigned char score1;
	unsigned char score2;
	unsigned char score3;
	unsigned char score4;
	unsigned char score5;
	unsigned char score6;
	float average;
} studentType;

//**Grobal Value
vector<studentType> sSet;	//文件內容 
vector<studentType> HashTable;

//**Fuction list
int ReadFile(); 	// 讀檔
void ShowData();  // 顯示資料 
int HashSize();
void Finding_Prime_Num(int, bool*);
int readBin();
void HashTable_Print();
void Hash_Table_Resize(int);
void Hash_Table();	
void collision(int,int);

int main(int argc, char** argv) {
	
	int read = ReadFile();
	
	string cmd;
	
	while(read){
	  
	  	ShowData();
	  	readBin();
	  	int i = HashSize();
		Hash_Table_Resize(i);		
		Hash_Table();
		HashTable_Print();
		cout << endl << "[0]Quit or [AnyOther]Continue ?" << endl;
		cin >> cmd;
		sSet.clear();
		if(!cmd.compare("0")) return 0;
		else read = ReadFile();
	  
  }
	
	return 0;
}

int ReadFile(){
	
	string fileName;
	cout << endl << "Input the file name: 401, 402 ... [0]Quit" << endl;
	cin >> fileName;
	
	if(!fileName.compare("0")) return 0;	//quit
	
	ofileName = "input" + fileName + ".bin";
	fileName = "input" + fileName + ".txt";
	fstream fin(fileName.c_str());
	
	if(!fin){
		cout << "### " << fileName << " does not exist! ###" << endl;
		return ReadFile();	//再讀檔一次 
	}
	else{
	  
	  ofstream fout(ofileName.c_str(), ios::binary); // output file
	  
		studentType oneS;
	
		string line;
		string oneStr;
		stringstream iss;
		int check;
		
		while( getline(fin, line, '\n') ){	//一次擷取一行 
			check = 0;
			iss << line;
			while( getline(iss, oneStr, '\t') ){	//再擷取出的字串分出 token 
				
				switch(check){
					case 0: strcpy(oneS.sid, oneStr.c_str());
							break;
					case 1: strcpy(oneS.sname, oneStr.c_str());
							break;
					case 2: oneS.score1 = (unsigned char) atoi(oneStr.c_str());
							break;
					case 3: oneS.score2 = (unsigned char) atoi(oneStr.c_str());
							break;
					case 4: oneS.score3 = (unsigned char) atoi(oneStr.c_str());
							break;
					case 5: oneS.score4 = (unsigned char) atoi(oneStr.c_str());
							break;
					case 6: oneS.score5 = (unsigned char) atoi(oneStr.c_str());
							break;
					case 7: oneS.score6 = (unsigned char) atoi(oneStr.c_str());
							break;
					case 8: oneS.average = atof(oneStr.c_str());
							break;
					default: break;
				}
				check++;

				if(check == 9) {
				  sSet.push_back(oneS);
				  fout.write( (char*)&oneS, sizeof(studentType) );  // write file
        }
				  
			}
			
			iss.clear();	// *stringstream需清空才有辦法get下一行 
		}

		fin.close();
		fout.close();

		return 1;
	}
	
} // ReadFile()

void ShowData(){

	cout << endl << "~~ A binary file has been successfully created! ~~" << endl;
	for(int i = 0; i < sSet.size(); i++){
		cout << "[" << i+1 << "]" << "\t" << sSet[i].sid << ", " << sSet[i].sname << "\t" << (int)sSet[i].score1 << "\t" << (int)sSet[i].score2 << "\t" 
         << (int)sSet[i].score3 << "\t" << (int)sSet[i].score4 << "\t" << (int)sSet[i].score5 << "\t" << (int)sSet[i].score6 << "\t" << sSet[i].average << "\t"<<  endl;
	}
	cout << endl;
	
} // ShowData()

int HashSize(){
	float i = sSet.size();
	int j, num;
	bool result = false;
	
	i = i*1.2;
	j = i;
	
	for(num=j;num>0;num--){
		Finding_Prime_Num(num,&result);
		if(result==true){break;}
	}
	return num;
	
}

void Finding_Prime_Num(int num, bool *result){
	int factor = 0;
	for(int i=1;i<=num;i++){
		if(num%i==0){factor++;}
	}
	if(factor==2){*result = true;}	
}

void Hash_Table_Resize(int i){
	sSet.clear();
	sSet.resize(i);	
}

void Hash_Table(){
	string str;
	studentType oneS;
	int sum=1;
	int num, quotient, remainder;
	for(int j=0;j<HashTable.size();j++){
		sum = 1; num = 0;
		remainder = 0; quotient = 0;
		str.assign(HashTable[j].sid);
		num = atoi(HashTable[j].sid);
		for(int i = pow(10,str.size()-1);i>0;i=i/10){
			quotient = num/i;
			if(quotient!=0){
				sum = sum * quotient;
				num = num - i*quotient;		
			}
				cout << sum << endl;
				cout << num << endl;
		}
		remainder = sum%sSet.size();
	
		if(sSet[remainder].average==0){
		strcpy(sSet[remainder].sid,HashTable[j].sid);
		strcpy(sSet[remainder].sname,HashTable[j].sname);
		sSet[remainder].score1 = HashTable[j].score1;
		sSet[remainder].score2 = HashTable[j].score2;
		sSet[remainder].score3 = HashTable[j].score3;
		sSet[remainder].score4 = HashTable[j].score4;
		sSet[remainder].score5 = HashTable[j].score5;
		sSet[remainder].score6 = HashTable[j].score6;
		sSet[remainder].average = HashTable[j].average;
		}
		else{
			collision(remainder,j);
		}
	}
}

void collision(int key,int j){
	if(sSet[key].average!=0){
		collision(key+1,j);
	}
	else{
		strcpy(sSet[key].sid,HashTable[j].sid);
		strcpy(sSet[key].sname,HashTable[j].sname);
		sSet[key].score1 = HashTable[j].score1;
		sSet[key].score2 = HashTable[j].score2;
		sSet[key].score3 = HashTable[j].score3;
		sSet[key].score4 = HashTable[j].score4;
		sSet[key].score5 = HashTable[j].score5;
		sSet[key].score6 = HashTable[j].score6;
		sSet[key].average = HashTable[j].average;	
	}
}

int readBin(){
	
	if(!ofileName.compare("0")) return 0;	//quit
	
	fstream fin(ofileName.c_str(), ios::in | ios::binary);
	//fin.unsetf(std::ios::skipws);
	
	if(!fin){
		cout << "### " << ofileName << " does not exist! ###" << endl; 
	}
	else{
	  
		studentType oneS;
	
		string line;
		string oneStr;
		stringstream iss;
		int check;
		
		while(!fin.eof()){	//一次擷取一行 
			fin.read(reinterpret_cast<char*>(&oneS), sizeof(studentType));
			if(!fin.eof()){	
				HashTable.push_back(oneS);			
			}
		}
			

		fin.close();
		return 1;
	}
}

void HashTable_Print(){
	for(int i = 0; i < sSet.size(); i++){
		cout << "[" << i+1 << "]" << "\t" << sSet[i].sid << ", " << sSet[i].sname << "\t" << (int)sSet[i].score1 << "\t" << (int)sSet[i].score2 << "\t" 
         << (int)sSet[i].score3 << "\t" << (int)sSet[i].score4 << "\t" << (int)sSet[i].score5 << "\t" << (int)sSet[i].score6 << "\t" << sSet[i].average << "\t"<<  endl;
	}
	cout << endl;
}
