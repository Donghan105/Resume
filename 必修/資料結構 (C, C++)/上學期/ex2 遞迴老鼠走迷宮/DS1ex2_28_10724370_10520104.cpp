//10724370 馮浩陽, 10520104莊東翰
#include <iostream>
#include <stdio.h>
#include<fstream>
#include<typeinfo>
using namespace std;

class MAZE{
	private:
		
	public:
		int x1=0, y1=0, G_time = 0, goals;	//任務二goals,  G_time
		char **maze;
		char **visited;	//經過路徑
		char **merge_m;	//任務二 有走訪G唯一路徑
		
		//void step(int, int);	//任務一 
		//void Multi_G(int, int);	//任務二  
		//void Visit(int, int, char); //每走一步 修改一次visited陣列 
		//void Merge_Paths();	//任務二 合併每一次找到G唯一路徑 
		//void print_maze();	//印出maze
		//void print_visited();	//印出經過路徑 
		//void Print_Merge();	//任務二 印出所有走訪G唯一路徑 
		
		void step(int x, int y)                //用遞迴算法求解路徑 
		{
			//print_maze();
			if(G_time<1){
				if(maze[x][y] == 'G'){
			    	G_time++;
			    	print_maze();
				}
				else{
					Visit(x, y, 'V');	//走過就紀錄 
					maze[x][y] = 'R';
					if(y < x1-1){
			    		if(maze[x][y + 1] == 'G' || maze[x][y + 1] == 'E'){step(x, y + 1);}	//右 
					}
					if(x < y1-1){
						if(maze[x + 1][y] == 'G' || maze[x + 1][y] == 'E'){step(x + 1, y);}	//下 
					}
					if(y > 0){
						if(maze[x][y - 1] == 'G' || maze[x][y - 1] == 'E'){step(x, y - 1);}	//左 
					}
					if(x > 0){
						if(maze[x - 1][y] == 'G' || maze[x - 1][y] == 'E'){step(x - 1, y);}	//上 
					}
					maze[x][y] = 'E';	//走完還回E
				}
			}
		}
		
		void Multi_G(int x, int y)//用遞迴算法求解路徑 
		{
			//print_maze();
			if(G_time!=goals){	//已找到G次數作為結束條件
				if(maze[x][y] == 'G'){
			    	G_time++;
			    	Merge_Paths();
			    	maze[x][y] = 'X';
			    	if(y < x1-1){
			    		if(maze[x][y + 1] == 'G' || maze[x][y + 1] == 'E'){Multi_G(x, y + 1);}	//右 
					}
					if(x < y1-1){
						if(maze[x + 1][y] == 'G' || maze[x + 1][y] == 'E'){Multi_G(x + 1, y);}	//下 
					}
					if(y > 0){
						if(maze[x][y - 1] == 'G' || maze[x][y - 1] == 'E'){Multi_G(x, y - 1);}	//左 
					}
					if(x > 0){
						if(maze[x - 1][y] == 'G' || maze[x - 1][y] == 'E'){Multi_G(x - 1, y);}	//上 
					}
				}
				else{
					Visit(x, y, 'V');	//走過就紀錄 
					maze[x][y] = 'V';
			    	if(y < x1-1){
			    		if(maze[x][y + 1] == 'G' || maze[x][y + 1] == 'E'){Multi_G(x, y + 1);}	//右 
					}
					if(x < y1-1){
						if(maze[x + 1][y] == 'G' || maze[x + 1][y] == 'E'){Multi_G(x + 1, y);}	//下 
					}
					if(y > 0){
						if(maze[x][y - 1] == 'G' || maze[x][y - 1] == 'E'){Multi_G(x, y - 1);}	//左 
					}
					if(x > 0){
						if(maze[x - 1][y] == 'G' || maze[x - 1][y] == 'E'){Multi_G(x - 1, y);}	//上 
					}
					maze[x][y] = 'E';	//走完還回E 
				}
			}
			
		
		}
		
		void print_maze(){	//印出maze 
			for(int i=0;i<y1;i++){
				for(int j=0;j<x1;j++){
					printf("%c",maze[i][j]);
				}
				printf("\n");
			}
			printf("\n");
		}
		
		void print_visited(){
			for(int i=0;i<y1;i++){
				for(int j=0;j<x1;j++){
					printf("%c",visited[i][j]);
				}
				printf("\n");
			}
			printf("\n");
		}
		
		void Visit(int x, int y, char word){	//經過就改成V 
			visited[x][y] = word;
		}
		
		void Merge_Paths(){	//任務二 將每一次找到新的G的路徑合併 
			for(int i=0;i<y1;i++){
				for(int j=0; j<x1;j++){
					if(maze[i][j]=='V' && merge_m[i][j]=='E'){	//VVVVVG
						merge_m[i][j] = 'R';	//VVVVVG => RRRRRG
					}
				}
			}
		}
		
		void Print_Merge(){	// print merge
			for(int x = 0; x < y1; x++)
			{
				for(int y = 0; y < x1; y++)
				{
					printf("%c", merge_m[x][y]);
				}
				printf("\n");
			}
		}
};



int main(int argc, char** argv){
	while(1){
		MAZE MZ;
		MZ.x1=0, MZ.y1=0, MZ.G_time = 0;
		
		int command;
		string filename;
		int size[2]={0};	//A X B
		ifstream infile;
		
		
		cout << "Please input your command ( 0:Quit, 1:One goal, 2:More goals ): ";
		cin >> command;
		cout << endl;
		if(command==1){
			cout << "Please input your file name (ex: 201, 202...): " ;
			cin >> filename;
			cout << endl;
			filename =  "input" + filename + ".txt";
			infile.open(filename.c_str());
			if(infile==NULL){printf("Fail to open the file.\n");}
			else{
				for (int i = 0; i <=1; i++){
					infile >> size[i];
				}
				MZ.x1=size[0]; MZ.y1=size[1];	//A X B
				MZ.maze = new char*[MZ.y1];	//直行 
				MZ.visited = new char*[MZ.y1];	//直行  
				MZ.merge_m = new char*[MZ.y1];	//直行 
				
				for(int i=0;i<MZ.y1;i++){
					MZ.maze[i]=new char[MZ.x1];	//橫列 
					MZ.visited[i]=new char[MZ.x1];	//橫列 
					MZ.merge_m[i]=new char[MZ.x1];	//橫列 
				}
				
				for (int i = 0; i <MZ.y1; i++){
					for ( int j = 0; j < MZ.x1; j++){
						infile >> MZ.maze[i][j];	//讀入迷宮 
						MZ.visited[i][j] = MZ.maze[i][j];	//先讓visited 和 merge_m跟input一樣 
						MZ.merge_m[i][j] = MZ.maze[i][j];
					}	
				}
				infile.close();
				MZ.step(0, 0);	//任務一 從(0,0)開始 
				MZ.print_visited();	//印出所有經過路徑 
			}
		}
		else if(command==2){
			cout << "Please input your file name (ex: 201, 202...): " ;
			cin >> filename;
			cout << endl;
			filename =  "input" + filename + ".txt";
			infile.open(filename.c_str());
			if(infile==NULL){printf("Fail to open the file.\n");}
			else{
				cout << "Input number of goals: ";
				cin >> MZ.goals;
				cout << endl;
				
				for (int i = 0; i <=1; i++){
					infile >> size[i];
				}
				MZ.x1=size[0]; MZ.y1=size[1];	//A X B
				MZ.maze = new char*[MZ.y1];	//直行 
				MZ.visited = new char*[MZ.y1];	//直行 
				MZ.merge_m = new char*[MZ.y1];	//直行 
				
				for(int i=0;i<MZ.y1;i++){
					MZ.maze[i]=new char[MZ.x1];	//橫列 
					MZ.visited[i]=new char[MZ.x1];	//橫列 
					MZ.merge_m[i]=new char[MZ.x1];	//橫列 
				}
				
				for (int i = 0; i <MZ.y1; i++){
					for ( int j = 0; j < MZ.x1; j++){
						infile >> MZ.maze[i][j];	//讀入迷宮
						MZ.visited[i][j] = MZ.maze[i][j];	//先讓visited 和 merge_m跟input一樣 
						MZ.merge_m[i][j] = MZ.maze[i][j];
					}	
				}
				infile.close();
				
				MZ.Multi_G(0, 0);	//任務二 從(0,0)開始
				MZ.print_visited();	//印出所有經過路徑 
				
				if(MZ.G_time == MZ.goals){	//當找到G數量==N 
					cout << endl;
		    		MZ.Print_Merge();	//印出所有走訪G唯一路徑 
				}
			}
		}
		else if(command==0){
			break;
		}
		else{
			cout << "no such command." << endl;
		}
		cout << endl;
	}
 }

