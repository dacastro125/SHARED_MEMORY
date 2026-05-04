#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *funcion_maneja_hilos(void *);

typedef struct Nodo{
    int val;
}Nodo;

int main(){
    pthread_t pidhilo[2];
    int i;

    for(i = 0;i<2;i++){
        Nodo nodo;
        nodo.val = i;
        pthread_create(&pidhilo[i],NULL,funcion_maneja_hilos,(void*)&nodo);
    }
    printf("Hilos principal (idthread -> [%lu])\n",pthread_self());
    for(i = 0;i<2;i++){
        pthread_join(pidhilo[i],NULL);
    }
    return 0;
}

void * funcion_maneja_hilos(void *param){
    printf("Hilo param->%d (idthread ->[%lu])\n",((struct Nodo *)param)->val,pthread_self());
    pthread_exit(0);
}
