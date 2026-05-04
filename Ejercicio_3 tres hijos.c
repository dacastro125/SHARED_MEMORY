#include <stdio.h>
#include <signal.h>

void manejador(int s){
}

int main(){
	int hijos[3];
	signal(SIGUSR1, manejador);
	if(!(hijos[0]=fork())){
		pause();
		printf("Hijo 1: %d\n", getpid());
		kill(getppid(), SIGUSR1);
	}else{
		if(!(hijos[1]=fork())){
			pause();
			printf("Hijo 2: %d\n", getpid());
			kill(hijos[0], SIGUSR1);
		}else{
			if(!(hijos[2]=fork())){
				pause();
				printf("Hijo 3: %d\n", getpid());
				kill(hijos[1], SIGUSR1);
			}else{
				usleep(100000);
				kill(hijos[2], SIGUSR1);
				pause();
				printf("Padre: %d\n", getpid());
				wait(NULL);
				wait(NULL);
				wait(NULL);
			}
		}
	}
	return 0;
}