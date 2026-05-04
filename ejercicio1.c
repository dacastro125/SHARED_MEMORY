#include <stdio.h>
#include <signal.h>

void manejador(){}

int main(){
	int hijo;
	signal(SIGUSR1,manejador);
	hijo = fork();

	if(hijo == 0){
		pause();
		printf("hijo \n");
		kill(getppid(),SIGUSR1);
	}
	else{
		printf("padre \n");
		usleep(1000);
		kill(hijo,SIGUSR1);
		pause();
		printf("padre \n");
	}
}
