//10724370 ���E��, 10520104���F�� 
#include <iostream>
#include <stdio.h>
#include <string.h> //string���O, strcpy, assign, substr, erase, find, erase, strstr 
#include <vector>  //�ʺA�}�C 
#include <stdlib.h> // atoi 
#include <fstream>
#include <iomanip> //cout �p���I��X��  
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
	//void m2_clear();	//�M�����ȤGqueue, abort, done  
	//void mission2();
	//void Read_file();
	//void Print();
	//void CPU();	//CPU�Ƶ{
	//int Abort_or_not(unsigned int, unsigned int, unsigned int, unsigned int);	//�P�_�O�_��� 
	//void Aborted(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);	//��� 
	//void Done(unsigned int, unsigned int, unsigned int, unsigned int);	//���� 
	//JobType List_get_job();	//�qlist�����U�@���u�@ 
	//void Push_in_Q(JobType);
	//void Pop_Q();
	//void Write_file(string);
	
	public:
		void mission2(){
			Read_file();	
		}
		
		void m2_clear(){	//�M�����ȤGqueue, abort, done 
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
				outfile << std::fixed <<  std::setprecision(2) << avg_delay << " ms" << endl; //��2�� 
				success_rate = done.size()/items;
				outfile << "[Success Rate]\t";
				outfile << std::fixed <<  std::setprecision(2) << success_rate*100 << " %" << endl; //��2�� 
				
				cout << endl;
				cout << "see " << filename << endl;
			}
			outfile.close();	
		}
		
		void Pop_Q(){
			queue.erase(queue.begin());	//�qqueue���R�� 
		}
		
		void Push_in_Q(JobType job){
			queue.push_back(job);
		}
		
		JobType List_get_job(JobType job){	//�@�����@�Ӥu�@ 
			job.OID = list[0].OID;
			job.Arrival = list[0].Arrival;
			job.Duration = list[0].Duration;
			job.TimeOut =  list[0].TimeOut;
			return job;
		}
		
		void Pop_list(){
			list.erase(list.begin());	//�qlist���R�� 
		}
		
		void Aborted(unsigned int error_code, unsigned int job_id, unsigned int job_arrival, unsigned int cpu_clock, unsigned int job_timeout){
			ResultType job;
			if(error_code==1){	//�iqueue�e�ˬd���� 
				job.OID = job_id;
				job.Time = job_arrival;
				job.Delay = 0;
				aborted.push_back(job);
				
			}
			else if(error_code==2){	//cpu������ˬd�O�� (�W�Lclock) 		
				job.OID = job_id;
				job.Time = job_timeout;	//abort time
				job.Delay = job_timeout - job_arrival;
				aborted.push_back(job);
			}
			else if(error_code==3){	//�qqueue���X���ˬd�O�� 
				job.OID = job_id;
				job.Time = cpu_clock;	//abort time
				job.Delay = cpu_clock - job_arrival;
				aborted.push_back(job);
			}
		}
		
		void Done(unsigned int job_id, unsigned int cpu_clock, unsigned int job_duration, unsigned int job_arrival){	//����  
			ResultType tmp;
			tmp.OID = job_id;
			tmp.Time = cpu_clock + job_duration;
			tmp.Delay = cpu_clock - job_arrival;
			done.push_back(tmp);
		}
		
		
		int Abort_or_not(int command, int q_size, int cpu_clock, int job_timeout, int job_duration){	//�P�_�O�_���  
			//cout << "q_size: " <<  q_size << endl; 
			if(command==1 && q_size==QSIZE){	//�iqueue�e�ˬd���� 
				//cout << "queue�w��" << endl; 
				return 1;
			}
			else if(command==2 && cpu_clock + job_duration > job_timeout){	//cpu������ˬd�O�� (�W�Lclock) 
				//cout << "cpu����ɵo�͹O�� (�W�Lclock) " << endl; 
				return 2;
			}
			else if(command==3 && cpu_clock >= job_timeout){	//�qqueue���X���ˬd�O�� 
				//cout << "�qqueue���X�ɵo�{�O��" << endl;
				return 3;
			}
			else{	//�L�~ 
				return 0;
			}
		}
		
		void CPU(){
			unsigned int cpu_clock = 0, error_code;
			JobType job;
			
			
			while(cpu_clock==0){	//�����Ĥ@�Ӥu�@ 
				//cout << "===================================Print===================================" << endl; 
				job = List_get_job(job);
				Pop_list();
				error_code = Abort_or_not(2, queue.size(), cpu_clock, job.TimeOut, job.Duration);	//cpu������ˬd�O�� (�W�Lclock) 
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
		
			if(list.size() > 0){	//�����n�U�@��job  
				job = List_get_job(job);
				Pop_list();
				if(list.size()==0){
					Push_in_Q(job); //�u���ⵧ���, �̫�@����Jqueue 
				} 
			}
			
			while(list.size()!=0){	//�٦�job 
				while(cpu_clock>=job.Arrival){
					//cout << "===================================Print===================================" << endl; 
					if(cpu_clock==job.Arrival){	//��n�@�� 
						if(queue.size()==0){	//��nQ���׬�0 ������ 
							error_code = Abort_or_not(2, queue.size(), cpu_clock, job.TimeOut, job.Duration);	//cpu������ˬd�O�� (�W�Lclock) 
							if(error_code == 0){	//cpu���椣�|�O�� 
									//cout << "done: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl;
									Done(job.OID, cpu_clock, job.Duration, job.Arrival);
									cpu_clock = cpu_clock + job.Duration;
							}
							else{
								//cout << "abbort: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl;
								cpu_clock = job.TimeOut;	//�����timeout�~���, �ɶ��w���O 
								//cout << "cpu clock: " << cpu_clock << endl; 
								Aborted(error_code, job.OID, job.Arrival, cpu_clock, job.TimeOut);	//��� 
								job.OID = -1;	//��� 
							}
							
							if(list.size() > 0){	//�٦��F�� 
								job = List_get_job(job);
								Pop_list();
							}
							else{
								break;		
							}
						}
						else{	//queue���F��, �@���B�z�@��job 
							//cout << "���b�B�zqueue" << endl; 
							error_code = Abort_or_not(3, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);
							if(error_code == 0){	//�qqueue���X�ɤ��|�O�� 
								error_code = Abort_or_not(2, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);	//cpu���椣�|�O�� 
								if(error_code == 0){
									//cout << "done_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
									Done(queue[0].OID, cpu_clock, queue[0].Duration, queue[0].Arrival);
									cpu_clock = cpu_clock + queue[0].Duration;
									Pop_Q();	//pop from queue  
								}
								else{
									//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
									cpu_clock = queue[0].TimeOut;	//�����timeout�~���, �ɶ��w���O 
									//cout << "cpu clock: " << cpu_clock << endl; 
									Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//��� 
									Pop_Q();	//pop from queue 
								}
							}
							else{
								//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
								Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut); //��� 
								Pop_Q();	//pop from queue 
							}
						}
					}
					else{ //cpu_clock>job.Arrival 
						error_code = Abort_or_not(1, queue.size(), cpu_clock, job.TimeOut, job.Duration);	//�O�_queue�w��  
						//cout << "error_code: " << error_code << endl;
						if(error_code == 0){	//success, queue�S�� 
								//cout << "push_in_Q: " << job.OID << endl;
								Push_in_Q(job);
						}
						else{	//����, queue�w��  
							//cout << "abbort: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl; 
							Aborted(error_code, job.OID, job.Arrival, cpu_clock, job.TimeOut);
							job.OID = -1;	//��� 
						}
						
						if(list.size() > 0){	//�٦��F�� 
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
				
				if(queue.size()!=0){	//cpu_clock<job.Arrival, queue���F��, �@���B�z�@��job 
					//cout << "===================================Print===================================" << endl;
					//cout << "���b�B�zqueue" << endl; 
					error_code = Abort_or_not(3, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);
					if(error_code == 0){	//�qqueue���X�ɤ��|�O�� 
						error_code = Abort_or_not(2, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);	//cpu���椣�|�O�� 
						if(error_code == 0){
							//cout << "done_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
							Done(queue[0].OID, cpu_clock, queue[0].Duration, queue[0].Arrival);
							cpu_clock = cpu_clock + queue[0].Duration;
							Pop_Q();	//pop from queue 	
						}
						else{
							//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl;
							cpu_clock = queue[0].TimeOut;	//�����timeout�~���, �ɶ��w���O 
							//cout << "cpu clock: " << cpu_clock << endl; 
							Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);
							Pop_Q();	//pop from queue 
						}
					}
					else{
						//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl;
						Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//��� 
						Pop_Q();
					}
					//Print();
					//cout << "===================================END_print================================" << endl;
				}
				
				
				if(queue.size()==0 && cpu_clock<job.Arrival){	//�Yqueue�M�����Ocpu clock���p��U�@��job  
					cpu_clock = job.Arrival; //clock�զ��U�@��job��arrival, while�i�H�~��] 
				}
			}// end 1st while
			
			//cout << "list�w����" << endl; 
			//cout << "list�w����job: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl; 
			//cout << endl;
			
			if(queue.size()==0 && cpu_clock==job.Arrival){	//list�w����	queue�]�S�F�� �ѳ̫�@��job ������ 
				error_code = Abort_or_not(2, queue.size(), cpu_clock, job.TimeOut, job.Duration);	//cpu������ˬd�O�� (�W�Lclock) 
				if(error_code == 0){	//cpu���椣�|�O�� 
						//cout << "done: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl;
						Done(job.OID, cpu_clock, job.Duration, job.Arrival);
						cpu_clock = cpu_clock + job.Duration;
				}
				else{
					//cout << "abbort: " << job.OID <<  " " << job.Arrival <<  " " << cpu_clock <<  " " << job.TimeOut << endl;
					cpu_clock = job.TimeOut;	//�����timeout�~���, �ɶ��w���O 
					//cout << "cpu clock: " << cpu_clock << endl; 
					Aborted(error_code, job.OID, job.Arrival, cpu_clock, job.TimeOut);	//��� 
					job.OID = -1;	//��� 
				}
			}
			
			if(queue.size()!=0){	//list�w����	�B�z�̫᪺queue 
				while(queue.size()!=0){
					//cout << "===================================Print===================================" << endl;
					if(cpu_clock>=queue[0].Arrival){
						error_code = Abort_or_not(3, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);
						if(error_code == 0){	//�qqueue���X�ɤ��|�O�� 
							error_code = Abort_or_not(2, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);	//cpu���椣�|�O�� 
							if(error_code == 0){
								//cout << "done_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
								Done(queue[0].OID, cpu_clock, queue[0].Duration, queue[0].Arrival);	//���� 
								cpu_clock = cpu_clock + queue[0].Duration;
								Pop_Q();	//pop from queue 
							}
							else{
								//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
								cpu_clock = queue[0].TimeOut;	//�����timeout�~���, �ɶ��w���O 
								//cout << "cpu clock: " << cpu_clock << endl; 
								Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//��� 
								Pop_Q();	//pop from queue 
							}
						}
						else{
							//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
							Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//��� 
							Pop_Q();	//pop from queue 
						}
						
						if(job.OID != -1){	//���Q��󪺳̫�job 
							Push_in_Q(job);	//�̫�@����Jqueue 
							job.OID = -1;
						}
						
						//Print();
						//cout << "===================================END_print================================" << endl;
						//cout << endl;	
					}
					else{	//cpu clock�p��queue�U�@��job 
						job.OID = -1; 
						cpu_clock = queue[0].Arrival; //clock�զ��U�@��job��arrival, while�i�H�~��] 
						error_code = Abort_or_not(3, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);
						if(error_code == 0){	//�qqueue���X�ɤ��|�O�� 
							error_code = Abort_or_not(2, queue.size(), cpu_clock, queue[0].TimeOut, queue[0].Duration);	//cpu���椣�|�O�� 
							if(error_code == 0){
								//cout << "done_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
								Done(queue[0].OID, cpu_clock, queue[0].Duration, queue[0].Arrival);	//���� 
								cpu_clock = cpu_clock + queue[0].Duration;
								Pop_Q();	//pop from queue 
							}
							else{
								//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
								cpu_clock = queue[0].TimeOut;	//�����timeout�~���, �ɶ��w���O 
								//cout << "cpu clock: " << cpu_clock << endl; 
								Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//��� 
								Pop_Q();	//pop from queue 
							}
						}
						else{
							//cout << "abbort_Q: " << queue[0].OID <<  " " <<  queue[0].Arrival <<  " " <<  cpu_clock <<  " " <<  queue[0].TimeOut << endl; 
							Aborted(error_code, queue[0].OID, queue[0].Arrival, cpu_clock, queue[0].TimeOut);	//��� 
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
				
				infile >> ch;	//Ū�U�� 
				infile >> ch;
				infile >> ch;
				infile >> ch;
				
				while(infile >> ch){
					if(ch==" "){
						//cout << "�ť�" << endl; 
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
						list.push_back(job);	//��Jlist�� 
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
					
					cycle++;	//4���@�`�� 
					if(cycle==5){
						cycle = 1;	//�k�s 
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
    unsigned int **a; //�G���}�C 
    int n=0;//�p��t���L�Ӽ� 
    int i, j,k,g,t,c,d;
    char p;
    char l=0; 
    clock_t start, finish;
    double time1=0,time2=0,time3=0;
    int num=0,f=0,A;
    string filename, savefile;
    
    cout << "Please input your filename:";//��J�ݭn���}���ɮ� 
    cin >> filename;
    savefile = "sorted" + filename + ".txt";
    filename = "input" + filename + ".txt";
    
    
    start = clock();//reading data �p�ɶ}�l 
	
	
	 
    f1=fopen(filename.c_str(),"r");//Ū�� 
    if(f1==NULL){printf("Fail to open the file.");}
    else{
	    while((p = fgetc(f1)) != EOF){
			if(p=='	')//�p�Gp����Ů� 
				n++;//�p�ƥ[�@ 
		}
	    n=(n-3)/3;//�p��@�@���h�֤��� 
	    rewind(f1);//���Ц^��}�Y 
	    a = (unsigned int**)calloc(n, sizeof(unsigned int*));//�ʺA�ӽЪŶ� 
	    for(i = 0; i <n; i++){
	    	*(a + i) = (unsigned int*)calloc(4, sizeof(unsigned int));
		}
	    fscanf(f1,"%*[^\n]%*c");//���L�Ĥ@��q�ĤG��}�lŪ 
	    for(i=0;i<n;i++){//�N���ɤ��Ʀr�s�J�G���}�C 
			for(j=0;j<4;j++){
				fscanf(f1,"%u",&a[i][j]);
				cout << "123123: " << a[i][j] << endl;
			}
	  		fscanf(f1,"%c",&l);
	  	}
	  	
	  		finish = clock();//reading data  
		   time1 = 1000*((double)(finish - start) / CLOCKS_PER_SEC); 
		 	  
		
	  	

	   	for(i=0;i<n;i++){//�L�XŪ�쪺�G���}�C 
	   		for(j=0;j<4;j++){
			   printf(" %d",a[i][j]);
			}
	    	printf("\n");
		}
	
		
		  
		 start = clock();//reading data �p�ɶ}�l 
	
	    Shell_sort(a,n);	//n jobs�ƺ��Ƨ� 
		printf("\n");
		c=0,d=0,g=0,t=0;
		j=0,k=0,num=0;
		A=a[0][1];
		for(i=1;i<n;i++){//�d��s��ۦP��arrival�ɶ��}�B������l���y�� 
			if(A==a[i][1]){
				num++;}
			else if(A!=a[i][1]&&num>=1){
				f=i-num;//�����ۦP���Ʀr�}�q�M���� 
				//printf(" %d",i);
				 for(k=f;k<i;++k)//���s��ۦParrival�ɶ������A�N�L�̭��s����ID�j�p�Ƨ� 
  				  {
      					  t=a[k][0];//�Na[k][0]�ƥ� 
       					 c=a[k][2];
       				 d=a[k][3];
        			for(j=k-1;j>=f-1 && a[j][0]>t;j--)//�M�e�����}�C 
       				 {

          			  a[j+1][0]=a[j][0];
           			 a[j+1][2]=a[j][2];
           			 a[j+1][3]=a[j][3];
       							 }
       				 a[j+1][0]=t;//���J 
        			a[j+1][2]=c;
       				 a[j+1][3]=d;

    }
				num=0;
				f=0;
				A=a[i][1];
			}
			else if(A!=a[i][1]){A=a[i][1];}//�p�G�e�@���M�Z�@�����۵��h�~�򩹤U�� 
		}
		
			finish = clock();//reading data  
		   time2 = 1000*((double)(finish - start) / CLOCKS_PER_SEC); 
		 	  
		
		printf("\n");
		for(i=0;i<n;i++){//�L�X�����ƧǦZ���}�C 
			for(j=0;j<4;j++){printf(" %d",a[i][j]);}
			printf("\n");
		}
		start = clock();//reading data �p�ɶ}�l  
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
    gap=len/2;//�@�}�lgap���׬�len���@�b

    while(gap>0){
      for(i=0; i<gap; i++)
        {for(j=i+gap; j<len; j=j+gap)//�Hgap���פ���
            {
                if(a[j][1] < a[j - gap][1])//�p�G�@�դ�����@�ӼƤ�e�@�ӼƤp
		         {   b=a[j][0];//�ƥ��ƭ�
		            c=a[j][2];
		            d=a[j][3];
		            temp = a[j][1];
		                    k = j - gap;//�e�@�Ӱ}�C���y��
		
		                    while(k>=0 && a[k][1]>temp)
		                    {
		                        a[k + gap][1] = a[k][1];//�N�ba[j][1]�e�B��temp���Ȥj�������V�Ჾ�ʤ@��
		                        a[k+gap][0]=a[k][0];
		                        a[k+gap][2]=a[k][2];
		                        a[k+gap][3]=a[k][3];
		                        k = k - gap;
		
		                    }
		
		                    a[k + gap][1] = temp;//�~��V�e���
		                    a[k+gap][0]=b;
		                    a[k+gap][2]=c;
		                    a[k+gap][3]=d;
			
			    }
		   }
	   }
			        gap=gap/2 ;//���槹�@��gap��2   
	}
}

int main(int argc, char** argv){	
	Queue sim;
	string your_input;
	int cmd, contin;
	while(1){	
		cout << "Input your command(0:quit, 1:���Ȥ@, 2:���ȤG):";
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
