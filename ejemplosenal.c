#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

void manejador(int s){
	//printf("Señal %d %d\n",s,getppid() );
}

int main(int argc, char const *argv[]){
	pid_t pidhijo;
	signal(SIGUSR1, manejador);
	pidhijo = fork();
	if (pidhijo==0)
	{
		printf("Yo soy el hijo \n");
		kill(getppid(),SIGUSR1);
	}else{
		pause();
		printf("Soy el padre %d\n", getppid());
		wait(NULL);
	}

	return 0;
}