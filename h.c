
//para compilar =   gcc -pthread h.c -o h 


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>



void* funcion_maneja_hilo(void*);


int main(){

	pthread_t pidhilo;
	pthread_create(&pidhilo,NULL, funcion_maneja_hilo,NULL);
	printf("hilo principal(idthread-> [%d])\n", (long int) pthread_self());
	pthread_join(pidhilo, NULL);

return 0;

}

void* funcion_maneja_hilo(void *param){
printf("hilo (idthread->[%d])\n", (long int) pthread_self());

pthread_exit(0);

}

