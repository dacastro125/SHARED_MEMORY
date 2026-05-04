#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * funcion_hilo(void *);

int main(){

	pthread_t hiloid;

	pthread_create(&hiloid, NULL, funcion_hilo, NULL);
	printf("hilo [%u], ppal \n", (unsigned int)pthread_self());
	pthread_join(hiloid, NULL);

return 0;

}

void * funcion_hilo(void * arg){
	printf("Hilo [%u] nuevo \n", (unsigned int)pthread_self() );
	pthread_exit(NULL);
}
