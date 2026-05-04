#include <signal.h>
#include <stdio.h>

void manejador(int sig){
	printf("Señal: %d", sig);
}

int main(){
	(void)signal(SIGINT, manejador);
	while(1){
		printf("hola\n");
		sleep(1);	
	}
}
