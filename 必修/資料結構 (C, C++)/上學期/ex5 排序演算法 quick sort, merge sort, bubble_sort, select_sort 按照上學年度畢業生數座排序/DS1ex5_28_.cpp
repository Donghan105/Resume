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
#include <sstream>
using namespace std;

//*****************************任務一***********************************
struct Custom{		//結構 用來自定義型別  (#include <string.h>)
	string Number;//學校程式碼
	string name;//學校名稱
	int code;//學校名稱
	string subject;//科系名稱
	string day;//D
	string day1;//日
	string level;//B
	string c;//學士
	int number;//學生數  
	int number1;//教師數  
	int number2;//上學年度畢業生數
	int number3;
	string city;//縣市名稱
	int a;//1
	string ch;//體系別
	float score;	//string type
};
//******************************end*************************************

//*****************************任務二***********************************
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

struct LT_GTtype{	//less than, Greater than
	vector<CustomType> tmp;
	int pivot;	//分隔位置 
};

class CLASS_Quick{	//quick sort
	private:
		vector<CustomType> temp;	
		LT_GTtype Less_or_Greater(vector<CustomType>& tmp, int start, int end){//找出小於pivot的元素 放左側
			int cur = start;
			for(int i=start+1;i<=end;i++){
				if(tmp[i].graduates < tmp[cur].graduates){
					CustomType swap;
					swap = tmp[i];
					tmp.erase(tmp.begin()+i);
					tmp.insert(tmp.begin()+cur, swap);
					cur++;
				}
			}
			
			LT_GTtype TT;
			TT.tmp = tmp;
			TT.pivot = cur;
			return TT;
		}
		
		vector<CustomType>& quick_sort(vector<CustomType>& tmp, int start, int end){
			int pivot;
			LT_GTtype TT;
			TT = Less_or_Greater(tmp, start, end);//less than, Greater than
			tmp = TT.tmp;
			pivot =TT.pivot;
			if( end-(pivot+1) >= 1 ){	//右側還能做
				quick_sort(tmp, pivot+1, end);
			}
			if( pivot-1-start >= 1 ){	//左側還能做
				quick_sort(tmp, start, pivot-1);
			}
			
			temp = tmp;
		}
		
	public:
		void Print(vector<CustomType>&List){
			for(int i=0;i<List.size();i++){
				cout << "學校代碼: " << List[i].school_id << ", 學校名稱: " << List[i].school_name << ", 科系代碼: " << List[i].department_id << ", 科系名稱: "  << List[i].department_name << endl;
				cout << "日間∕進修別: "  << List[i].day << ", 等級別: " << List[i].level << ", 學生數: " << List[i].students << ", 教師數: " << List[i].teachers << ", 上學年度畢業生數: " << List[i].graduates << ", 縣市名稱: " << List[i].city << ", 體系別: " << List[i].system << endl;
			}
		}
		
		void run(vector<CustomType>& tmp){
			quick_sort(tmp, 0, tmp.size()-1);
			std::reverse(temp.begin(), temp.end());	//reverse vector
			tmp = temp;
			
		}

};

class CLASS_Merge{	// merge sort
	private:
		vector<CustomType>teemp;
		
		vector<CustomType>& merge(vector<CustomType>& tmp, vector<CustomType>& tmpL, vector<CustomType>& tmpR, unsigned int left, unsigned int pivot, unsigned int right){	
			for(unsigned int j = left;j<pivot;j++){	//copy left
				tmpL.push_back(tmp[j]);
			}
			
			for(unsigned int j = pivot;j<=right;j++){//copy right
				tmpR.push_back(tmp[j]);
			}
			
			if(tmpL.size()==2){
				if(tmpL[0].graduates < tmpL[1].graduates){
					CustomType swap;
					swap = tmpL[0];
					tmpL[0] = tmpL[1];
					tmpL[1] = swap;
				}
			}
			
			if(tmpR.size()==2){
				if(tmpR[0].graduates < tmpR[1].graduates){
					CustomType swap;
					swap = tmpR[0];
					tmpR[0] = tmpR[1];
					tmpR[1] = swap;
				}
			}
			
			
			for(unsigned int i = 0;i<tmpR.size();i++){	//merging
				for(unsigned int j = 0;j<tmpL.size();j++){
					if(tmpR[i].graduates > tmpL[j].graduates){
						tmpL.insert(tmpL.begin()+j, tmpR[i]);
						j = tmpL.size();
					}	
					else if(j == tmpL.size()-1){
						tmpL.push_back(tmpR[i]);
						j = tmpL.size();
					}
				}
			}
			for(int i=0;i<tmpL.size(); i++){	//覆蓋原本陣列
				tmp[i+left] = tmpL[i];
			}


			return tmp;
		}
		
		merge_sort(vector<CustomType>& tmp, unsigned int left, unsigned int pivot, unsigned int right){
			vector<CustomType> tmpL, tmpR;
			if(pivot - left >2){//左邊 	//01 234 				012 3456    0123 0123 45 67 
				merge_sort(tmp, left, (left + pivot)/2, pivot-1);
			}
			if(right - pivot >1){//右邊  
				merge_sort(tmp, pivot, (pivot + right + 1)/2, right);
			}
			
			teemp = merge(tmp, tmpL, tmpR, left, pivot, right);
		}
	
	public:
		void Print(vector<CustomType>&List){
			for(int i=0;i<List.size();i++){
				cout << "學校代碼: " << List[i].school_id << ", 學校名稱: " << List[i].school_name << ", 科系代碼: " << List[i].department_id << ", 科系名稱: "  << List[i].department_name << endl;
				cout << "日間∕進修別: "  << List[i].day << ", 等級別: " << List[i].level << ", 學生數: " << List[i].students << ", 教師數: " << List[i].teachers << ", 上學年度畢業生數: " << List[i].graduates << ", 縣市名稱: " << List[i].city << ", 體系別: " << List[i].system << endl;
			}
		}
		
		void run(vector<CustomType>& tmp){
			merge_sort(tmp, 0, tmp.size()/2, tmp.size()-1);
			tmp = teemp;
			teemp.clear();
		}

};
//******************************end*************************************


//*****************************任務一***********************************
void bubble_sort( vector<Custom>& arrary, int n)  ;//氣泡排序
void select_sort( vector<Custom>& arrary, int n)  ;//選擇排序
Custom setting_struct(Custom);//氣泡排序
void readfile();//讀檔  
void writefile(vector<Custom>& arrary, int n);//氣泡排序寫檔
void writefile2(vector<Custom>& arrary2, int n);//選擇排序寫檔

int sum=0;
struct Custom temp;//氣泡排序暫存
struct Custom t;//選擇排序暫存
//******************************end*************************************

//*****************************任務二***********************************
vector<CustomType> SchoolList;
vector<CustomType> Std_merge;
vector<CustomType> Std_quick;

string readfile_m2();
void writefile_m2(vector<CustomType>&, string, int);
void writefile_graduates_only_m2(vector<CustomType>&, string, int);
void clear_all_m2();
void Print_m2(vector<CustomType>&);
//******************************end*************************************

void readfile(){
	int i=0,j=0;
    int c;//指令
	Custom test;
    vector<Custom> arrary;//
    vector<Custom> arrary2;
	ifstream infile;
	string filename, str;
	cout << "Please input your file name (ex: 501, 502...): ";
	cin >> filename;
	cout << endl;
	filename = "input" + filename + ".txt";
	infile.open(filename.c_str());	//.c_str()用來將string 轉成 char[XXX], 因為open函數只能讀char[XXX], 但string撰寫比較方便

   if(infile==NULL){
        cout << "Fail to open " << filename << endl;
    } //NULL為"空"的意思
	else{
		cout << "完成開啟" << filename << endl;
		i=0;
        while( infile >> str!= 0 ){
            j++;
            if(j>15){
				if(i==0){
                    test.Number=str;
                    i++;
                }
                else if(i==1){
                    test.name=str;
                    i++;
                }
                else if(i==2){
                    stringstream geek(str);
                	geek >> test.code;
                    i++;
                }
                else if(i==3){
                    test.subject=str;
                    i++;
                }
                else if(i==4){
                    test.day=str;
                    i++;
                }
                else if(i==5){
                    test.day1=str;
                    i++;
                }
                else if(i==6){
                    test.level=str;
                    i++;             
				}
                else if(i==7){
                    test.c=str;
                    i++;
                }
                else if(i==8){
                    stringstream geek(str);
                    geek>>test.number;
                    i++;
                }
                else if(i==9){
                    stringstream geek(str);
                    geek>>test.number1;
                    i++;
                }
                else if(i==10){
                    stringstream geek(str);
                    geek>>test.number2;
                    i++;
                    sum++;
                }
                else if(i==11){
                    stringstream geek(str);
                    geek>>test.number3;
                    i++;
                }
                else if(i==12){
                    test.city=str;
                    i++;
                }
                else if(i==13){
                    stringstream geek(str);
                    geek >> test.a;
                    i++;
                }
                else if(i==14){
                	test.ch=str;
                    i=0;
                    arrary.push_back(test);
                    arrary2.push_back(test);
                }
            }
        }
    }
    printf("請輸入 1.气泡排序，2.選擇排序");
    scanf("%d",&c);
    if(c==1){bubble_sort(arrary, sum);}
    else if(c==2){select_sort(arrary2, sum);}
    else{
    	cout << "no such command." << endl;
	}
	
	arrary.clear();	//reset
	arrary2.clear(); //reset
	sum=0; //reset
}

void writefile(vector<Custom>& arrary, int n){
	ofstream outfile;//(#include <fstream>), ofstream = fstream中的"out"
	string filename = "select_sort.txt";

	outfile.open(filename.c_str());//嘗試建檔
	if(outfile==NULL){cout << "Fail to save " << filename << endl;} //NULL為"空"的意思
	else{
		cout << "完成建立" << filename << endl;
	   	for(int i=0;i<n;i++){//print
	        outfile <<arrary[i].Number<<'\t'<< arrary[i].name<<'\t'<< arrary[i].code<<'\t'<<  arrary[i].subject<<'\t'<<arrary[i].day<<'\t'
		    << arrary[i].day1<<'\t'<< arrary[i].level<<'\t'<<arrary[i].c<<'\t'<<arrary[i].number<<'\t'<<arrary[i].number1<<'\t'
		    <<arrary[i].number2<<'\t'<<arrary[i].number3<<'\t'<<arrary[i].city<<'\t'<< arrary[i].a<<'\t'<<arrary[i].ch<<endl; //outfile後面的東西全部流入outfile中
		}
		cout << endl;
    }
}
void writefile2(vector<Custom>& arrary, int n){
	ofstream outfile;//(#include <fstream>), ofstream = fstream中的"out"
	string filename = "bubble_sort.txt";

	outfile.open(filename.c_str());//嘗試建檔
	if(outfile==NULL){cout << "Fail to save " << filename << endl;} //NULL為"空"的意思
	else{
		cout << "完成建立" << filename << endl;
	    for(int i=0;i<n;i++){//print
	        outfile <<arrary[i].Number<<'\t'<< arrary[i].name<<'\t'<< arrary[i].code<<'\t'<<  arrary[i].subject<<'\t'<<arrary[i].day<<'\t'
		    << arrary[i].day1<<'\t'<< arrary[i].level<<'\t'<<arrary[i].c<<'\t'<<arrary[i].number<<'\t'<<arrary[i].number1<<'\t'
		    <<arrary[i].number2<<'\t'<<arrary[i].number3<<'\t'<<arrary[i].city<<'\t'<< arrary[i].a<<'\t'<<arrary[i].ch<<endl; //outfile後面的東西全部流入outfile中
		}
		cout << endl;
	}
}

void select_sort(vector<Custom>& arrary2, int n){      //傳入數組的要排序的元素個數
	int i,j,max;
	int begintime,endtime;
	begintime = clock();
	for(i=0;i<n-1;i++){
	    max=i;     //max:當前最小值下標
		for(j=i+1;j<n;j++){
			if(arrary2[max].number2<arrary2[j].number2){max=j;}    //若有其它元素更大，就記錄其下標
		}
	
		if(max!=i){
			t=arrary2[max];
			arrary2[max]=arrary2[i]; //保若最大值不在排序區首位，就換到首位
			arrary2[i]=t;
		}
	}
	endtime = clock();
	printf("select_sort：%dms\n", endtime-begintime);
	writefile(arrary2,n);
	//for(i=0;i<n;i++){
	    //cout<<arrary2[i].Number<< ' ' << arrary2[i].name<< ' ' << arrary2[i].code<<  arrary2[i].subject<<' '<<arrary2[i].day<<' '
	    //<< arrary2[i].day1<< ' '<< arrary2[i].level<<' '<<arrary2[i].c<< ' '<<arrary2[i].number<<' '<<arrary2[i].number1<< ' '
	    //<<arrary2[i].number2<<' '<<arrary2[i].number3<< ' '<<arrary2[i].city<<' '<< arrary2[i].a<<' '<<arrary2[i].ch<<endl;
	//}
}


void bubble_sort( vector<Custom>& arrary, int n){       //傳入數組的要排序的元素個數  
	int flag=1,i,j;
	int Border;//無序數列的邊界
	int begintime,endtime;
	begintime = clock();
	Border=n-1;//邊界
	int last= 0;//記錄最後一次交換的位置
	for(i=0;i<n&&flag!=0;i++){
	    flag=0;
	    for(j=0;j<Border;j++){
	        if(arrary[j].number2<arrary[j+1].number2){
		        temp=arrary[j];
		        arrary[j]=arrary[j+1];
		        arrary[j+1]=temp;
		        flag=1;//有元素交換，標記變為1
		        last = j;//最後一次交換元素的位置
	        }
	    }
	    Border = last;
	}
	endtime = clock();
	printf("bubble_sort：%dms\n", endtime-begintime);
    writefile2(arrary,n);
	//for(i=0;i<n;i++){
	    //cout<<arrary[i].Number<< '\t' << arrary[i].name<< '\t' << arrary[i].code<<  arrary[i].subject<<'\t'<<arrary[i].day<<'\t'
	    //<< arrary[i].day1<< '\t'<< arrary[i].level<<'\t'<<arrary[i].c<< '\t'<<arrary[i].number<<'\t'<<arrary[i].number1<< '\t'
	   	 //<<arrary[i].number2<<'\t'<<arrary[i].number3<< '\t'<<arrary[i].city<<'\t'<< arrary[i].a<<'\t'<<arrary[i].ch<<endl;
	//}
}

string readfile_m2(){
	ifstream infile;//(#include <fstream>), ifstream = fstream中的"in" 
	string filename, str;
	cout << "Please input your file name (ex: 501, 502...): ";
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

void writefile_m2(vector<CustomType>& tmp, string filename, int cmd){//Std_merge Std_quick
	ofstream outfile;
	filename.erase(0, 5);
	
	if(cmd==0){
		filename = "merge_" + filename;
	}
	else if(cmd==1){
		filename = "quick_" + filename;
	}
	outfile.open(filename.c_str());//嘗試建檔 
	if(outfile==NULL){cout << "Fail to save " << filename << endl;}
	else{
		for(int i=0; i<tmp.size();i++){
			outfile << tmp[i].school_id << "\t\t" << tmp[i].school_name << "\t\t" << tmp[i].department_id << "\t\t"  << tmp[i].department_name;
			outfile << "\t\t"  << tmp[i].day << "\t\t" << tmp[i].level << "\t\t" << tmp[i].students << "\t\t" << tmp[i].teachers << "\t\t" << tmp[i].graduates << "\t\t" << tmp[i].city << "\t\t" << tmp[i].system << endl;
		}
		cout << "完成輸出" << filename << endl; 
	}
}

void writefile_graduates_only_m2(vector<CustomType>& tmp, string filename, int cmd){//Std_merge Std_quick
	ofstream outfile;
	filename.erase(0, 5);
	
	if(cmd==0){
		filename = "merge_" + filename;
	}
	else if(cmd==1){
		filename = "quick_" + filename;
	}
	outfile.open(filename.c_str());//嘗試建檔 
	if(outfile==NULL){cout << "Fail to save " << filename << endl;}
	else{
		for(int i=0; i<tmp.size();i++){
			outfile << tmp[i].graduates << "\t\t" << tmp[i].school_id << "\t\t" << tmp[i].school_name << "\t\t" << tmp[i].department_id << "\t\t"  << tmp[i].department_name << endl;
			//outfile << tmp[i].school_id << "aaa1" << tmp[i].school_name << "aaa2" << tmp[i].department_id << "aaa3"  << tmp[i].department_name;
			//outfile << "aaa4"  << tmp[i].day << "aaa5" << tmp[i].level << "aaa6" << tmp[i].students << "aaa7" << tmp[i].teachers << "aaa8" << tmp[i].graduates << "aaa9" << tmp[i].city << "aaa10" << tmp[i].system << endl;
		}
		cout << "完成輸出" << filename << endl; 
	}
}

void clear_all_m2(){
	SchoolList.clear();
	Std_merge.clear();
	Std_merge.clear();
}

void Print_m2(vector<CustomType>&List){
	for(int i=0;i<List.size();i++){
		cout << "學校代碼: " << List[i].school_id << ", 學校名稱: " << List[i].school_name << ", 科系代碼: " << List[i].department_id << ", 科系名稱: "  << List[i].department_name << endl;
		cout << "日間∕進修別: "  << List[i].day << ", 等級別: " << List[i].level << ", 學生數: " << List[i].students << ", 教師數: " << List[i].teachers << ", 上學年度畢業生數: " << List[i].graduates << ", 縣市名稱: " << List[i].city << ", 體系別: " << List[i].system << endl;
	}
}

int main(int argc, char** argv){
	int cmd;
	clock_t start, end;
	while(1){
		cout << "*********************************" << endl;
		cout << "*0: quit			*" << endl;
		cout << "*1: Bubble Sort & Selection Sort*" << endl;
		cout << "*2: Merge Sort & Quick Sort	*" << endl;
		cout << "*********************************" << endl;
		cout << "Input your command: ";
		cin >> cmd;
		cout << endl;
		if(cmd==0){
			break;
		}
		else if(cmd==1){
			readfile();
			cout << endl;
		}
		else if(cmd==2){
			CLASS_Quick quick;
			CLASS_Merge merged;
			string filename;
			
			filename = readfile_m2();
			cout << endl;
			
			if(filename != "0"){
				Std_merge.assign(SchoolList.begin(), SchoolList.end()); //copy
				Std_quick.assign(SchoolList.begin(), SchoolList.end()); //copy
				
				start = clock();
				merged.run(Std_merge);
				end = clock();
				cout << "Merge Sort: " << end - start << " ms" << endl;
				cout << endl;
				
				start = clock();
				quick.run(Std_quick);
				end = clock();
				cout << "Quick Sort: " << end - start << " ms" << endl;
				cout << endl;
				

				//writefile_m2(Std_merge, filename, 0);
				writefile_graduates_only_m2(Std_merge, filename, 0);
				
				//writefile_m2(Std_quick, filename, 1);
				writefile_graduates_only_m2(Std_quick, filename, 1);
				
				cout << endl;
			}
			clear_all_m2();
		}
		else{
			cout << "請輸入0, 1 or 2" << endl;
			cout << endl;
		}
	}
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

