#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* funcion_maneja_hilo(void*);

int main(int argc, char const *argv[])
{
	int i;
	pthread_t pidhilo[3];
	
	//donde se aloja esa direccion (void*)"hola"??
	for (i = 0; i < 10; i++){
		//usleep(10000);//pierde paraleleismo					//la cuarta direccion sea solo para un hilo
		pthread_create(&pidhilo[i], NULL, funcion_maneja_hilo, (void*)&i);// no se le manda la cadena hola, 
	}
	printf("Hilo principal(idthread -> [%ld])\n", (long int) pthread_self());
	for (i = 0; i < 10; i++){
		pthread_join(pidhilo[i], NULL);
	}
	return 0;
}

void* funcion_maneja_hilo(void *param){// argumento void (sin tipo) para que pueda ser enviado cualquier tipo
	printf("hilo param %d,(idthread ->[%ld])\n",*(int *)param, (long int) pthread_self());
	pthread_exit(0);
}