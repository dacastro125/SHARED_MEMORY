#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

static int global = 0;

void * funcionhilo(void *argumento){
	int iter = *(int *) argumento;
	int i, local;

	for (i = 0; i < iter; i++){
		local = global;
		local ++;
		global = local;
	}

	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t hilos[2];
	int maxiter, i, r;

	maxiter = 100000000;

	for (i = 0; i < 2; i++){
		r = pthread_create(&hilos[i], NULL, funcionhilo, (void*)&maxiter);
		if (r != 0){
			printf("Error al crear hilos\n");
			exit(EXIT_FAILURE);
		}
	}

	for (i = 0; i < 2; i++){
		r = pthread_join(hilos[i], NULL);
		if (r != 0){
			printf("Error al esperar hilos\n");
			exit(EXIT_FAILURE);
		}
	}

	printf("variable global = %d\n", global);
	return 0;
}