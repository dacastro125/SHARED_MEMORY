#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *funcion_maneja_hilo(void *);
typedef unsigned long int tipo_hilo;

struct nodo
{
    int val;
};

int turno=-1;

int main()
{
    pthread_t pidhilo[3];
    struct nodo *Nodo;
    int i;
    
     
    for(i=0;i<3;i++){
    Nodo=(struct nodo*)malloc(sizeof( struct nodo));
    Nodo->val=i;
    pthread_create(&pidhilo[i], NULL, funcion_maneja_hilo, (void*)Nodo);   
	}
    printf("Hilo principal con id-> [%lu])\n", pthread_self());
    turno=0;
    

	for(i=0;i<2;i++){
         
	pthread_join(pidhilo[i],NULL);
    }
    
    return 0;
}



void *funcion_maneja_hilo(void *param)
{
        while(((struct nodo*) param)->val!=turno){

        }
	printf("Soy el Hilo %d con id -> [%lu])\n",turno, pthread_self());
        turno=turno+1;
  
	pthread_exit(0);
}
