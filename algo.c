#include "stdio.h"
#include "unistd.h"
#include "pthread.h"
#include "stdlib.h"

void* funcion_manejador_hilo(void *);
pthread_t hilo[4];
int turnoglobal = 0;
int N=2;
int main(){    
    int i,j, *pi;
	
    for(i = 0; i < 4; i++){	
    	pi = (int*)malloc(sizeof(int));
    	switch(i){
    		case 0: *pi=1; break;
    		case 1: *pi=2; break;
    		case 2: *pi=4; break;
    		case 3: *pi=5; break;
    	}
    	pthread_create(&hilo[i], NULL, funcion_manejador_hilo, (void*) (pi));
    }

printf("Hilo principal (idthread -> [%ld])\n", (long) pthread_self());
turnoglobal++;
for(j=0;j<N;j++){
	while(turnoglobal!=3 &&  turnoglobal!=6 );
	printf("Hilo principal (idthread -> [%ld])\n", (long) pthread_self());
turnoglobal++;}/*
while(turnoglobal!=3);
printf("Hilo principal (idthread -> [%ld])\n", (long) pthread_self());
turnoglobal++;
while(turnoglobal!=6);
printf("Hilo principal (idthread -> [%ld])\n", (long) pthread_self());
//}*/
    for(i = 0; i < 4; i++)	
		pthread_join(hilo[i], NULL);
   
    return 0;
}

void* funcion_manejador_hilo(void *param){
	int turno = *(int*) param, j;
	while(turno!=turnoglobal);
		printf("Hilo %d (idthread -> [%ld])\n", turno, (long) pthread_self());
		turnoglobal++;	
   pthread_exit(0);
}
