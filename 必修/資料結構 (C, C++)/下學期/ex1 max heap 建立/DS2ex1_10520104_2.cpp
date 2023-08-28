//�q��G 10520104 ���F��
#include <iostream>
#include <fstream>   
#include <string.h>
#include <vector>  // vector
#include <stdlib.h>

using namespace std;
static int count = 0; //�O��txt�ɤ�����Ƽ� 

typedef struct 
{ 
	string number;; //�ǮեN�X 
	int students = -1; //�ǥͼ� 
	int graduates = -1; //���~�H�� 
	string school;  //�ǮզW�� 
	string sub; //��t 
	string Day_Division; // �鶡�A�i�קO
	string level; // ���ŧO
	int serial_number = -1;	
} School;

vector<School> aList;	//�������ƪ����e 
vector<School> Search_List;	//�j�M�᪺���G
vector<School> aHeap;	//��n�᪺���G 

class Search{
	public :
		
		void Searching(){
				string school, sub, Day_Division, level;
				cout << "Enter a keyword of �ǮզW��: [*]for all"<< endl;   
			    cin  >> school;
			    cout << "Enter a keyword of ��t�W��: [*]for all"<< endl;  
			    cin  >> sub;
			    cout << "Enter a keyword of ��]�O: [*]for all"<< endl;  
				cin  >> Day_Division;
			    cout << "Enter a keyword of ���ŧO: [*]for all"<< endl;  
			    cin  >> level;
			    if(school!="*"){Search_String(school,0);} //�Y��"*" �h���ΧR��������  
			    if(sub!="*"){Search_String(sub,1);}	//�Y��"*" �h���ΧR�������� 
			    if(Day_Division!="*"){Search_String(Day_Division,2);}	//�Y��"*" �h���ΧR�������� 
			    if(level!="*"){Search_String(level,3);}	//�Y��"*" �h���ΧR�������� 
			    Print(); //�L�X���G 
		}
			
		void Search_String(string str, int num){	//0���j�M�Ǯ� 1���j�M��t 2���j�M��]�O 3���j�M���ŧO 
			size_t found;	//����string.find�X�Ӫ����G 
			int times;	//����Search_list��Ƹ̪���Ƽ� 
			string current_char;	//�������j�M��string
			string current_string;
			
			times = Search_List.size();
			if(num==0){	
				for(int i=0;i<times&&Search_List.size()!=0;i++){
					current_char= Search_List.at(i).school; //�ƻs���e 
					current_string.assign(current_char); 	//�Nchar�নstring
					found = current_string.find(str);	//�ϥ�string.find�禡 
					if(found==string::npos&&Search_List.size()!=0){  
						Search_List.erase(Search_List.begin()+i);	//�Y���ŦX��J����� �N������Ʊq�M�椤�R�� 
						i--;	//�R����n�Ni-1 
						times--;
					}
				}
			}
			else if(num==1){
				for(int i=0;i<times&&Search_List.size()!=0;i++){
					current_char = Search_List.at(i).sub;
					current_string.assign(current_char);
					found = current_string.find(str);
					if(found==string::npos&&Search_List.size()!=0){
						Search_List.erase(Search_List.begin()+i);
						i--;
						times--;
					}
				}
			} 
			else if(num==2){
				for(int i=0;i<times&&Search_List.size()!=0;i++){
					current_char = Search_List.at(i).Day_Division;
					current_string.assign(current_char);
					found = current_string.find(str);
					if(found==string::npos&&Search_List.size()!=0){
						Search_List.erase(Search_List.begin()+i);
						i--;
						times--;
					}
				}
			}
			else if(num==3){
				for(int i=0;i<times&&Search_List.size()!=0;i++){
					current_char = Search_List.at(i).level;
					current_string.assign(current_char);
					found = current_string.find(str);
					if(found==string::npos&&Search_List.size()!=0){
						Search_List.erase(Search_List.begin()+i);
						i--;
						times--;
					}
				}
			}
			 
		}
		
		void Print(){	//�L�XSearch_List�����e 
			if(Search_List.size()!=0){
				cout << "There are "<< Search_List.size() <<" matched records, list as below: " << endl;
				for(int i=0;i<Search_List.size();i++){
					Search_List.at(i).serial_number = i+1;
					cout << "[" << i+1 << "] " <<  Search_List.at(i).school << ", " << Search_List.at(i).sub << ", " << Search_List.at(i).Day_Division << ", " << Search_List.at(i).level << ", " << Search_List.at(i).graduates << endl;
				}
			}
			else{
				cout << "There are 0 matched records, list as below: " << endl;
			}
		}

};

class Stuff{
	public :
		
		//char file_Number[50] ; // ��X�J�ɦW�Ʀr 
		//char file_Name_Output[25] ; // ��X�ɦW 
		//string file_Name_Output ; // ��X�ɦW 
		
		string file_Number_Input; // ��J�ɦW 
		string r; // �U�� 
		string tmp;
		char buf[255];
		bool Have_Input = false; 
		char *temp;
		int pos = 0, pre = 0;
		
		void Load_File(string file_Number_Input){
			School data ;
			fstream file;
			file_Number_Input = "input" + file_Number_Input + ".txt" ; //�r��X�� 
			file.open(file_Number_Input.c_str(), fstream::in);
			if (file_Number_Input.compare("0")!=0) {
				if (file.is_open()!=0) {		    
					Have_Input = true ;
					file.ignore(255,'\n');       // �ɮפ@�}�Y���U��                            
	        		file.ignore(255,'\n');                                            
	        		file.ignore(255,'\n');      
	 
					while(file.getline(buf, 255, '\n')){ // �u���}�lŪ
						tmp.assign(buf); //char to string
						
						pos = tmp.find_first_of('\t', pre);
   
			           	char *temp = new char[tmp.substr(pre, pos - pre).length()+1];	//���Nstring�ରchar* 
			           	strcpy(temp,tmp.substr(pre, pos - pre).c_str()); //�ƻs�r�� 
			           	data.number = temp;

		                pre = pos+1; 		//���U�M��U�@�Ӧr�� 
		                pos = tmp.find_first_of('\t', pre); 	//�J��\t���� 
		     			data.school  = tmp.substr(pre, pos - pre);               
						
						pre = pos+1;
			            pos = tmp.find_first_of('\t', pre); 
			            pre = pos+1;
			            pos = tmp.find_first_of('\t', pre); 
			            data.sub = tmp.substr(pre, pos - pre);
			            
		                pre = pos+1;
		                pos = tmp.find_first_of('\t', pre); 
		                data.Day_Division = tmp.substr(pre, pos - pre);
									                
						pre = pos+1;
			            pos = tmp.find_first_of('\t', pre);
			            data.level = tmp.substr(pre, pos -pre);
			            
			           	pre = pos+1;
			            pos = tmp.find_first_of('\t', pre); 
			            temp = new char[tmp.substr(pre, pos - pre).length()+1];
			           	strcpy(temp,tmp.substr(pre, pos - pre).c_str());
			           	data.students = atoi(temp);
			           	
			           	pre = pos+1;
			            pos = tmp.find_first_of('\t', pre);
						pre = pos+1;
		                pos = tmp.find_first_of('\t', pre); 
		                temp = new char[tmp.substr(pre, pos - pre).length()+1];
		            	strcpy(temp,tmp.substr(pre, pos - pre).c_str());			            	
						data.graduates = atoi(temp); 		//char* to int 
						
						while(pos != string::npos){
								pre = pos+1;
			                	pos = tmp.find_first_of('\t', pre);
							}    
	           			if(data.graduates!=NULL){ //�Y���~�H�Ƭ�\t�h�˱� 
	           				if(data.students==NULL){data.students='\0';}
							if(data.school==""){data.school="\0";}
							if(data.sub==""){data.sub="\0";}
							if(data.Day_Division==""){data.Day_Division="\0";}
							if(data.level==""){data.level="\0";}	
							
	           				aList.push_back(data);
							Search_List.push_back(data); //����Search_list��aList���e�ۦP
						   }
			            pos = 0;
			            pre = 0;
			            delete []temp ;
					}
					file.close(); // �ɮפ��eŪ���������� 
				}			
		}
	}
}; 

class Max_Heap{
	public:
		
		int Check(int i){
			
			if(aHeap.size()==1||i==0){} //�ڵ��I�����ˬd 
			else if(i==1||i==2){
				if(aHeap.size()==2&&aHeap.at(1).graduates>aHeap.at(0).graduates){ //�Y�u��0,1��m����� �B1�j��0 
					Swap(0, 1);
				}	
				else if(aHeap.size()>=3){ //�Y��3�Ӹ`�I 
						if(aHeap.at(1).graduates>aHeap.at(0).graduates&&aHeap.at(1).graduates>aHeap.at(2).graduates){
							Swap(0, 1);
						}
						else if(aHeap.at(2).graduates>aHeap.at(0).graduates&&aHeap.at(2).graduates>aHeap.at(1).graduates){
							Swap(0, 2);
						}
				}
			}
			else if(i==aHeap.size()-1&&(i-1)%2==0){ //�Y���̩������`�I 
				if(aHeap.at((i-1)/2).graduates < aHeap.at(aHeap.size()-1).graduates){
					Swap((i-1)/2, i);
				}
			}
			else if(i%2==0){ //�Y�����Ʀ�m���`�I 
				if(aHeap.at((i-2)/2).graduates < aHeap.at(i).graduates||aHeap.at((i-2)/2).graduates < aHeap.at(i-1).graduates){
					if(aHeap.at(i).graduates>=aHeap.at(i-1).graduates){
						Swap((i-2)/2, i);
					}
					else{
						Swap((i-2)/2, i-1);
					}
				}
			}
			else{ 	//�Y���_�Ʀ�m���`�I 
				if(aHeap.at((i-1)/2).graduates < aHeap.at(i).graduates||aHeap.at((i-1)/2).graduates < aHeap.at(i+1).graduates){
					if(aHeap.at(i).graduates>=aHeap.at(i-1).graduates){
						Swap((i-1)/2, i);
					}
					else{
						Swap((i-1)/2, i-1);
					}
				}
			}
			
		}
		
		void Swap(int small, int big){ //�N���ƥ洫��m 
			School tmp;
			
			tmp.students = aHeap.at(small).students;
			tmp.number = aHeap.at(small).number;
			tmp.graduates = aHeap.at(small).graduates;
			tmp.school = aHeap.at(small).school;
			tmp.sub =  aHeap.at(small).sub;
			tmp.Day_Division = aHeap.at(small).Day_Division;
			tmp.level =  aHeap.at(small).level;
			tmp.serial_number = aHeap.at(small).serial_number;
			
			aHeap.at(small).students = aHeap.at(big).students;
			aHeap.at(small).number = aHeap.at(big).number;
			aHeap.at(small).graduates = aHeap.at(big).graduates;
			aHeap.at(small).school = aHeap.at(big).school;
			aHeap.at(small).sub = aHeap.at(big).sub;
			aHeap.at(small).Day_Division = aHeap.at(big).Day_Division;
			aHeap.at(small).level = aHeap.at(big).level;
			aHeap.at(small).serial_number = aHeap.at(big).serial_number;
			
			aHeap.at(big).students = tmp.students;
			aHeap.at(big).number = tmp.number;
			aHeap.at(big).graduates = tmp.graduates;
			aHeap.at(big).school = tmp.school;
			aHeap.at(big).sub =  aHeap.at(small).sub;
			aHeap.at(big).Day_Division = tmp.Day_Division;
			aHeap.at(big).level = tmp.level;
			aHeap.at(big).serial_number = tmp.serial_number;
			
			Check(small); //�A���ˬd�洫�᪺�`�I 
		}
		
		
		void Add(){ //�N��ƥ[�Jheap�� 
			School sd ;
			for(int i=0;i<Search_List.size();i++){
				sd.students = Search_List.at(i).students;
				sd.school = Search_List.at(i).school;
				sd.sub = Search_List.at(i).sub;
				sd.Day_Division = Search_List.at(i).Day_Division;
				sd.level = Search_List.at(i).level;	
				sd.number = Search_List.at(i).number;
				sd.graduates = Search_List.at(i).graduates;
				sd.serial_number = Search_List.at(i).serial_number;
				
				aHeap.push_back(sd);
				Check(i);
			}
		}
		
		int find_left_bottom(){ //�M��̥��U���`�I 
			int i=1,j=0;
			while(i<=aHeap.size()){
				j=i;
				i=i*2+1;
			}
			return j;
		}
		
		void Print(){
			int left_bottom,right_bottom;
			left_bottom = find_left_bottom();
			cout << endl;
			cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
			cout << "@@@ Mission Two: Build a Max Heap from the Selected Data @@@" << endl;
			cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
			cout << endl;
			cout << "<max heap>" << endl;
			cout << "root:[" << aHeap.at(0).serial_number << "] " << aHeap.at(0).graduates << endl;
			cout << "bottom:[" << aHeap.at(aHeap.size()-1).serial_number << "] " << aHeap.at(aHeap.size()-1).graduates << endl;
			cout << "leftmost bottom:[" << aHeap.at(left_bottom).serial_number << "] " << aHeap.at(left_bottom).graduates << endl;
		} 
};
	
int main()
{
    string projnum;
    string file_number;
    Stuff stuff;
    Search search;
    Max_Heap heap;
    
    cout << "Input the file number: 101, 102, ... [0]Quit" << endl;  
	cin  >> file_number;
	cout << endl;
	if(file_number!="0"){  
		stuff.Load_File(file_number);
		if(stuff.Have_Input==true){
			cout << "************************************************************" << endl;
			cout << "*** Mission One: Select Matched Recoeds From a Text File ***" << endl;
			cout << "************************************************************" << endl;
			cout << endl;
			search.Searching();	//�C�C�R�� ���ŦX�j�M���e ����� 
			heap.Add(); //�[�Jheap 
			heap.Print(); //�L�X��� 
			cout << endl;
			while(1){
				cout << "[0]Quit or [Any other]continue?" << endl; 
			    cin >> projnum;
			    if(projnum=="0"){aList.clear(); Search_List.clear(); aHeap.clear(); break;} //reset
			    else{aHeap.clear(); search.Searching(); heap.Add(); heap.Print();}
			}
			exit(0);
		}
		else{
			cout << "input" << file_number <<".txt does not exist!" <<endl; 
		}

	}
	else{exit(0);}
    
    return 0;
}
