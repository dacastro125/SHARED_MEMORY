#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void *funcion_maneja_hilos(void *);

typedef struct Nodo{
    int val;
}Nodo;

int main(){
    pthread_t pidhilo[2];
    Nodo *nodo;
    Nodo *nodos[2];
    int i;

    for(i = 0;i<2;i++){
        nodos[i] = (Nodo *)malloc(sizeof(Nodo));
        nodos[i]->val = i;
        pthread_create(&pidhilo[i],NULL,funcion_maneja_hilos,nodos[i]);
    }
    printf("Hilos principal (idthread -> [%lu])\n",pthread_self());
    for(i = 0;i<2;i++){
        pthread_join(pidhilo[i],NULL);
        free(nodos[i]);
    }

    return 0;
}

void * funcion_maneja_hilos(void *param){
    printf("Hilo param->%d (idthread ->[%lu])\n",((struct Nodo *)param)->val,pthread_self());
    pthread_exit(0);
}
