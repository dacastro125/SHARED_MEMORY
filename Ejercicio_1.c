#include <stdio.h>
#include <signal.h>

void manejador(int s){
}

int main(){
	int c;
	signal(SIGUSR1, manejador);
	//este if asigna las tareas a realizar para cada proceso padre e hijo
	if(!(c=fork())){
		//bloquea el proceso del hijo hasta recibir una señal cualquiera
		pause();
		printf("Hijo despertado\n");
	}else{
		printf("Padre: enviar señal a hijo\n");
		//al comentariar el usleep se envia la señal de despertar al proceso hijo antes de que este se bloqueara,
		//por ende, el proceso hijo se bloqueara despues y quedara el proceso hijo en ejecucion continua.
		usleep(100000);
		//envia una señal al hijo
		kill(c, SIGUSR1);
		//retorna el estado en el cual termina un proceso "return 0 para una finalizacion normal"
		//recibe como parametro un *int para guardar el estado de terminacion de un proceso
		wait(NULL);
	}
	return 0;
}
