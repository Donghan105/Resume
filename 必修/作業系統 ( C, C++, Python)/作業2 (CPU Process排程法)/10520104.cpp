# include <iostream>
# include <stdio.h>
# include <string> // string型別, strcpy, assign, substr, erase, find, erase, strstr 
# include <vector>  // 動態陣列 
# include <stdlib.h> // atoi 
# include <sstream> // char to string  
#include <fstream>
using namespace std;


struct TaskType{
  int pid;
  int cpu_burst;
  int arrival;
  int priority;
  int remaining;
  float response_ratio;
  float turnaround_time;
  float waiting_time;
};


void Readfile( string filename, vector<TaskType>& tmp );
void WriteFile( string filename, vector<int>& tmp, string method );
void WriteFile_ALL( string, vector<int>&, vector<int>&, vector<int>&, vector<int>&, vector<int>&, vector<TaskType>&, vector<TaskType>&, vector<TaskType>&, vector<TaskType>&, vector<TaskType>& );
string Get_Gant_Chart( vector<int>& tmp );
void Sort_Jobs( vector<TaskType>& jobs );
void Sort( string mode );
bool All_jobs_are_done();
void Update( vector<TaskType>& q, vector<TaskType>& tmp );
vector<int> FCFS();
vector<int> RR();
vector<int> SRTF();
void SRTF_sort( vector<TaskType>& q );
vector<int> PPRR();
void Sort_PPRR( vector<TaskType>& q );
int Max_priority( vector<TaskType>& q );
bool RR_or_not( vector<TaskType>& q );
vector<int> HRRN();
void Sort_HRRN( vector<TaskType>& q, float timer );

void Print( vector<TaskType>& );



vector<TaskType> jobs;
int mode = 0;
int time_slice = 0;



void Readfile( string filename, vector<TaskType>& tmp ) { // ext:副檔名  
  ifstream infile;
  string str;
  int number;
  infile.open(filename.c_str());
	
  if ( infile == NULL ) { cout << "Fail to open " << filename << endl; }
  else {
    cout << "完成開啟" << filename << endl; 
    infile >> str;
    mode = atoi(str.c_str());
    
    infile >> str;
    time_slice = atoi(str.c_str());
    
    infile >> str;
    infile >> str;
    infile >> str;
    infile >> str;
    infile >> str;
    infile >> str;

	while ( infile >> str != 0 ) {
	  if ( str == " " ) {
				//cout << "空白" << endl; 
	  }
	  else if ( str == "\n" ) {
		//cout << "\\n" << endl; 
	  }
	  else if ( str == "\t" ) {
		//cout << "\\t" << endl; 
	  }
	  else {
	  	TaskType element;
	  	element.turnaround_time = -1.0;
	  	element.waiting_time = -1.0;
        element.response_ratio = -1.0;

	  	element.pid = atoi(str.c_str());
	  	infile >> str;
	  	element.cpu_burst = atoi(str.c_str());
	  	element.remaining = element.cpu_burst;
	  	infile >> str;
	  	element.arrival = atoi(str.c_str());
	  	infile >> str;
	  	element.priority = atoi(str.c_str());
        tmp.push_back( element );
	  }
	}
  }
}

void WriteFile( string filename, vector<int>& tmp, string method ) {
  string result = "";
  for ( int i = 0; i < tmp.size(); i++ ) {
    if ( tmp[i] == -1 ) {
      result += "-";
	}
    else if ( tmp[i] == 0 ) {
      result += "0";
	}
    else if ( tmp[i] == 1 ) {
      result += "1";
	}
    else if ( tmp[i] == 2 ) {
      result += "2";	
	}
    else if ( tmp[i] == 3 ) {
      result += "3";	
	}
    else if ( tmp[i] == 4 ) {
      result += "4";	
	}
    else if ( tmp[i] == 5 ) {
      result += "5";	
	}
    else if ( tmp[i] == 6 ) {
      result += "6";	
	}
    else if ( tmp[i] == 7 ) {
      result += "7";	
	}
    else if ( tmp[i] == 8 ) {
      result += "8";	
	}
    else if ( tmp[i] == 9 ) {
      result += "9";	
	}
    else if ( tmp[i] == 10 ) {
      result += "A";	
	}
    else if ( tmp[i] == 11 ) {
      result += "B";	
	}
    else if ( tmp[i] == 12 ) {
      result += "C";	
	}
    else if ( tmp[i] == 13 ) {
      result += "D";	
	}
    else if ( tmp[i] == 14 ) {
      result += "E";	
	}
    else if ( tmp[i] == 15 ) {
      result += "F";	
	}
    else if ( tmp[i] == 16 ) {
      result += "G";	
	}
    else if ( tmp[i] == 17 ) {
      result += "H";	
	}
    else if ( tmp[i] == 18 ) {
      result += "I";	
	}
    else if ( tmp[i] == 19 ) {
      result += "J";	
	}
    else if ( tmp[i] == 20 ) {
      result += "K";	
	}
    else if ( tmp[i] == 21 ) {
      result += "L";	
	}
    else if ( tmp[i] == 22 ) {
      result += "M";	
	}
    else if ( tmp[i] == 23 ) {
      result += "N";	
	}
    else if ( tmp[i] == 24 ) {
      result += "O";	
	}
    else if ( tmp[i] == 25 ) {
      result += "P";	
	}
    else if ( tmp[i] == 26 ) {
      result += "Q";	
	}
    else if ( tmp[i] == 27 ) {
      result += "R";	
	}
    else if ( tmp[i] == 28 ) {
      result += "S";	
	}
    else if ( tmp[i] == 29 ) {
      result += "T";	
	}
    else if ( tmp[i] == 30 ) {
      result += "U";	
	}
    else if ( tmp[i] == 31 ) {
      result += "V";	
	}
    else if ( tmp[i] == 32 ) {
      result += "W";	
	}
    else if ( tmp[i] == 33 ) {
      result += "X";	
	}
    else if ( tmp[i] == 34 ) {
      result += "Y";	
	}
    else if ( tmp[i] == 35 ) {
      result += "Z";	
	}
  }

  for ( int i = 0; i < jobs.size(); i++ ) {
  	for ( int j = 0; j < jobs.size(); j++ ) {
      if ( j != jobs.size() - 1 ) {
  	    if ( jobs[j].pid > jobs[j+1].pid ) {
  	  	  TaskType tmp;  // swap 
  	  	  tmp = jobs[j];
  	  	  jobs[j] = jobs[j+1];
  	  	  jobs[j+1] = tmp;
	    }
	  }
	}
  }
 

  ofstream outfile;
  filename = "out_" + filename;

  outfile.open(filename.c_str());

  if ( outfile == NULL ){cout << "Fail to save " << filename << endl;}
  else {
  	outfile << method << endl;
  	outfile << result << endl;
  	outfile << "===========================================================" << endl;
  	outfile << endl;
  	
  	outfile << "waiting" << endl;
  	outfile << "ID	" + method << endl;
  	outfile << "===========================================================" << endl;
  	for ( int i = 0; i < jobs.size(); i++ ) {
  	  outfile << jobs[i].pid << "\t" << jobs[i].waiting_time << endl;
  	}
  	outfile << "===========================================================" << endl;
  	outfile << endl;

  	outfile << "Turnaround Time" << endl;
  	outfile << "ID	" + method << endl;
  	outfile << "===========================================================" << endl;
  	for ( int i = 0; i < jobs.size(); i++ ) {
  	  outfile << jobs[i].pid << "\t" << jobs[i].turnaround_time << endl;
  	}

  	cout << filename << " saved successfully" << endl;
  }
  outfile.close();
}

void WriteFile_ALL( string filename, vector<int>& gant_chart_FCFS, vector<int>& gant_chart_RR, vector<int>& gant_chart_SRTF, vector<int>& gant_chart_PPRR, vector<int>& gant_chart_HRRN, vector<TaskType>& jobs_FCFS, vector<TaskType>& jobs_RR, vector<TaskType>& jobs_SRTF, vector<TaskType>& jobs_PPRR, vector<TaskType>& jobs_HRRN ) {
  string FCFS_result = "", RR_result = "", SRTF_result = "", PPRR_result = "", HRRN_result = "";

  FCFS_result = Get_Gant_Chart( gant_chart_FCFS );
  RR_result = Get_Gant_Chart( gant_chart_RR );
  SRTF_result = Get_Gant_Chart( gant_chart_SRTF );
  PPRR_result = Get_Gant_Chart( gant_chart_PPRR );
  HRRN_result = Get_Gant_Chart( gant_chart_HRRN );


  Sort_Jobs( jobs_FCFS );
  Sort_Jobs( jobs_RR );
  Sort_Jobs( jobs_SRTF );
  Sort_Jobs( jobs_PPRR );
  Sort_Jobs( jobs_HRRN );
  

  ofstream outfile;
  filename = "out_" + filename;

  outfile.open(filename.c_str());


  if ( outfile == NULL ){cout << "Fail to save " << filename << endl;}
  else {
  	outfile << "All" << endl;
  	outfile << "==        FCFS==" << endl;
  	outfile << FCFS_result << endl;
  	outfile << "==          RR==" << endl;
  	outfile << RR_result << endl;
  	outfile << "==        SRTF==" << endl;
  	outfile << SRTF_result << endl;
  	outfile << "==        PPRR==" << endl;
  	outfile << PPRR_result << endl;
  	outfile << "==        HRRN==" << endl;
  	outfile << HRRN_result << endl;
  	outfile << "===========================================================" << endl;
  	outfile << endl;
  	
  	outfile << "waiting" << endl;
  	outfile << "ID	FCFS	RR	SRTF	PPRR	HRRN" << endl;
  	outfile << "===========================================================" << endl;
  	for ( int i = 0; i < jobs_FCFS.size(); i++ ) {
  	  outfile << jobs_FCFS[i].pid << "\t" << jobs_FCFS[i].waiting_time << "\t" << jobs_RR[i].waiting_time << "\t" << jobs_SRTF[i].waiting_time << "\t" << jobs_PPRR[i].waiting_time << "\t" << jobs_HRRN[i].waiting_time << endl;
  	}
  	outfile << "===========================================================" << endl;
  	outfile << endl;

  	outfile << "Turnaround Time" << endl;
  	outfile << "ID	FCFS	RR	SRTF	PPRR	HRRN" << endl;
  	outfile << "===========================================================" << endl;
  	for ( int i = 0; i < jobs_FCFS.size(); i++ ) {
  	  outfile << jobs_FCFS[i].pid << "\t" << jobs_FCFS[i].turnaround_time << "\t" << jobs_RR[i].turnaround_time << "\t" << jobs_SRTF[i].turnaround_time << "\t" << jobs_PPRR[i].turnaround_time << "\t" << jobs_HRRN[i].turnaround_time << endl;
  	}
  	outfile << "===========================================================" << endl;

  	cout << filename << " saved successfully" << endl;
  }
  outfile.close();
}

string Get_Gant_Chart( vector<int>& tmp ) {
  string result = "";
    for ( int i = 0; i < tmp.size(); i++ ) {
    if ( tmp[i] == -1 ) {
      result += "-";
	}
    else if ( tmp[i] == 0 ) {
      result += "0";
	}
    else if ( tmp[i] == 1 ) {
      result += "1";
	}
    else if ( tmp[i] == 2 ) {
      result += "2";	
	}
    else if ( tmp[i] == 3 ) {
      result += "3";	
	}
    else if ( tmp[i] == 4 ) {
      result += "4";	
	}
    else if ( tmp[i] == 5 ) {
      result += "5";	
	}
    else if ( tmp[i] == 6 ) {
      result += "6";	
	}
    else if ( tmp[i] == 7 ) {
      result += "7";	
	}
    else if ( tmp[i] == 8 ) {
      result += "8";	
	}
    else if ( tmp[i] == 9 ) {
      result += "9";	
	}
    else if ( tmp[i] == 10 ) {
      result += "A";	
	}
    else if ( tmp[i] == 11 ) {
      result += "B";	
	}
    else if ( tmp[i] == 12 ) {
      result += "C";	
	}
    else if ( tmp[i] == 13 ) {
      result += "D";	
	}
    else if ( tmp[i] == 14 ) {
      result += "E";	
	}
    else if ( tmp[i] == 15 ) {
      result += "F";	
	}
    else if ( tmp[i] == 16 ) {
      result += "G";	
	}
    else if ( tmp[i] == 17 ) {
      result += "H";	
	}
    else if ( tmp[i] == 18 ) {
      result += "I";	
	}
    else if ( tmp[i] == 19 ) {
      result += "J";	
	}
    else if ( tmp[i] == 20 ) {
      result += "K";	
	}
    else if ( tmp[i] == 21 ) {
      result += "L";	
	}
    else if ( tmp[i] == 22 ) {
      result += "M";	
	}
    else if ( tmp[i] == 23 ) {
      result += "N";	
	}
    else if ( tmp[i] == 24 ) {
      result += "O";	
	}
    else if ( tmp[i] == 25 ) {
      result += "P";	
	}
    else if ( tmp[i] == 26 ) {
      result += "Q";	
	}
    else if ( tmp[i] == 27 ) {
      result += "R";	
	}
    else if ( tmp[i] == 28 ) {
      result += "S";	
	}
    else if ( tmp[i] == 29 ) {
      result += "T";	
	}
    else if ( tmp[i] == 30 ) {
      result += "U";	
	}
    else if ( tmp[i] == 31 ) {
      result += "V";	
	}
    else if ( tmp[i] == 32 ) {
      result += "W";	
	}
    else if ( tmp[i] == 33 ) {
      result += "X";	
	}
    else if ( tmp[i] == 34 ) {
      result += "Y";	
	}
    else if ( tmp[i] == 35 ) {
      result += "Z";	
	}
  }
  
  return result;
}

void Sort_Jobs( vector<TaskType>& jobs ) { // 輸出檔案時根據PID排列  
  for ( int i = 0; i < jobs.size(); i++ ) {
  	for ( int j = 0; j < jobs.size(); j++ ) {
      if ( j != jobs.size() - 1 ) {
  	    if ( jobs[j].pid > jobs[j+1].pid ) {
  	  	  TaskType tmp;  // swap 
  	  	  tmp = jobs[j];
  	  	  jobs[j] = jobs[j+1];
  	  	  jobs[j+1] = tmp;
	    }
	  }
	}
  }
}

void Sort( string mode ) { // 每個方法開始前先排列好, 照順序拿出工作放入queue即可  
  string mode1 = "", mode2 = "", mode3 = "";
  if ( mode == "FCFS" || mode == "RR" ) {
  	mode1 = "arrival";
  	mode2 = "pid";
  }
  else if ( mode == "SRTF" ) {
  	mode1 = "arrival";
  	mode2 = "burst";
  	mode3 = "pid";
  }
  else if ( mode == "PPRR" ) {
  	mode1 = "arrival";
  	mode2 = "priority";
  }
  else if ( mode == "HRRN" ) {
  	mode1 = "arrival";
  	mode2 = "response_ratio";
  	mode3 = "pid";
  }

  if ( mode1 == "arrival" ) {
    if ( mode2 == "burst" ) {
  	  if ( mode3 == "pid" ) { // SRTF 
  	  	for ( int i = 0; i < jobs.size(); i++ ) { // buublesort 
  	  	  for ( int j = 0; j < jobs.size(); j++ ) {
  	  	  	if ( j != jobs.size() - 1 ) {
  	  	  	  if ( jobs[j].arrival > jobs[j+1].arrival ) { // arrival
  	  	  	  	TaskType tmp;  // swap 
  	  	  	  	tmp = jobs[j];
  	  	  	  	jobs[j] = jobs[j+1];
  	  	  	  	jobs[j+1] = tmp;
			  }
			  else if ( jobs[j].arrival == jobs[j+1].arrival ) {
			  	if ( jobs[j].cpu_burst > jobs[j+1].cpu_burst ) { // burst
  	  	  	  	  TaskType tmp;  // swap 
  	  	  	  	  tmp = jobs[j];
  	  	  	  	  jobs[j] = jobs[j+1];
  	  	  	  	  jobs[j+1] = tmp;
				}
				else if ( jobs[j].cpu_burst == jobs[j+1].cpu_burst ) {
				  if ( jobs[j].pid > jobs[j+1].pid ) { // pid
  	  	  	  	    TaskType tmp;  // swap 
  	  	  	  	    tmp = jobs[j];
  	  	  	  	    jobs[j] = jobs[j+1];
  	  	  	  	    jobs[j+1] = tmp;
				  }
				}
			  }
			}
		  }
		}
	  }
    }
    else if ( mode2 == "priority" ) { // PPRR 
      for ( int i = 0; i < jobs.size(); i++ ) { 
  	  	for ( int j = 0; j < jobs.size(); j++ ) {
  	  	  if ( j != jobs.size() - 1 ) {
  	  	  	if ( jobs[j].arrival > jobs[j+1].arrival ) { // arrival
  	  	  	  TaskType tmp;  // swap 
  	  	  	  tmp = jobs[j];
  	  	  	  jobs[j] = jobs[j+1];
  	  	  	  jobs[j+1] = tmp;
			}
		    else if ( jobs[j].priority == jobs[j+1].priority ) {
		      if ( jobs[j].arrival > jobs[j+1].arrival ) {
  	  	        TaskType tmp;  // swap 
  	  	        tmp = jobs[j];
  	  	        jobs[j] = jobs[j+1];
  	  	        jobs[j+1] = tmp;
		      }
		      else if ( jobs[j].arrival == jobs[j+1].arrival ) {
		  	    if ( jobs[j].pid > jobs[j+1].pid ) {
  	  	          TaskType tmp;  // swap 
  	  	          tmp = jobs[j];
  	  	          jobs[j] = jobs[j+1];
  	  	          jobs[j+1] = tmp;	
			    }
		      }
		    } 
	      }
	    }
	  } 
    }
    else if ( mode2 == "pid" ) { // FCFS, RR 
  	  for ( int i = 0; i < jobs.size(); i++ ) { // buublesort 
  	  	for ( int j = 0; j < jobs.size(); j++ ) {
  	  	  if ( j != jobs.size() - 1 ) {
  	  	  	if ( jobs[j].arrival > jobs[j+1].arrival ) { // arrival
  	  	  	  TaskType tmp;  // swap 
  	  	  	  tmp = jobs[j];
  	  	  	  jobs[j] = jobs[j+1];
  	  	  	  jobs[j+1] = tmp;
			}
			else if ( jobs[j].arrival == jobs[j+1].arrival ) {
			  if ( jobs[j].pid > jobs[j+1].pid ) { // pid 
  	  	  	  	TaskType tmp;  // swap 
  	  	  	  	tmp = jobs[j];
  	  	  	  	jobs[j] = jobs[j+1];
  	  	  	  	jobs[j+1] = tmp;
			  }
		    }
	      }
	    }
	  }
	}
	else if ( mode2 == "response_ratio" ) {
  	  if ( mode3 == "pid" ) { // HRRN
  	  	for ( int i = 0; i < jobs.size(); i++ ) { // buublesort 
  	  	  for ( int j = 0; j < jobs.size(); j++ ) {
  	  	  	if ( j != jobs.size() - 1 ) {
  	  	  	  if ( jobs[j].arrival > jobs[j+1].arrival ) { // arrival
  	  	  	  	TaskType tmp;  // swap 
  	  	  	  	tmp = jobs[j];
  	  	  	  	jobs[j] = jobs[j+1];
  	  	  	  	jobs[j+1] = tmp;
			  }
			  else if ( jobs[j].arrival == jobs[j+1].arrival ) {
			  	if ( jobs[j].response_ratio < jobs[j+1].response_ratio ) { // response_ratio
  	  	  	  	  TaskType tmp;  // swap 
  	  	  	  	  tmp = jobs[j];
  	  	  	  	  jobs[j] = jobs[j+1];
  	  	  	  	  jobs[j+1] = tmp;
				}
				else if ( jobs[j].response_ratio == jobs[j+1].response_ratio ) {
				  if ( jobs[j].pid > jobs[j+1].pid ) { // pid
  	  	  	  	    TaskType tmp;  // swap 
  	  	  	  	    tmp = jobs[j];
  	  	  	  	    jobs[j] = jobs[j+1];
  	  	  	  	    jobs[j+1] = tmp;
				  }
				}
			  }
			}
		  }
		}
	  }	
	}
  }
}

bool All_jobs_are_done() { // 確定所有工作都完成  
  bool done = true;
  for ( int i = 0; i < jobs.size() && done == true; i++ ) {
  	if ( jobs[i].remaining != 0 ) {
  	  done = false;
	}
  }
  
  return done;
}

void Update( vector<TaskType>& q, vector<TaskType>& tmp ) { // 做完的工作把turnaround time, waiting time寫入  
  for ( int i = 0; i < q.size(); i++ ) {
  	for ( int j = 0; j < tmp.size(); j++ ) {
  	  if ( q[i].pid == tmp[j].pid ) {
  	    tmp[j] = q[i];
	  } 
	}
  }
}

vector<int> FCFS() {
  int timer = 0;
  vector<int> gant_chart;
  vector<TaskType> queue;
  Sort( "FCFS" );
  // Print(jobs);
  
  while ( All_jobs_are_done() == false ) {
  	for ( int i = 0; i < jobs.size(); i++ ) {
  	  if ( timer == jobs[i].arrival ) { // add to queue
  	    queue.push_back(jobs[i]);
	  }
	}
	
	if ( queue.size() != 0 ) {
	  gant_chart.push_back( queue[0].pid ); // 加入甘特圖  
	  queue[0].remaining--;
	  if ( queue[0].remaining == 0 ) {
	  	queue[0].turnaround_time = ( timer + 1 ) - queue[0].arrival; // 計算 turnaround_time 
	  	queue[0].waiting_time = queue[0].turnaround_time - queue[0].cpu_burst; // 計算 waiting_time  
	  	Update(queue, jobs);
	  	queue.erase(queue.begin());
	  }
	}
	else {
	  gant_chart.push_back( -1 );
	}
	  
	timer++; 
  }
  
  return gant_chart;
}

vector<int> RR() {
  int timer = 0, slice_remain;
  vector<int> gant_chart;
  vector<TaskType> queue;
  Sort( "RR" );
  // Print(jobs);
  
  while ( All_jobs_are_done() == false ) {
  	if ( queue.size() == 0 ) {
  	  for ( int i = 0; i < jobs.size(); i++ ) {
  	    if ( timer == jobs[i].arrival ) {
  	      queue.push_back(jobs[i]);
	    }
	  }
    }
	
	if ( queue.size() != 0 ) {
	  slice_remain = time_slice;
	  while ( slice_remain != 0 && All_jobs_are_done() == false ) { // 時間片段尚未用完  
	  	bool done = false;
	  	gant_chart.push_back( queue[0].pid );
	    queue[0].remaining--;

	    if ( queue[0].remaining == 0 ) { // 做完了 cpu burst = 0  
	  	  queue[0].turnaround_time = ( timer + 1 ) - queue[0].arrival;
	  	  queue[0].waiting_time = queue[0].turnaround_time - queue[0].cpu_burst;
	  	  Update(queue, jobs);
	  	  queue.erase(queue.begin());
	  	  slice_remain = 1; // break 
	  	  done = true;
	    }

		slice_remain--;
		timer++;
		
  	    for ( int i = 0; i < jobs.size(); i++ ) {
  	      if ( timer == jobs[i].arrival ) { // 新工作先加入queue 
  	        queue.push_back(jobs[i]);
	      }
	    }
		
		if ( slice_remain == 0 && done == false ) { // 把time slice用完卻還沒做完  排回隊尾   
		  TaskType tmp;
		  tmp = queue[0];
	  	  queue.erase(queue.begin());
	  	  queue.push_back(tmp); // 排回隊尾  
		}
	  }
	}
	else {
	  gant_chart.push_back( -1 );
	  timer++;
	}	  
  }

//  cout << "waiting: " << endl; 
//  for( int i = 0; i < jobs.size(); i++ ) {
//  	cout << "ID: " << jobs[i].pid << ", " << jobs[i].waiting_time << endl; 
//  }
//
//  cout << "turnaround: " << endl; 
//  for( int i = 0; i < jobs.size(); i++ ) {
//  	cout << "ID: " << jobs[i].pid << ", " << jobs[i].turnaround_time << endl; 
//  }

  return gant_chart;
}

vector<int> SRTF() {
  int timer = 0;
  vector<int> gant_chart;
  vector<TaskType> queue;
  Sort( "SRTF" );
  // Print(jobs);
  
  while ( All_jobs_are_done() == false ) { 
  	for ( int i = 0; i < jobs.size(); i++ ) {
  	  if ( timer == jobs[i].arrival ) { // add to queue
  	    queue.push_back(jobs[i]);
	  }
	}
	
	if ( queue.size() != 0 ) {
	  SRTF_sort( queue ); // 排序, 時間剩餘最少排前面  
	  gant_chart.push_back( queue[0].pid );
      queue[0].remaining--;
	  if ( queue[0].remaining == 0 ) {
	  	queue[0].turnaround_time = ( timer + 1 ) - queue[0].arrival;
	  	queue[0].waiting_time = queue[0].turnaround_time - queue[0].cpu_burst;
	  	Update(queue, jobs);
	  	queue.erase(queue.begin());
	  }
	}
	else {
	  gant_chart.push_back( -1 );
	}
	timer++;
  }
  

  return gant_chart;	
}

void SRTF_sort( vector<TaskType>& q ) {
  for ( int i = 0; i < q.size(); i++ ) { // buublesort 
    for ( int j = 0; j < q.size(); j++ ) {
      if ( j != q.size() - 1 ) {
        if ( q[j].remaining > q[j+1].remaining ) { // remaining 
          TaskType tmp;  // swap 
          tmp = q[j];
          q[j] = q[j+1];
          q[j+1] = tmp;
        }
        else if ( q[j].remaining == q[j+1].remaining ) {
          if ( q[j].arrival > q[j+1].arrival ) { // arrival 
            TaskType tmp;  // swap 
            tmp = q[j];
            q[j] = q[j+1];
            q[j+1] = tmp;
		  }
		  else if ( q[j].arrival == q[j+1].arrival ) {
            if ( q[j].pid > q[j+1].pid ) { // pid
              TaskType tmp;  // swap 
              tmp = q[j];
              q[j] = q[j+1];
              q[j+1] = tmp;
            }
		  }
        }
      }
    }
  }
}

vector<int> PPRR() {
  int timer = 0, slice_remain;
  vector<int> gant_chart;
  vector<TaskType> queue;
  Sort( "PPRR" );
  // Print(jobs);
  while ( All_jobs_are_done() == false ) {
  	if ( queue.size() == 0 ) {
      for ( int i = 0; i < jobs.size(); i++ ) {
  	    if ( timer == jobs[i].arrival ) {
  	      queue.push_back(jobs[i]);
	    }
  	  }
      Sort_PPRR( queue ); // queue中只針對優先權重新排序, 免得打亂RR的順序  
    } 
	
	if ( queue.size() != 0 ) {
	  gant_chart.push_back( queue[0].pid );
	  timer++;
	  queue[0].remaining--;
      for ( int i = 0; i < jobs.size(); i++ ) {
  	    if ( timer == jobs[i].arrival ) { // 新工作先加入queue 
  	      queue.push_back(jobs[i]);
	    }
	  }
	  
	  if ( queue[0].remaining == 0 ) { // 做完了 cpu burst = 0  
	  	queue[0].turnaround_time = timer - queue[0].arrival;
	  	queue[0].waiting_time = queue[0].turnaround_time - queue[0].cpu_burst;
	  	Update(queue, jobs);
	  	queue.erase(queue.begin());
	  	Sort_PPRR( queue ); // queue中只針對優先權重新排序, 免得打亂RR的順序   
	  }
	  else { // 尚未做完  才需要考慮是否需要RR  
	    if ( RR_or_not( queue ) == true ) { // 需要RR  
	      slice_remain = time_slice - 1; // 前面做過一秒了 扣回來   
	  	  int priority = queue[0].priority;

		  if ( slice_remain == 0 && queue[0].remaining != 0 ) { // 把time slice用完卻還沒做完, 滾到同優先權的最尾端   
		    for ( int i = 1; i < queue.size(); i++ ) { 
		      if ( i != queue.size() - 1 && queue[i+1].priority != priority ) {  // 0到i有priority相同  
		        TaskType tmp;
		        tmp = queue[0];
		        queue.insert(queue.begin() + i + 1, tmp); // queue[0]排回同priority隊尾 
	  	        queue.erase(queue.begin());
                i = queue.size(); // break for 
		      }
		      else if ( i == queue.size() - 1 ) { // queue內priority都一樣  
		        TaskType tmp;
		        tmp = queue[0];
		        queue.insert(queue.begin() + i + 1, tmp); // queue[0]排回同priority隊尾 
	  	        queue.erase(queue.begin());
                i = queue.size(); // break for 
			  }
		    }
		  }
	      else { 
	        while ( slice_remain != 0 && Max_priority( queue ) == queue[0].priority ) { // time slice剩餘 > 1的情況 且仍為最大優先權 開始連續做  
	  	      gant_chart.push_back( queue[0].pid );
	          queue[0].remaining--;
		      slice_remain--;
		      timer++;

    		  for ( int i = 0; i < jobs.size(); i++ ) {
  	            if ( timer == jobs[i].arrival ) { // 新工作先加入queue 
  	              queue.push_back(jobs[i]);
	            }
	          }
	        
		      if ( slice_remain == 0 && queue[0].remaining != 0 ) { // 把time slice用完卻還沒做完, 滾到同優先權的最尾端   
		        for ( int i = 1; i < queue.size(); i++ ) { 
		          if ( i != queue.size() - 1 && queue[i+1].priority != priority ) {  // 0到i有priority相同  
		            TaskType tmp;
		            tmp = queue[0];
		            queue.insert(queue.begin() + i + 1, tmp); // queue[0]排回同priority隊尾 
	  	            queue.erase(queue.begin());
                    i = queue.size(); // break for 
		          }
		          else if ( i == queue.size() - 1 ) { // queue內priority都一樣  
		            TaskType tmp;
		            tmp = queue[0];
		            queue.insert(queue.begin() + i + 1, tmp); // queue[0]排回同priority隊尾 
	  	            queue.erase(queue.begin());
                    i = queue.size(); // break for 
			      }
		        }
		      }
	          else if ( queue[0].remaining == 0 ) { // 做完了 cpu burst = 0  
	  	        queue[0].turnaround_time = timer - queue[0].arrival;
	  	        queue[0].waiting_time = queue[0].turnaround_time - queue[0].cpu_burst;
	  	        Update(queue, jobs);
	  	        queue.erase(queue.begin());
	  	        slice_remain = 0; // break 
	          }
	        }
	        
	        if ( slice_remain != 0 && queue[0].remaining != 0 && Max_priority( queue ) != queue[0].priority ) { // time slice中途因新工作priority較大而退出  
		      for ( int i = 1; i < queue.size(); i++ ) { 
		        if ( i != queue.size() - 1 && queue[i+1].priority != priority ) {  // 0到i有priority相同  
		          TaskType tmp;
		          tmp = queue[0];
		          queue.insert(queue.begin() + i + 1, tmp); // queue[0]排回同priority隊尾 
	  	          queue.erase(queue.begin());
                  i = queue.size(); // break for 
		        }
		      }
			}
		  }
	    }
	  }
	  Sort_PPRR( queue ); // queue中只針對優先權重新排序, 免得打亂RR的順序   
	}
	else {
	  gant_chart.push_back( -1 );
	  timer++;
	}	  
  }

  return gant_chart;
}

void Sort_PPRR( vector<TaskType>& q ) {
  for ( int i = 0; i < q.size(); i++ ) { 
  	for ( int j = 0; j < q.size(); j++ ) {
  	  if ( j != q.size() - 1 ) {
		if ( q[j].priority > q[j+1].priority ) { // priority
  	  	  TaskType tmp;  // swap 
  	  	  tmp = q[j];
  	  	  q[j] = q[j+1];
  	  	  q[j+1] = tmp;
		}
//		else if ( q[j].priority == q[j+1].priority ) {
//		  if ( q[j].arrival > q[j+1].arrival ) {
//  	  	    TaskType tmp;  // swap 
//  	  	    tmp = q[j];
//  	  	    q[j] = q[j+1];
//  	  	    q[j+1] = tmp;
//		  }
//		  else if ( q[j].arrival == q[j+1].arrival ) {
//		  	if ( q[j].pid > q[j+1].pid ) {
//  	  	      TaskType tmp;  // swap 
//  	  	      tmp = q[j];
//  	  	      q[j] = q[j+1];
//  	  	      q[j+1] = tmp;	
//			}
//		  }
//		} 
	  }
	}
  }
}

int Max_priority( vector<TaskType>& q ) { // 回傳目前最大priority  
  int max = q[0].priority;
  for ( int i = 1; i < q.size(); i++ ) { 
    if ( q[i].priority < max ) {
      max = q[i].priority;
	}
  }
  
  return max;
}

bool RR_or_not( vector<TaskType>& q ) {
  bool result = false;
  for ( int i = 1; i < q.size(); i++ ) {
  	if ( q[i].priority == q[0].priority ) {
  	  result = true;
  	  break;
	}
  }
  return result;
}

vector<int> HRRN() {
  float timer = 0.0;
  vector<int> gant_chart;
  vector<TaskType> queue;
  Sort( "HRRN" );
  // Print(jobs);
  
  while ( All_jobs_are_done() == false ) {
  	for ( int i = 0; i < jobs.size(); i++ ) {
  	  if ( timer == jobs[i].arrival ) { // add to queue
  	    queue.push_back(jobs[i]);
	  }
	}
	Sort_HRRN( queue, timer ); // 根據response ratio做排序 
	
	if ( queue.size() != 0 ) {
	  while ( queue[0].remaining != 0 ) { // non-preemptive 
	  	gant_chart.push_back( queue[0].pid );
	    queue[0].remaining--;

	    if ( queue[0].remaining == 0 ) { // 做完了  
	  	  queue[0].turnaround_time = ( timer + 1 ) - queue[0].arrival;
	  	  queue[0].waiting_time = queue[0].turnaround_time - queue[0].cpu_burst;
	  	  Update(queue, jobs);
	    }
	    else {
	      timer++;
  	      for ( int i = 0; i < jobs.size(); i++ ) { // 若有新工作  
  	        if ( timer == jobs[i].arrival ) { // add to queue
  	          queue.push_back(jobs[i]);
	        }
	      }	
		}
	  }
	  queue.erase(queue.begin());
	}
	else {
	  gant_chart.push_back( -1 );
	}
	  
	timer++; 
  }
  
//  for ( int i = 0; i < gant_chart.size(); i++ ) {
//    cout << "gant_chart: " << gant_chart[i] << endl;
//  }
//  
//  cout << "waiting: " << endl; 
//  for( int i = 0; i < jobs.size(); i++ ) {
//  	cout << "ID: " << jobs[i].pid << ", " << jobs[i].waiting_time << endl; 
//  }
//
//  cout << "turnaround: " << endl; 
//  for( int i = 0; i < jobs.size(); i++ ) {
//  	cout << "ID: " << jobs[i].pid << ", " << jobs[i].turnaround_time << endl; 
//  }
  
  return gant_chart;
}

void Sort_HRRN( vector<TaskType>& q, float timer ) {
  for ( int i = 0; i < q.size(); i++ ) {
  	q[i].response_ratio = ( ( timer - q[i].arrival ) / q[i].remaining ) + 1.0; // waiting time + 1  
  } 
  for ( int i = 0; i < q.size(); i++ ) { // buublesort 
  	for ( int j = 0; j < q.size(); j++ ) {
  	  if ( j != q.size() - 1 ) {
		if ( q[j].response_ratio < q[j+1].response_ratio ) { // response_ratio
  	  	  TaskType tmp;  // swap 
  	  	  tmp = q[j];
  	  	  q[j] = q[j+1];
  	  	  q[j+1] = tmp;
		}
		else if ( q[j].response_ratio == q[j+1].response_ratio ) {
		  if ( q[j].arrival > q[j+1].pid ) { // arrival
  	  	  	TaskType tmp;  // swap 
  	  	  	tmp = q[j];
  	  	  	q[j] = q[j+1];
  	  	  	q[j+1] = tmp;
		  }
		  else if ( q[j].arrival == q[j+1].pid ) {
		    if ( q[j].pid > q[j+1].pid ) { // pid
  	  	  	  TaskType tmp;  // swap 
  	  	  	  tmp = q[j];
  	  	  	  q[j] = q[j+1];
  	  	  	  q[j+1] = tmp;
		    }
		  } 
		}
	  }
	}
  }
}



void Print(vector<TaskType>& tmp) {
  for( int i = 0; i < tmp.size(); i++ ) {
  	cout << "ID:" << tmp[i].pid << " CPU Burst: " << tmp[i].cpu_burst; 
  	cout << " Arrival Time: " << tmp[i].arrival << " Priority: " << tmp[i].priority << endl; 
  }
  cout << endl;
}



int main() {
  string filename;
  vector<int> gant_chart; 
  int ascii_sum, index;

  cout << "input file name(含副檔名): ";
  cin >> filename;
  cout << endl; 

  Readfile( filename, jobs );
  // Print(jobs);
  
  if ( mode == 1 ) { // FCFS 
  	gant_chart = FCFS();
  	WriteFile( filename, gant_chart, "FCFS" );
  }
  else if ( mode == 2 ) { // RR 
  	gant_chart = RR();
  	WriteFile( filename, gant_chart, "RR" );
  }
  else if ( mode == 3 ) { // SRTF 
  	gant_chart = SRTF();
  	WriteFile( filename, gant_chart, "SRTF" );
  }
  else if ( mode == 4 ) { // PPRR ////////////////////////////////////到這  
    gant_chart = PPRR();
    WriteFile( filename, gant_chart, "PPRR" );
  }
  else if ( mode == 5 ) { // HRRN 
  	gant_chart = HRRN();
  	WriteFile( filename, gant_chart, "HRRN" );
  }
  else if ( mode == 6 ) { // ALL
    vector<int> gant_chart_FCFS, gant_chart_RR, gant_chart_SRTF, gant_chart_PPRR, gant_chart_HRRN;
    vector<TaskType> jobs_FCFS, jobs_RR, jobs_SRTF, jobs_PPRR, jobs_HRRN;


  	gant_chart_FCFS = FCFS();
  	for ( int i = 0; i < jobs.size(); i++ ) {
  	  jobs_FCFS.push_back( jobs[i] );
	}
  	jobs.clear();
  	
  	Readfile( filename, jobs );
    gant_chart_RR = RR();
  	for ( int i = 0; i < jobs.size(); i++ ) {
  	  jobs_RR.push_back( jobs[i] );
	}
  	jobs.clear();

  	Readfile( filename, jobs );
    gant_chart_SRTF = SRTF();
  	for ( int i = 0; i < jobs.size(); i++ ) {
  	  jobs_SRTF.push_back( jobs[i] );
	}
  	jobs.clear();

  	Readfile( filename, jobs );
    gant_chart_PPRR = PPRR();
  	for ( int i = 0; i < jobs.size(); i++ ) {
  	  jobs_PPRR.push_back( jobs[i] );
	}
  	jobs.clear();

  	Readfile( filename, jobs );
    gant_chart_HRRN = HRRN();
  	for ( int i = 0; i < jobs.size(); i++ ) {
  	  jobs_HRRN.push_back( jobs[i] );
	}
  	jobs.clear();


  	WriteFile_ALL( filename, gant_chart_FCFS, gant_chart_RR, gant_chart_SRTF, gant_chart_PPRR, gant_chart_HRRN, jobs_FCFS, jobs_RR, jobs_SRTF, jobs_PPRR, jobs_HRRN );
  }
  
  return 0;
} // main() 

