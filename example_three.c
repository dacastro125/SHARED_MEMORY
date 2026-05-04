#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv){

	pid_t pid, pid2;
	int x;
	
	for(x=1;x<=3;x++){
		
		pid=fork();
		if(pid!=0){
			printf("soy el proceso %d\n", getpid());
			sleep(2);
		}else{
			printf("soy el hijo %d, mi padre es %d\n", getpid(),getppid());
			sleep(2);
			exit(0);
		}
		
	}
	
	
	
	

return 0;
}
