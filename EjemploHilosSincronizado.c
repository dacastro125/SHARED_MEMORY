

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t condicion = PTHREAD_COND_INITIALIZER;

void* funcion_maneja_hilo(void *);
void esperarHilos(int);
typedef unsigned long int tipo_hilo;

struct nodo{
 int val
};


int main(){
  
  struct nodo *Nodo;
  int i,n;


   printf("Ingrese la cantidas de Hilos: ");
   scanf("%d",&n);
   pthread_t  pidhilo[n];

 for(i=0;i<n;i++){
     Nodo = (struct nodo *)malloc(sizeof(struct nodo));
     pthread_mutex_lock(&mutex);
     Nodo->val = i;
     pthread_mutex_unlock(&mutex);

     pthread_create(&pidhilo[i],NULL,funcion_maneja_hilo, (void*) Nodo);
 }
 printf("Master Thread (idthread -> [%lu])\n",pthread_self());

 for(i=0;i<n;i++){
   pthread_join(pidhilo[i],NULL);
 }
 pthread_mutex_destroy(&mutex);
 return 0;
}

void* funcion_maneja_hilo(void *param){

  printf("Hilo param ->%d (idthread -> [%ld])\n",((struct nodo *)param)->val,pthread_self());
  free(param);
  esperarHilos(1000);
  pthread_exit(0);
}
void esperarHilos(int n){
	usleep(n);

}
	
