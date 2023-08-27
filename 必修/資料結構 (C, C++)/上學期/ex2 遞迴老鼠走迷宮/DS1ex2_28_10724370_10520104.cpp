//10724370 ���E��, 10520104���F��
#include <iostream>
#include <stdio.h>
#include<fstream>
#include<typeinfo>
using namespace std;

class MAZE{
	private:
		
	public:
		int x1=0, y1=0, G_time = 0, goals;	//���ȤGgoals,  G_time
		char **maze;
		char **visited;	//�g�L���|
		char **merge_m;	//���ȤG �����XG�ߤ@���|
		
		//void step(int, int);	//���Ȥ@ 
		//void Multi_G(int, int);	//���ȤG  
		//void Visit(int, int, char); //�C���@�B �ק�@��visited�}�C 
		//void Merge_Paths();	//���ȤG �X�֨C�@�����G�ߤ@���| 
		//void print_maze();	//�L�Xmaze
		//void print_visited();	//�L�X�g�L���| 
		//void Print_Merge();	//���ȤG �L�X�Ҧ����XG�ߤ@���| 
		
		void step(int x, int y)                //�λ��j��k�D�Ѹ��| 
		{
			//print_maze();
			if(G_time<1){
				if(maze[x][y] == 'G'){
			    	G_time++;
			    	print_maze();
				}
				else{
					Visit(x, y, 'V');	//���L�N���� 
					maze[x][y] = 'R';
					if(y < x1-1){
			    		if(maze[x][y + 1] == 'G' || maze[x][y + 1] == 'E'){step(x, y + 1);}	//�k 
					}
					if(x < y1-1){
						if(maze[x + 1][y] == 'G' || maze[x + 1][y] == 'E'){step(x + 1, y);}	//�U 
					}
					if(y > 0){
						if(maze[x][y - 1] == 'G' || maze[x][y - 1] == 'E'){step(x, y - 1);}	//�� 
					}
					if(x > 0){
						if(maze[x - 1][y] == 'G' || maze[x - 1][y] == 'E'){step(x - 1, y);}	//�W 
					}
					maze[x][y] = 'E';	//�����٦^E
				}
			}
		}
		
		void Multi_G(int x, int y)//�λ��j��k�D�Ѹ��| 
		{
			//print_maze();
			if(G_time!=goals){	//�w���G���Ƨ@����������
				if(maze[x][y] == 'G'){
			    	G_time++;
			    	Merge_Paths();
			    	maze[x][y] = 'X';
			    	if(y < x1-1){
			    		if(maze[x][y + 1] == 'G' || maze[x][y + 1] == 'E'){Multi_G(x, y + 1);}	//�k 
					}
					if(x < y1-1){
						if(maze[x + 1][y] == 'G' || maze[x + 1][y] == 'E'){Multi_G(x + 1, y);}	//�U 
					}
					if(y > 0){
						if(maze[x][y - 1] == 'G' || maze[x][y - 1] == 'E'){Multi_G(x, y - 1);}	//�� 
					}
					if(x > 0){
						if(maze[x - 1][y] == 'G' || maze[x - 1][y] == 'E'){Multi_G(x - 1, y);}	//�W 
					}
				}
				else{
					Visit(x, y, 'V');	//���L�N���� 
					maze[x][y] = 'V';
			    	if(y < x1-1){
			    		if(maze[x][y + 1] == 'G' || maze[x][y + 1] == 'E'){Multi_G(x, y + 1);}	//�k 
					}
					if(x < y1-1){
						if(maze[x + 1][y] == 'G' || maze[x + 1][y] == 'E'){Multi_G(x + 1, y);}	//�U 
					}
					if(y > 0){
						if(maze[x][y - 1] == 'G' || maze[x][y - 1] == 'E'){Multi_G(x, y - 1);}	//�� 
					}
					if(x > 0){
						if(maze[x - 1][y] == 'G' || maze[x - 1][y] == 'E'){Multi_G(x - 1, y);}	//�W 
					}
					maze[x][y] = 'E';	//�����٦^E 
				}
			}
			
		
		}
		
		void print_maze(){	//�L�Xmaze 
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
		
		void Visit(int x, int y, char word){	//�g�L�N�令V 
			visited[x][y] = word;
		}
		
		void Merge_Paths(){	//���ȤG �N�C�@�����s��G�����|�X�� 
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
				MZ.maze = new char*[MZ.y1];	//���� 
				MZ.visited = new char*[MZ.y1];	//����  
				MZ.merge_m = new char*[MZ.y1];	//���� 
				
				for(int i=0;i<MZ.y1;i++){
					MZ.maze[i]=new char[MZ.x1];	//��C 
					MZ.visited[i]=new char[MZ.x1];	//��C 
					MZ.merge_m[i]=new char[MZ.x1];	//��C 
				}
				
				for (int i = 0; i <MZ.y1; i++){
					for ( int j = 0; j < MZ.x1; j++){
						infile >> MZ.maze[i][j];	//Ū�J�g�c 
						MZ.visited[i][j] = MZ.maze[i][j];	//����visited �M merge_m��input�@�� 
						MZ.merge_m[i][j] = MZ.maze[i][j];
					}	
				}
				infile.close();
				MZ.step(0, 0);	//���Ȥ@ �q(0,0)�}�l 
				MZ.print_visited();	//�L�X�Ҧ��g�L���| 
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
				MZ.maze = new char*[MZ.y1];	//���� 
				MZ.visited = new char*[MZ.y1];	//���� 
				MZ.merge_m = new char*[MZ.y1];	//���� 
				
				for(int i=0;i<MZ.y1;i++){
					MZ.maze[i]=new char[MZ.x1];	//��C 
					MZ.visited[i]=new char[MZ.x1];	//��C 
					MZ.merge_m[i]=new char[MZ.x1];	//��C 
				}
				
				for (int i = 0; i <MZ.y1; i++){
					for ( int j = 0; j < MZ.x1; j++){
						infile >> MZ.maze[i][j];	//Ū�J�g�c
						MZ.visited[i][j] = MZ.maze[i][j];	//����visited �M merge_m��input�@�� 
						MZ.merge_m[i][j] = MZ.maze[i][j];
					}	
				}
				infile.close();
				
				MZ.Multi_G(0, 0);	//���ȤG �q(0,0)�}�l
				MZ.print_visited();	//�L�X�Ҧ��g�L���| 
				
				if(MZ.G_time == MZ.goals){	//����G�ƶq==N 
					cout << endl;
		    		MZ.Print_Merge();	//�L�X�Ҧ����XG�ߤ@���| 
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

