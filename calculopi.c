#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void * funhilos(void *);
int ntrap , ctrap=0;
double delta, pi;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;




int main(){
	int nhilos=0, i=0;

	pthread_t  * pidhilos=NULL;

	printf("Numero de trapecios: ");
	scanf("%d", &ntrap);

	printf("Numero de hilos: ");
	scanf("%d", &nhilos);
	delta = 1/(double)ntrap;

	pidhilos = (pthread_t *)calloc(nhilos, sizeof(pthread_t));

	for (i = 0; i < nhilos; i++){
		
		pthread_create(&pidhilos[i], NULL, funhilos, NULL);	
		
	}

	for (i = 0; i < nhilos; i++){
		pthread_join(pidhilos[i], NULL);
	}
	
	pi *= delta;
	printf("PI: %.10f\n", pi);

	
	return 0;
}

void * funhilos(void * arg){
	double x,y;
	int trapl;

	while(ctrap<=ntrap){
		pthread_mutex_unlock(&mutex);
		ctrap++;
		trapl=ctrap;
		pthread_mutex_lock(&mutex);
		
		if(trapl<ntrap){
		
		//printf("%d--%lu\n", ctrap, pthread_self());

		x = (double)(ctrap-0.5)*delta;
		y = 4.0/(1.0+(x*x));
		
	
		pthread_mutex_unlock(&mutex);
		pi += y;	
		pthread_mutex_lock(&mutex);
		
		}
		
		
	}

	pthread_exit(0);
}
