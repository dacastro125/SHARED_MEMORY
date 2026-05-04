#include <signal.h>
#include <stdio.h>

void manejador(){}
int main(){
	int c, n,i;
	signal(SIGUSR1, manejador);
	printf("Digite n=");
	scanf("%d",&n);
	if( !(c=fork()) ){
		for(i=0; i<n; i++){
			pause();
			printf("Hijo\n");
			kill(getppid(), SIGUSR1);
		}
	}
	else{
		for(i=0; i<n; i++){
			sleep(1);
			printf("Padre \n");
			kill(c, SIGUSR1);
			pause();
		}
		wait(NULL);
	}
	return 0;
}
