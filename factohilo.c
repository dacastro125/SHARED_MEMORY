#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


struct nodo {
    int val;
};


void * factorial(void * param) {
    int i, fact = 1;

    for (i = 2; i <= ((struct nodo *) param)->val; i++) {
        fact = fact*i;
    }

    printf("fact: %d\n", fact);
    return (void *) fact;
}

main(int argc, char *argv[]) {

    pthread_t hilo_id[argc];

    int i, val=0;
    struct nodo *Nodo;

    for (i = 1; i < argc; i++) {
        Nodo = (struct nodo *)malloc(sizeof(struct nodo));
        Nodo->val = atoi(argv[i]); //atoi:convierte cadena string en  entero
        pthread_create(&hilo_id[i], NULL, &factorial, (void *) Nodo);
    }

    int suma=0;

    for (i = 1; i < argc; i++) {
        pthread_join(hilo_id[i], (void *) &val);
        suma = suma + val;
    }

    printf("SUMA = %d\n", suma);

}
