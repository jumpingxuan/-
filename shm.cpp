/*
    source: https://www.ibm.com/developerworks/aix/library/au-spunix_sharedmemory/
    compile: gcc -o shm shm.c -lrt
    exec: ./shm
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

void error_and_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int r;

  const char *memname = "sample";
  const size_t region_size = sysconf(_SC_PAGE_SIZE);

  int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
  if (fd == -1)
    error_and_die("shm_open");

  r = ftruncate(fd, region_size);
  if (r != 0)
    error_and_die("ftruncate");

  int *ptr =(int*) mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED)
    error_and_die("mmap");
  close(fd);
	
  pid_t pid = fork();

  if (pid == 0) {
  
	int store[2][2];
	int p=atoi(argv[2]);

	cout<<"["<<getpid()<<" Child]: Random Seed "<< p <<endl;
	ptr[0]=1;
	//cout<<ptr[0]<<endl;
	
	srand(p);
        store[0][0]=rand()%4;
        store[0][1]=rand()%4;
            
        while(true)
        {
           //get another part of boad
           int dx=store[0][0]+rand()%2;               
           int dy=store[0][1]+rand()%2;
           store[1][0]=dx;
           store[1][1]=dy;
           if(dx>=0&&dx<4&&dy>=0&&dy<4&&(dx!=store[0][0]||dy!=store[0][1])) break;
	 }//while end

	while(true)
	{
		if(ptr[0]==0){
		cout<<"["<<getpid()<<" Child]: The gunboat: ("<<store[0][0]<<","<<store[0][1]<<")"<<"("<<store[1][0]<<","<<store[1][1]<<")"<<endl;
		ptr[0]=1;
		break;
		}//if end
	}//while end
	
	int hit=0;
	int counter=0;
	int x=0;

	while(true)
	{
		
		if(ptr[0]==0){
			//counter++;		
			//check position be hit
			if((ptr[1]==store[0][0]&&ptr[2]==store[0][1]) || (ptr[1]==store[1][0]&&ptr[2]==store[1][1])){
				hit++;
				if(ptr[1]==store[0][0]&&ptr[2]==store[0][1]){
					store[0][0]=-1;
					store[0][1]=-1;
				}//if end
				else{
					store[1][0]=-1;
					store[1][1]=-1;
				}//else end

				cout<<"["<<getpid()<<" Child]: hit"<<endl;
				
				x=1;
				//when parent wins
				

			}//if end
			
			//child still live
			else{
				cout<<"["<<getpid()<<" Child]: missed"<<endl;
				x=1;
			}

			if(hit==2){//parent wins
				
				
				//should print child's pid
				cout<<"["<<getpid()<<" Child]: hit and sink"<<endl;
				ptr[0]=-1; //parent should cout the result
				break;
				//return to parent
					
			}//if end
			
			if(x==1){

			counter++; // how many time did child hit parent
			int z=rand()%4;
			int y=rand()%4;
			ptr[1]=z;
			ptr[2]=y;
			cout<<"["<<getpid()<<" Child]: bombing ("<<ptr[1]<<","<<ptr[2]<<")"<<endl;
			ptr[0]=2;//turn to parent
			x=0;

			}//if(x==1) end
			

		}//if end
		if(ptr[0]==-1)//child wins
		{
			//cout<<getpid()<<" wins with the "<<counter<<" bombs"<<endl;
ptr[6]=0;
			ptr[6]=counter;
			//cout<<"CC"<<counter<<" "<<ptr[3]<<endl;

			ptr[4]=1;
			break;
		}
			
	}//while end
ptr[3]=1;
//cout<<"hiC"<<endl;
    exit(0);
  }
  else{
	int store[2][2];
	int p=atoi(argv[1]);
	
	cout<<"["<<getpid()<<" Parent]: Random Seed "<< p <<endl;
	
	srand(p);
        store[0][0]=rand()%4;
        store[0][1]=rand()%4;
            
        while(true)
        {
           //get another part of boad
           int dx=store[0][0]+rand()%2;               
           int dy=store[0][1]+rand()%2;
           store[1][0]=dx;
           store[1][1]=dy;
           if(dx>=0&&dx<4&&dy>=0&&dy<4&&(dx!=store[0][0]||dy!=store[0][1])) break;
	 }//while end
	
	while(true)
	{
		if(ptr[0]==1){
		cout<<"["<<getpid()<<" Parent]: The gunboat: ("<<store[0][0]<<","<<store[0][1]<<")"<<"("<<store[1][0]<<","<<store[1][1]<<")"<<endl;
		ptr[0]=0;
		break;
		}//if end
	}//while end
	int counter=0;
	while(true)
	{
		if(ptr[0]==1){
			counter++;
			int x=rand()%4;
			int y=rand()%4;
			ptr[1]=x;
			ptr[2]=y;
			cout<<"["<<getpid()<<" Parent]: bombing ("<<ptr[1]<<","<<ptr[2]<<")"<<endl;
			ptr[0]=0;
			break;
		}//if end
	}//while end

	int hit=0;
	int x=0;
	int i=0;
	while(true)
	{
		
		if(ptr[0]==2){
			//counter++;		
			//check position be hit
			if((ptr[1]==store[0][0]&&ptr[2]==store[0][1]) || (ptr[1]==store[1][0]&&ptr[2]==store[1][1])){
				hit++;
				if(ptr[1]==store[0][0]&&ptr[2]==store[0][1]){
					store[0][0]=-1;
					store[0][1]=-1;
				}//if end
				else{
					store[1][0]=-1;
					store[1][1]=-1;
				}//else end

				cout<<"["<<getpid()<<" Parent]: hit"<<endl;
				x=1;

				//when parent wins
				

			}//if end
			
			//child still live
			else{
				cout<<"["<<getpid()<<" Parent]: missed"<<endl;
				x=1;
			}

			if(hit==2){
				//child wins
				cout<<"["<<getpid()<<" Parent]: hit and sink"<<endl;
				//i=1;
				//if(i==1){
				//int pidF=(int)getpid();
				//ptr[5]=pidF;
				
				
				ptr[0]=-1; //inform child to cout
				while(true){
					//int j=ptr[6];
					if(ptr[4]==1){

			
						cout<<"["<<getpid()<<" Parent]: "<<pid<<" wins with "<<ptr[6]<<" bombs"<<endl;
						break;
					}
				}
				/*while(true)
				{
					cout<<"1"<<endl;
					if(ptr[3]==1){
						break;
					}
				}*/
				

				break;
				
			}//if end
				
			if(x==1){

			counter++; // how many time did child hit parent
			int z=rand()%4;
			int y=rand()%4;
			ptr[1]=z;
			ptr[2]=y;
			cout<<"["<<getpid()<<" Parent]: bombing ("<<ptr[1]<<","<<ptr[2]<<")"<<endl;
			ptr[0]=0;//turn to c
			x=0;
			}//if(x==1) end
			
			

		}//if end
		
		if(ptr[0]==-1)//parent wins
		{
			cout<<"["<<getpid()<<" Parent]: "<<getpid()<<" wins with "<<counter<<" bombs"<<endl;
			break;
		}//if end
 
	}//while end

   
    int status;
 waitpid(pid, &status, 0);
 }

  r = munmap(ptr, region_size);
  if (r != 0)
    error_and_die("munmap");

  r = shm_unlink(memname);
  if (r != 0)
    error_and_die("shm_unlink");

  return 0;
}



