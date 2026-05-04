#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void manejador(int sig){}

int main(){
	int c;
	(void)signal(SIGUSR1, manejador);
	if((c=fork()) == 0){
		pause();
		printf("Hijo\n");
	}else{
		usleep(10000);
		printf("Padre \n");
		kill(c, SIGUSR1);
		wait(NULL);
	}

	return 0;
}
