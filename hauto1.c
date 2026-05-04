#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* func_hilo(void*);

int main(int argc, char const *argv[])
{
	int i = 0;
	pthread_t pidhilo;

	pthread_create(&pidhilo, NULL, func_hilo, NULL);// no se le manda la cadena hola, 
	printf("Hilo principal %u\n", (unsigned int)pthread_self());
	pthread_join(pidhilo, NULL);

	return 0;
}

void* func_hilo(void* param){
	printf("Hilo  [%u]\n", (unsigned int) pthread_self());
}
