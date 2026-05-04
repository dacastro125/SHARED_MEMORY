#include <stdio.h>
#include <signal.h>
void manejador(int s){
	printf("Señal %d\n",s);		
			}

	int main(){
		signal(SIGINT, manejador);
		for(;;){
		sleep(1);
		}
	return 0;
		}
