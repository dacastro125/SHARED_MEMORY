#include <stdio.h>
#include <signal.h>

void manejador(){}

int main(){
	int bro[4];;
	signal(SIGUSR1,manejador);
	bro[0]=fork();

	if(bro[0] == 0){
		pause();		
		printf("hijo1 \n");
		kill(bro[1],SIGUSR1);
	}
	bro[1]=fork();
	if(bro[1] == 0){
		pause();		
		printf("hijo2 \n");
		kill(bro[1],SIGUSR1);
	}

	else{
		printf("padre \n");
		usleep(1000);
		kill(bro[0],SIGUSR1);
		pause();
		printf("padre \n");
	}
}
