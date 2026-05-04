#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv){

	pid_t pid;
	pid = fork();
	
	
	if(pid==-1){
	printf("No se pudo crear el proceso\n");
	}else{
		if(pid==0){
			printf(" Soy el proceso %d, hijo de %d\n", getpid(), getppid());
		}else{
			printf(" Soy el proceso %d \n", getpid());
		}
	}
	
	//printf("Hola\n");

return 0;
}
