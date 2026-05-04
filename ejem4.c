#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


void* funcion_maneja_hilo(void *);
typedef unsigned long int tipo_hilo;

struct nodo{
 int val;

};

int bandera = -1;
int i,n,m;

int main(){
	printf("ingrese el tamaño\n");
	scanf("%d",&n);
	
        printf("ingrese el numero de veces que se imprime\n");
        scanf("%d",&m);
	
        pthread_t pidhilo[n];
        struct nodo *Nodo;
        

        for(i=0; i<n; i++){
        Nodo = (struct nodo *)malloc(sizeof(struct nodo));
        Nodo->val = i;
        pthread_create(&pidhilo[i],NULL,funcion_maneja_hilo, (void*)Nodo);
        }
	printf("hilo principal (idthread -> [%lu])\n",pthread_self());
        
        bandera++;
        for(i=0; i<n; i++){ 
          pthread_join(pidhilo[i],NULL);
        }
	return 0;
}

void* funcion_maneja_hilo(void *param){	
  for(;;){
   if(bandera == ((struct nodo *)param)->val){
    printf("hilo param->%d (idthread -> [%lu])\n",((struct nodo *)param)->val,pthread_self());
    bandera++;
   }
  }
  pthread_exit(0);      
}
