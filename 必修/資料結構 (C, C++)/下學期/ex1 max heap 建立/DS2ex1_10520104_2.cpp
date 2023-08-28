//電資二 10520104 莊東翰
#include <iostream>
#include <fstream>   
#include <string.h>
#include <vector>  // vector
#include <stdlib.h>

using namespace std;
static int count = 0; //記錄txt檔內的資料數 

typedef struct 
{ 
	string number;; //學校代碼 
	int students = -1; //學生數 
	int graduates = -1; //畢業人數 
	string school;  //學校名稱 
	string sub; //科系 
	string Day_Division; // 日間∕進修別
	string level; // 等級別
	int serial_number = -1;	
} School;

vector<School> aList;	//紀錄原資料的內容 
vector<School> Search_List;	//搜尋後的結果
vector<School> aHeap;	//堆積後的結果 

class Search{
	public :
		
		void Searching(){
				string school, sub, Day_Division, level;
				cout << "Enter a keyword of 學校名稱: [*]for all"<< endl;   
			    cin  >> school;
			    cout << "Enter a keyword of 科系名稱: [*]for all"<< endl;  
			    cin  >> sub;
			    cout << "Enter a keyword of 日夜別: [*]for all"<< endl;  
				cin  >> Day_Division;
			    cout << "Enter a keyword of 等級別: [*]for all"<< endl;  
			    cin  >> level;
			    if(school!="*"){Search_String(school,0);} //若為"*" 則不用刪除任何資料  
			    if(sub!="*"){Search_String(sub,1);}	//若為"*" 則不用刪除任何資料 
			    if(Day_Division!="*"){Search_String(Day_Division,2);}	//若為"*" 則不用刪除任何資料 
			    if(level!="*"){Search_String(level,3);}	//若為"*" 則不用刪除任何資料 
			    Print(); //印出結果 
		}
			
		void Search_String(string str, int num){	//0為搜尋學校 1為搜尋科系 2為搜尋日夜別 3為搜尋等級別 
			size_t found;	//紀錄string.find出來的結果 
			int times;	//紀錄Search_list資料裡的資料數 
			string current_char;	//紀錄欲搜尋的string
			string current_string;
			
			times = Search_List.size();
			if(num==0){	
				for(int i=0;i<times&&Search_List.size()!=0;i++){
					current_char= Search_List.at(i).school; //複製內容 
					current_string.assign(current_char); 	//將char轉成string
					found = current_string.find(str);	//使用string.find函式 
					if(found==string::npos&&Search_List.size()!=0){  
						Search_List.erase(Search_List.begin()+i);	//若不符合輸入的資料 將此筆資料從清單中刪除 
						i--;	//刪除後要將i-1 
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
		
		void Print(){	//印出Search_List的內容 
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
		
		//char file_Number[50] ; // 輸出入檔名數字 
		//char file_Name_Output[25] ; // 輸出檔名 
		//string file_Name_Output ; // 輸出檔名 
		
		string file_Number_Input; // 輸入檔名 
		string r; // 垃圾 
		string tmp;
		char buf[255];
		bool Have_Input = false; 
		char *temp;
		int pos = 0, pre = 0;
		
		void Load_File(string file_Number_Input){
			School data ;
			fstream file;
			file_Number_Input = "input" + file_Number_Input + ".txt" ; //字串合併 
			file.open(file_Number_Input.c_str(), fstream::in);
			if (file_Number_Input.compare("0")!=0) {
				if (file.is_open()!=0) {		    
					Have_Input = true ;
					file.ignore(255,'\n');       // 檔案一開頭的垃圾                            
	        		file.ignore(255,'\n');                                            
	        		file.ignore(255,'\n');      
	 
					while(file.getline(buf, 255, '\n')){ // 真的開始讀
						tmp.assign(buf); //char to string
						
						pos = tmp.find_first_of('\t', pre);
   
			           	char *temp = new char[tmp.substr(pre, pos - pre).length()+1];	//先將string轉為char* 
			           	strcpy(temp,tmp.substr(pre, pos - pre).c_str()); //複製字串 
			           	data.number = temp;

		                pre = pos+1; 		//往下尋找下一個字元 
		                pos = tmp.find_first_of('\t', pre); 	//遇到\t停止 
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
	           			if(data.graduates!=NULL){ //若畢業人數為\t則捨棄 
	           				if(data.students==NULL){data.students='\0';}
							if(data.school==""){data.school="\0";}
							if(data.sub==""){data.sub="\0";}
							if(data.Day_Division==""){data.Day_Division="\0";}
							if(data.level==""){data.level="\0";}	
							
	           				aList.push_back(data);
							Search_List.push_back(data); //先讓Search_list跟aList內容相同
						   }
			            pos = 0;
			            pre = 0;
			            delete []temp ;
					}
					file.close(); // 檔案內容讀取完後關閉 
				}			
		}
	}
}; 

class Max_Heap{
	public:
		
		int Check(int i){
			
			if(aHeap.size()==1||i==0){} //根結點不用檢查 
			else if(i==1||i==2){
				if(aHeap.size()==2&&aHeap.at(1).graduates>aHeap.at(0).graduates){ //若只有0,1位置有資料 且1大於0 
					Swap(0, 1);
				}	
				else if(aHeap.size()>=3){ //若有3個節點 
						if(aHeap.at(1).graduates>aHeap.at(0).graduates&&aHeap.at(1).graduates>aHeap.at(2).graduates){
							Swap(0, 1);
						}
						else if(aHeap.at(2).graduates>aHeap.at(0).graduates&&aHeap.at(2).graduates>aHeap.at(1).graduates){
							Swap(0, 2);
						}
				}
			}
			else if(i==aHeap.size()-1&&(i-1)%2==0){ //若為最底部的節點 
				if(aHeap.at((i-1)/2).graduates < aHeap.at(aHeap.size()-1).graduates){
					Swap((i-1)/2, i);
				}
			}
			else if(i%2==0){ //若為偶數位置的節點 
				if(aHeap.at((i-2)/2).graduates < aHeap.at(i).graduates||aHeap.at((i-2)/2).graduates < aHeap.at(i-1).graduates){
					if(aHeap.at(i).graduates>=aHeap.at(i-1).graduates){
						Swap((i-2)/2, i);
					}
					else{
						Swap((i-2)/2, i-1);
					}
				}
			}
			else{ 	//若為奇數位置的節點 
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
		
		void Swap(int small, int big){ //將兩資料交換位置 
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
			
			Check(small); //再次檢查交換後的節點 
		}
		
		
		void Add(){ //將資料加入heap中 
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
		
		int find_left_bottom(){ //尋找最左下角節點 
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
			search.Searching();	//慢慢刪除 不符合搜尋內容 的資料 
			heap.Add(); //加入heap 
			heap.Print(); //印出資料 
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
