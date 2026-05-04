#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(int argc,char *argv[]){

	pid_t pid, pid2, pid3;
	
	pid=fork();
	if (pid!=0){
		printf(" Soy el proceso padre %d \n", getpid());
	}else{
		pid2=fork();
		if(pid2!=0){
			printf(" Soy el proceso %d, mi padre es %d\n", getpid(), getppid());
			pid3=fork();			
			if(pid3==0){
				printf(" Soy el proceso %d, mi padre es %d\n", getpid(), getppid());
			}
			wait(NULL);
			wait(NULL);
		}
		else{
			printf(" Soy el proceso %d, mi padre es %d\n", getpid(), getppid());
		     }
	
	}
	
	wait(NULL);
return 0;
}
