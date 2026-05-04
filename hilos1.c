#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* funcion_maneja_hilo(void*);

int main(int argc, char const *argv[])
{
	pthread_t pidhilo;
	pthread_create(&pidhilo, NULL, funcion_maneja_hilo, NULL);// direccion donde almacenar el id del hilo, modificacion de atributos
								// funcion que ejecuta el hilo, argumento que recibe la funcion del hilo
	printf("Hilo principal(idthread -> [%ld])\n", (long int) pthread_self());
	pthread_join(pidhilo, NULL);//espera hasta la terminacion de un hilo específico
	// hilos esperables y no esperables, no esperables o desacoplados devuelven todos los recursos al terminar
	// los esperables no sueltan los recursos, por defecto son esperables
	// es bloqueante y espera hasta que el otro hilo  termine
	return 0;
}

void* funcion_maneja_hilo(void *param){// argumento void (sin tipo) para que pueda ser enviado cualquier tipo
	printf("hilo (idthread ->[%ld])\n", (long int) pthread_self());
	pthread_exit(0);
}