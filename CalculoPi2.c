#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *Pi(void *);
void *imprimir(void *);
int N, nhilos;
double suma=0.0, j=1;
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]){
 
 int i;
 N= atoi(argv[1]);
 nhilos= atoi(argv[2]); 	

 pthread_t hilos[nhilos];
 pthread_t hilo;
 for(i=0;i<nhilos;i++){
	pthread_create(&hilos[i], NULL, Pi, NULL);
 }

 pthread_create(&hilo, NULL, imprimir, (void *)hilos);
 pthread_join(hilo, NULL);
 
 return 0;
}

void *Pi(void *arg){
 int *i=(int *)malloc(sizeof(int)), rectan=0;
 double x, y;
 double delta= 1/(double)N;
 for(;;){
         pthread_mutex_lock(&m);
	 if(j<=N){
                x=(double)(j-0.5)*delta;
	        y=4.0/(1.0+(x*x));
		suma+=y;
		j++;
		rectan++;
		pthread_mutex_unlock(&m);
        }else{
		pthread_mutex_unlock(&m);
		break;
	}
        

 }	
 *i=rectan;	 
 pthread_exit(i);
}

void *imprimir(void *arg){

	pthread_t *hilos= (pthread_t *)arg;
	
	int *i, k;
        for(k=0;k<nhilos;k++){
 		pthread_join(hilos[k], (void **)&i);
		printf("Hilo%d[%u] N:%d\n", k+1, (unsigned int)pthread_self(), *i);
	}
	double	pi=suma*(1/(double)N);	
        printf("pi: %.20f\n", pi);
        pthread_exit(NULL);
}


