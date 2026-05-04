#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* funcion_maneja_hilos(void *);

int main(){
    pthread_t pidhilo;
    pthread_create(&pidhilo,NULL,funcion_maneja_hilos,NULL);
    printf("Hilo principal (idThread -> [%ld])\n",(long int)pthread_self());
    pthread_join(pidhilo,NULL);
    return 0;
}

void* funcion_maneja_hilos(void *param){
    printf("Hilo (idThread ->[%ld])\n",(long int)pthread_self());
    pthread_exit(0);
}
