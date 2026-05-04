#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void manejador(int s){
}

int main(){
	int *hijos, x, n;
	signal(SIGUSR1, manejador);
	printf("Digite el número de procesos a crear: ");
	scanf("%d", &n);
	hijos = malloc(sizeof(int)*n);
	for(x = 0; x < n; x++){
		if(!(hijos[x]=fork())){
			pause();
			printf("Hijo %d: %d\n", (x + 1), getpid());
			if(x > 0){
				kill(hijos[x-1], SIGUSR1);
			}else{
				kill(getppid(), SIGUSR1);
			}
			break;
		}
	}
	if(x == n){
		usleep(100000);
		kill(hijos[x-1], SIGUSR1);
		pause();
		printf("Padre: %d\n", getpid());
		//Crear un wait por cada hijo creado
		for(x = 0; x < n; x++){
			wait(NULL);
		}
	}
	return 0;
}