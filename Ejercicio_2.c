#include <stdio.h>
#include <signal.h>

void manejador(int s){
}

int main(){
	int c, n, i;
	signal(SIGUSR1, manejador);
	printf("Digite n = ");
	scanf("%d", &n);
	if(!(c=fork())){
		for(i = 0; i < n; i++){
			pause();
			printf("Hijo %d\n", getpid());
			kill(getppid(), SIGUSR1);
		}
	}else{
		for(i = 0; i < n; i++){
			printf("Padre %d\n", getppid());
			usleep(100000);
			kill(c, SIGUSR1);
			pause();
		}
		wait(NULL);
	}
	return 0;
}