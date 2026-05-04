#include <signal.h>
#include <stdio.h>

void manejador(int sig){
	if(sig == 2) return;
	system("date");
}

int main(){
	int sig;
	(void)signal(SIGINT, manejador);
	(void)signal(SIGALRM, manejador);

	printf("ingrese la señal: ");
	scanf("%d", &sig);

	printf("Generando la señal %d\n", sig);
	raise(sig);
	printf("Señal generada.\n");
	
}
