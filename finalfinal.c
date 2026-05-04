#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_maneja_hilo(void *);
typedef unsigned long int tipo_hilo;

struct nodo{
	int val;
};

int main(){
	pthread_t pidhilo[20];
	struct nodo Nodo;
	
	int n,i; 
printf("digite la cantida de hilos\n");
     scanf("%d",&n);

printf("hilo principal (idthread -> [%ld])\n",  pthread_self());

	for(i=0; i<n; i++){

		Nodo.val=i;
		pthread_create(&pidhilo[i], NULL, funcion_maneja_hilo, (void*)&Nodo.val);
		
}
		
		printf("hilo principal (idthread -> [%ld])\n",  pthread_self());		

		for(i=0; i<n; i++){
		pthread_join(pidhilo[i], NULL);		

		}
		printf("hilo principal (idthread -> [%ld])\n",  pthread_self());		
return(0);
}

void* funcion_maneja_hilo(void *param){
	printf("hilo (idthread -> [%ld])\n", ((struct nodo *) param) ->val, pthread_self());


	pthread_exit(0);
}
