//10724370 馮浩陽, 10520104莊東翰 
#include <iostream>
#include <stdio.h>
#include <string.h> //string型別, strcpy, assign, substr, erase, find, erase, strstr 
#include <vector>  //動態陣列 
#include <stdlib.h> // atoi 
#include <fstream>
#include <iomanip> //cout 小數點後幾位  
#define QSIZE 3
#include<time.h>
using namespace std;

struct JobType{
	unsigned int OID;
	unsigned int Arrival;
	unsigned int Duration;
	unsigned int TimeOut;
};

struct ResultType{
	unsigned int OID;
	unsigned int Time;
	unsigned int Delay;
};

void mission1();
void Shell_sort(unsigned int **a, int len);

vector <JobType> list;
vector <JobType> queue;
vector <ResultType> done;
vector <ResultType> aborted;

class Queue{
	//void m2_clear();	//清除任務二queue, abort, done  
	//void mission2();
	//void Read_file();
	//void Print();
	//void CPU();	//CPU排程
	//int Abort_or_not(unsigned int, unsigned int, unsigned int, unsigned int);	//判斷是否丟棄 
	//void Aborted(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);	//丟棄 
	//void Done(unsigned int, unsigned int, unsigned int, unsigned int);	//完成 
	//JobType List_get_job();	//從list中拿下一筆工作 
	//void Push_in_Q(JobType);
	//void Pop_Q();
	//void Write_file(string);
	
	public:
		void mission2(){
			Read_file();	
		}
		
		void m2_clear(){	//清除任務二queue, abort, done 
			list.clear();
			queue.clear();
			done.clear();
			aborted.clear();
		}
		
	private:
		void Write_file(string filename){
			ofstream outfile;
			float avg_delay = 0, success_rate, items, tmp;
			
			outfile.open(filename.c_str());
			if(outfile==NULL){cout << "Fail to save " << filename << endl;}
			else{
				outfile << "\t[Abort Jobs]" << endl;
				outfile << "\tOID\t" << "Abort\t" << "Delay" << endl;
				for(int i=0; i<aborted.size();i++){
					outfile << "["  << i+1 << "]\t" << aborted[i].OID << "\t" << aborted[i].Time << "\t" << aborted[i].Delay << endl;
					avg_delay = avg_delay + aborted[i].Delay;
				}
				
				outfile << "\t[Jobs Done]" << endl;
				outfile << "\tOID\t" << "Departure\t" << "Delay" << endl;
				for(int i=0; i<done.size();i++){
					outfile << "["  << i+1 << "]\t" << done[i].OID << "\t" << done[i].Time << "\t" << done[i].Delay << endl;
					avg_delay = avg_delay + done[i].Delay;
				}
				
				items = aborted.size() + done.size();
				avg_delay = avg_delay/items;
				outfile << "[Average Delay]\t";
				outfile << std::fixed <<  std::setprecision(2) << avg_delay << " ms" << endl; //後2位 
				success_rate = done.size()/items;
				outfile << "[Success Rate]\t";
				outfile << std::fixed <<  std::setprecision(2) << success_rate*100 << " %" << endl; //後2位 
				
				cout << endl;
				cout << "see " << filename << endl;
			}
			outfile.close();	
		}
		
		void Pop_Q(){
			queue.erase(queue.begin());	//從queue中刪除 
		}
		
		void Push_in_Q(JobType job){
			queue.push_back(job);
		}
		
		JobType List_get_job(JobType job){	//一次拿一個工作 
			job.OID = list[0].OID;
			job.Arrival = list[0].Arrival;
			job.Duration = list[0].Duration;
			job.TimeOut =  list[0].TimeOut;
			return job;
		}
		
		void Pop_list(){
			list.erase(list.begin());	//從list中刪除 
		}
		
		void Aborted(unsigned int error_code, unsigned int job_id, unsigned int job_arrival, unsigned int cpu_clock, unsigned int job_timeout){
			ResultType job;
			if(error_code==1){	//進queue前檢查長度 
				job.OID = job_id;
				job.Time = job_arrival;
				job.Delay = 0;
				aborted.push_back(job);
				
			}
			else if(error_code==2){	//cpu執行時檢查逾時 (超過clock) 		
				job.OID = job_id;
				job.Time = job_timeout;	//abort time
				job.Delay = job_timeout - job_arrival;
				aborted.push_back(job);
			}
			else if(error_code==3){	//從queue取出時檢查逾時 
				job.OID = job_id;
				job.Time = cpu_clock;	//abort time
				job.Delay = cpu_clock - job_arrival;
				aborted.push_back(job);
			}
		}
		
		void Done(unsigned int job_id, unsigned int cpu_clock, unsigned int job_duration, unsigned int job_arrival){	//完成  
			ResultType tmp;
			tmp.OID = job_id;
			tmp.Time = cpu_clock + job_duration;
			tmp.Delay = cpu_clock - job_arrival;
			done.push_back(tmp);
		}
		
		
		int Abort_or_not(int command, int q_size, int cpu_clock, int job_timeout, int job_duration){	//判斷是否丟棄  
			//cout << "q_size: " <<  q_size << endl; 
			if(command==1 && q_size==QSIZE){	//進queue前檢查長度 
				//cout << "queue已滿" << endl; 
				return 1;
			}
			else if(command==2 && cpu_clock + job_duration > job_timeout){	//cpu執行時檢查逾時 (超過clock) 
				//cout << "cpu執行時發生逾時 (超過clock) " << endl; 
				return 2;
			}
			else if(command==3 && cpu_clock >= job_timeout){	//從queue取出時檢查逾時 
				//cout << "從queue取出時發現逾時" << endl;
				return 3;
			}
			else{	//無誤 
				return 0;
			}
		}
		
		void CPU(){
			unsigned int cpu_clock = 0, error_code;
			JobType job;
			
			
			while(cpu_clock==0){	//先做第一個工作 
				//cout << "===================================Print===================================" << endl; 
				job = List_get_job(job);
				Pop_list();
				error_code = Abort_or_not(2, queue.size(), cpu_clock, job.TimeOut, job.Duration);	//cpu執行時檢查逾時 (超過clock) 
				if(error_code == 0){	//success
					cpu_clock =  job.Arrival;
					Done(job.OID, cpu_clock, job.Duration, job.Arrival);
					cpu_clock = cpu_clock + job.Duration;
				}
				else{	//failed 
					Aborted(error_code, job.OID, job.Arrival, cpu_clock, job.TimeOut);
				}
				//cout << "cpu clock: " << cpu_clock << endl; 
				//cout << "===================================END_print================================" << endl; 
				cout << endl;
			}
		
			if(list.size() > 0){	//先取好下一個job  
				job = List_get_job(job);
				Pop_list();
				if(list.size()==0){
					Push_in_Q(job); //只有兩筆資料, 最後一筆放入queue 
				} 
			}
			
			while(list.size()!=0){	//還有job 
				while(cpu_clock>=job.Arrival){
					//cout << "===================================Print===================================" << endl; 
					if(cpu_clock==job.Arrival){	//剛好一樣 
						if(queue.size()==0){	//剛好Q長度為0 直接做 
							error_code = Abort_or_not(2, queue.size(), cpu_clock, job.TimeOut, job.Duration);	//cpu執行時檢查逾時 (超過clock) 
							if(error_code == 0){	//cpu執行不會逾時 
									//cout << "done: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl;
									Done(job.OID, cpu_clock, job.Duration, job.Arrival);
									cpu_clock = cpu_clock + job.Duration;
							}
							else{
								//cout << "abbort: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl;
								cpu_clock = job.TimeOut;	//執行到timeout才放棄, 時間已浪費 
								//cout << "cpu clock: " << cpu_clock << endl; 
								Aborted(error_code, job.OID, job.Arrival, cpu_clock, job.TimeOut);	//丟棄 
								job.OID = -1;	//丟棄 
							}
							
							if(list.size() > 0){	//還有東西 
								job = List_get_job(job);
								Pop_list();
							}
							else{
								break;		
							}
						}
						else{	//queue有東西, 一次處理一個job 
							//cout << "正在處理queue" << endl; 
							error_code = Abort_or_not(3, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);
							if(error_code == 0){	//從queue取出時不會逾時 
								error_code = Abort_or_not(2, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);	//cpu執行不會逾時 
								if(error_code == 0){
									//cout << "done_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
									Done(queue[0].OID, cpu_clock, queue[0].Duration, queue[0].Arrival);
									cpu_clock = cpu_clock + queue[0].Duration;
									Pop_Q();	//pop from queue  
								}
								else{
									//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
									cpu_clock = queue[0].TimeOut;	//執行到timeout才放棄, 時間已浪費 
									//cout << "cpu clock: " << cpu_clock << endl; 
									Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//丟棄 
									Pop_Q();	//pop from queue 
								}
							}
							else{
								//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
								Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut); //丟棄 
								Pop_Q();	//pop from queue 
							}
						}
					}
					else{ //cpu_clock>job.Arrival 
						error_code = Abort_or_not(1, queue.size(), cpu_clock, job.TimeOut, job.Duration);	//是否queue已滿  
						//cout << "error_code: " << error_code << endl;
						if(error_code == 0){	//success, queue沒滿 
								//cout << "push_in_Q: " << job.OID << endl;
								Push_in_Q(job);
						}
						else{	//失敗, queue已滿  
							//cout << "abbort: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl; 
							Aborted(error_code, job.OID, job.Arrival, cpu_clock, job.TimeOut);
							job.OID = -1;	//丟棄 
						}
						
						if(list.size() > 0){	//還有東西 
							job = List_get_job(job);
							Pop_list();
						}
						else{
							break;		
						}
					} 
					
					//cout << "cpu_clock " << cpu_clock << endl;
					//cout << "queue.size(): " << queue.size() << endl;
					//Print();
					//cout << "===================================END_print================================" << endl;
					//cout << endl;
				}// end 2nd while
				
				if(queue.size()!=0){	//cpu_clock<job.Arrival, queue有東西, 一次處理一個job 
					//cout << "===================================Print===================================" << endl;
					//cout << "正在處理queue" << endl; 
					error_code = Abort_or_not(3, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);
					if(error_code == 0){	//從queue取出時不會逾時 
						error_code = Abort_or_not(2, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);	//cpu執行不會逾時 
						if(error_code == 0){
							//cout << "done_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
							Done(queue[0].OID, cpu_clock, queue[0].Duration, queue[0].Arrival);
							cpu_clock = cpu_clock + queue[0].Duration;
							Pop_Q();	//pop from queue 	
						}
						else{
							//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl;
							cpu_clock = queue[0].TimeOut;	//執行到timeout才放棄, 時間已浪費 
							//cout << "cpu clock: " << cpu_clock << endl; 
							Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);
							Pop_Q();	//pop from queue 
						}
					}
					else{
						//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl;
						Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//丟棄 
						Pop_Q();
					}
					//Print();
					//cout << "===================================END_print================================" << endl;
				}
				
				
				if(queue.size()==0 && cpu_clock<job.Arrival){	//若queue清完但是cpu clock仍小於下一個job  
					cpu_clock = job.Arrival; //clock調成下一筆job的arrival, while可以繼續跑 
				}
			}// end 1st while
			
			//cout << "list已拿完" << endl; 
			//cout << "list已拿完job: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl; 
			//cout << endl;
			
			if(queue.size()==0 && cpu_clock==job.Arrival){	//list已拿完	queue也沒東西 剩最後一筆job 直接做 
				error_code = Abort_or_not(2, queue.size(), cpu_clock, job.TimeOut, job.Duration);	//cpu執行時檢查逾時 (超過clock) 
				if(error_code == 0){	//cpu執行不會逾時 
						//cout << "done: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl;
						Done(job.OID, cpu_clock, job.Duration, job.Arrival);
						cpu_clock = cpu_clock + job.Duration;
				}
				else{
					//cout << "abbort: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl;
					cpu_clock = job.TimeOut;	//執行到timeout才放棄, 時間已浪費 
					//cout << "cpu clock: " << cpu_clock << endl; 
					Aborted(error_code, job.OID, job.Arrival, cpu_clock, job.TimeOut);	//丟棄 
					job.OID = -1;	//丟棄 
				}
			}
			
			if(queue.size()!=0){	//list已拿完	處理最後的queue 
				while(queue.size()!=0){
					//cout << "===================================Print===================================" << endl;
					if(cpu_clock>=queue[0].Arrival){
						error_code = Abort_or_not(3, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);
						if(error_code == 0){	//從queue取出時不會逾時 
							error_code = Abort_or_not(2, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);	//cpu執行不會逾時 
							if(error_code == 0){
								//cout << "done_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
								Done(queue[0].OID, cpu_clock, queue[0].Duration, queue[0].Arrival);	//完成 
								cpu_clock = cpu_clock + queue[0].Duration;
								Pop_Q();	//pop from queue 
							}
							else{
								//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
								cpu_clock = queue[0].TimeOut;	//執行到timeout才放棄, 時間已浪費 
								//cout << "cpu clock: " << cpu_clock << endl; 
								Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//丟棄 
								Pop_Q();	//pop from queue 
							}
						}
						else{
							//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
							Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//丟棄 
							Pop_Q();	//pop from queue 
						}
						
						if(job.OID != -1){	//未被丟棄的最後job 
							Push_in_Q(job);	//最後一筆放入queue 
							job.OID = -1;
						}
						
						//Print();
						//cout << "===================================END_print================================" << endl;
						//cout << endl;	
					}
					else{	//cpu clock小於queue下一個job 
						job.OID = -1; 
						cpu_clock = queue[0].Arrival; //clock調成下一筆job的arrival, while可以繼續跑 
						error_code = Abort_or_not(3, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);
						if(error_code == 0){	//從queue取出時不會逾時 
							error_code = Abort_or_not(2, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);	//cpu執行不會逾時 
							if(error_code == 0){
								//cout << "done_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
								Done(queue[0].OID, cpu_clock, queue[0].Duration, queue[0].Arrival);	//完成 
								cpu_clock = cpu_clock + queue[0].Duration;
								Pop_Q();	//pop from queue 
							}
							else{
								//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
								cpu_clock = queue[0].TimeOut;	//執行到timeout才放棄, 時間已浪費 
								//cout << "cpu clock: " << cpu_clock << endl; 
								Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//丟棄 
								Pop_Q();	//pop from queue 
							}
						}
						else{
							//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
							Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//丟棄 
							Pop_Q();	//pop from queue 
						}
						
						//Print();
						//cout << "===================================END_print================================" << endl;
						//cout << endl;
					}
				}
			}
			
			cout << "RESULT:" << endl;
			Print();
		}
		
		void Print(){
			float avg_delay = 0, success_rate, items;
			
			cout << "\t[List]" << endl;
			cout << "\tOID\t" << "Arrival\t" << "Duration\t" << "TimeOut" << endl;
			for(int i=0; i<list.size();i++){
				cout << "["  << i+1 << "]\t" << list[i].OID << "\t" << list[i].Arrival << "\t" << list[i].Duration << "\t" << list[i].TimeOut << endl;
			}
			cout << endl;
			
			cout << "\t[Queue]" << endl;
			cout << "\tOID\t" << "Arrival\t" << "Duration\t" << "TimeOut" << endl;
			for(int i=0; i<queue.size();i++){
				cout << "["  << i+1 << "]\t" << queue[i].OID << "\t" << queue[i].Arrival << "\t" << queue[i].Duration << "\t" << queue[i].TimeOut << endl;
			}
			cout << endl;
			
			cout << "\t[Abort Jobs]" << endl;
			cout << "\tOID\t" << "Abort\t" << "Delay" << endl;
			for(int i=0; i<aborted.size();i++){
				cout << "["  << i+1 << "]\t" << aborted[i].OID << "\t" << aborted[i].Time << "\t" << aborted[i].Delay << endl;
				avg_delay = avg_delay + aborted[i].Delay;
			}
			
			cout << "\t[Jobs Done]" << endl;
			cout << "\tOID\t" << "Departure\t" << "Delay" << endl;
			for(int i=0; i<done.size();i++){
				cout << "["  << i+1 << "]\t" << done[i].OID << "\t" << done[i].Time << "\t" << done[i].Delay << endl;
				avg_delay = avg_delay + done[i].Delay;
			}
			
			items = aborted.size() + done.size();
			avg_delay = avg_delay/items;
			cout << "[Average Delay] ";
			printf("%.2f", avg_delay);
			cout << " ms" << endl;
			success_rate = done.size()/items;
			cout << "[Success Rate] ";
			printf("%.2f", success_rate*100);
			cout << " %" << endl;
		}
		
		void Read_file(){
			string filename, output_file;
			ifstream infile;
			
			cout << "Please input your file name (ex: 401, 402...): " ;
			cin >> filename;
			cout << endl;
			output_file = "output" + filename + ".txt";
			filename = "sorted" + filename + ".txt";
			infile.open(filename.c_str());
			
			if(infile==NULL){cout << "Fail to open " << filename << endl;}
			else{
				string ch;
				unsigned int data, cycle = 1;
				JobType job;
				
				infile >> ch;	//讀垃圾 
				infile >> ch;
				infile >> ch;
				infile >> ch;
				
				while(infile >> ch){
					if(ch==" "){
						//cout << "空白" << endl; 
					}
					else if(ch=="\n"){
						//cout << "\\n" << endl; 
					}
					else if(ch=="\t"){
						//cout << "\\t" << endl; 
					}
					else{
						data = atoi(ch.c_str());	//string to const_char*, const_char* to int 
					}
					if(cycle==4){
						job.TimeOut = data;
						list.push_back(job);	//放入list中 
					}
					else if(cycle==1){
						job.OID = data;
					}
					else if(cycle==2){
						job.Arrival = data;
					}
					else if(cycle==3){
						job.Duration = data;
					}
					
					cycle++;	//4次一循環 
					if(cycle==5){
						cycle = 1;	//歸零 
					}
				}
				CPU();
				Write_file(output_file);
			}
			infile.close();
		}	
};

void mission1(){  //mission1 
	char b[4];
    FILE *f1 = NULL;
    unsigned int **a; //二維陣列 
    int n=0;//計算含有几個數 
    int i, j,k,g,t,c,d;
    char p;
    char l=0; 
    clock_t start, finish;
    double time1=0,time2=0,time3=0;
    int num=0,f=0,A;
    string filename, savefile;
    
    cout << "Please input your filename:";//輸入需要打開的檔案 
    cin >> filename;
    savefile = "sorted" + filename + ".txt";
    filename = "input" + filename + ".txt";
    
    
    start = clock();//reading data 計時開始 
	
	
	 
    f1=fopen(filename.c_str(),"r");//讀檔 
    if(f1==NULL){printf("Fail to open the file.");}
    else{
	    while((p = fgetc(f1)) != EOF){
			if(p=='	')//如果p等於空格 
				n++;//計數加一 
		}
	    n=(n-3)/3;//計算一共有多少元素 
	    rewind(f1);//指標回到開頭 
	    a = (unsigned int**)calloc(n, sizeof(unsigned int*));//動態申請空間 
	    for(i = 0; i <n; i++){
	    	*(a + i) = (unsigned int*)calloc(4, sizeof(unsigned int));
		}
	    fscanf(f1,"%*[^\n]%*c");//跳過第一行從第二行開始讀 
	    for(i=0;i<n;i++){//將文檔中數字存入二維陣列 
			for(j=0;j<4;j++){
				fscanf(f1,"%u",&a[i][j]);
				cout << "123123: " << a[i][j] << endl;
			}
	  		fscanf(f1,"%c",&l);
	  	}
	  	
	  		finish = clock();//reading data  
		   time1 = 1000*((double)(finish - start) / CLOCKS_PER_SEC); 
		 	  
		
	  	

	   	for(i=0;i<n;i++){//印出讀到的二維陣列 
	   		for(j=0;j<4;j++){
			   printf(" %d",a[i][j]);
			}
	    	printf("\n");
		}
	
		
		  
		 start = clock();//reading data 計時開始 
	
	    Shell_sort(a,n);	//n jobs希爾排序 
		printf("\n");
		c=0,d=0,g=0,t=0;
		j=0,k=0,num=0;
		A=a[0][1];
		for(i=1;i<n;i++){//查找連續相同的arrival時間并且紀錄其始末座標 
			if(A==a[i][1]){
				num++;}
			else if(A!=a[i][1]&&num>=1){
				f=i-num;//紀錄相同的數字開通和結尾 
				//printf(" %d",i);
				 for(k=f;k<i;++k)//找到連續相同arrival時間的項，將他們重新按照ID大小排序 
  				  {
      					  t=a[k][0];//將a[k][0]備份 
       					 c=a[k][2];
       				 d=a[k][3];
        			for(j=k-1;j>=f-1 && a[j][0]>t;j--)//和前面的陣列 
       				 {

          			  a[j+1][0]=a[j][0];
           			 a[j+1][2]=a[j][2];
           			 a[j+1][3]=a[j][3];
       							 }
       				 a[j+1][0]=t;//插入 
        			a[j+1][2]=c;
       				 a[j+1][3]=d;

    }
				num=0;
				f=0;
				A=a[i][1];
			}
			else if(A!=a[i][1]){A=a[i][1];}//如果前一項和后一項不相等則繼續往下找 
		}
		
			finish = clock();//reading data  
		   time2 = 1000*((double)(finish - start) / CLOCKS_PER_SEC); 
		 	  
		
		printf("\n");
		for(i=0;i<n;i++){//印出全部排序后的陣列 
			for(j=0;j<4;j++){printf(" %d",a[i][j]);}
			printf("\n");
		}
		start = clock();//reading data 計時開始  
		//save the file
		ofstream m1_save;
		m1_save.open(savefile.c_str());
		if(m1_save==NULL){cout << "Fail to save " << savefile << endl;}
		else{
			m1_save << "OID\t" << "Arrival\t" << "Duration\t" << "TimeOut" << endl;
			for(i=0;i<n;i++){
				for(j=0;j<4;j++){
					if(j!=3){
						m1_save << a[i][j] << "\t";	
					}
					else{
						m1_save << a[i][j] << endl;
					}
				}
			}
		}
		m1_save.close();
		
	}
	finish = clock();//reading data 
		   time3 = 1000*((double)(finish - start) / CLOCKS_PER_SEC); 
		 	  	
	printf("reading data=%.0f \n  sorting data=%.0f\n  writing data %.0f",time1,time2,time3);
	printf("\n");
	
}

void Shell_sort(unsigned int **a, int len)
{   
 int b=0,c=0,d=0,temp;//
    int i;
    int j;
    int k=0;
    int gap;
    gap=len/2;//一開始gap長度為len的一半

    while(gap>0){
      for(i=0; i<gap; i++)
        {for(j=i+gap; j<len; j=j+gap)//以gap長度分組
            {
                if(a[j][1] < a[j - gap][1])//如果一組之中後一個數比前一個數小
		         {   b=a[j][0];//備份數值
		            c=a[j][2];
		            d=a[j][3];
		            temp = a[j][1];
		                    k = j - gap;//前一個陣列的座標
		
		                    while(k>=0 && a[k][1]>temp)
		                    {
		                        a[k + gap][1] = a[k][1];//將在a[j][1]前且比temp的值大的元素向後移動一位
		                        a[k+gap][0]=a[k][0];
		                        a[k+gap][2]=a[k][2];
		                        a[k+gap][3]=a[k][3];
		                        k = k - gap;
		
		                    }
		
		                    a[k + gap][1] = temp;//繼續向前比較
		                    a[k+gap][0]=b;
		                    a[k+gap][2]=c;
		                    a[k+gap][3]=d;
			
			    }
		   }
	   }
			        gap=gap/2 ;//執行完一輪gap除2   
	}
}

int main(int argc, char** argv){	
	Queue sim;
	string your_input;
	int cmd, contin;
	while(1){	
		cout << "Input your command(0:quit, 1:任務一, 2:任務二):";
		cin >> cmd;
		if(cmd==0){
			break;
		}
		else if(cmd==1){
			mission1();
		}
		else if(cmd==2){
			sim.mission2();
			sim.m2_clear();
		}
		else{
			cout << "No such command." << endl;
		}
		
		cout << "continue? (0:No, 1:Yes):";
		cin >> contin;
		cout << endl;
		if(contin==0){
			break;
		}
	}
}
