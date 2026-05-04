#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){

	int pid1, pid2;
	
	pid1=fork();
	if(pid1!=0){
		printf(" Soy el proceso padre %d\n", getpid());
	}else{
		pid2 = fork();
		if(pid2!=0){
			printf(" Soy el proceso %d, mi padre es %d\n", getpid(), getppid());
		}
		if(pid2==0){
			printf(" Soy el proceso %d, mi padre es %d\n", getpid(), getppid());
		}		
	}

return 0;
}
