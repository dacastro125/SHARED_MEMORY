#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <signal.h>

#define MAX_TAM 50

typedef struct Datos{
    int n;
    double media;
    int vector[MAX_TAM];
    int mayor;
    int repetido;
}Datos;

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}


void promedio(Datos *datos){

    int i = 0,suma = 0;
    for(;i<datos->n;i++){
        suma += datos->vector[i];
    }
    datos->media = suma/(datos->n);
    printf("[%d] El promedio es: %lf\n",getpid(),datos->media);

}

void numeroMayor(Datos *datos){

    int i = 0;
    for(;i<datos->n;i++){
        if(datos->mayor < datos->vector[i]){
            datos->mayor = datos->vector[i];
        }
    }
    printf("[%d] El mayor es: %d\n",getpid(),datos->mayor);
}

void repetido(Datos *datos){
    printf("[%d] Haz la funcion del repetido.\n",getpid());
}

void manejador(){
}

int main(int argc, char *argv[]) {
    int vector[] = {1,2,3,4,5,6,7,8};
    int n = 8;
    int chpid;

    signal(SIGUSR1,manejador);

    int shmid = shmget(IPC_PRIVATE, sizeof(Datos), IPC_CREAT|0666);
    if (shmid == -1) {
        error("Error creando shmget");
    }

    void *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (void*)-1) {
        error("Error adjuntando segmento shmat");
    }

    Datos *datos = (Datos*)shm_ptr;
    datos->n = n;
    datos->mayor = 0;
    datos->media = 0.0;
    datos->repetido = 0;

    int j = 0;
    for(j;j<datos->n;++j){
        datos->vector[j] = vector[j];
    }

    int hijos[3], i = 0;;
    for(;i<3;i++){
        if(!(hijos[i]=fork())){
            break;
        }
    }

    if(i == 3){
        usleep(1000);
        printf("[%d] Soy el padre\n", getpid());
        kill(hijos[i-1],SIGUSR1);
        pause();

        printf("[%d] El mayor es: %d\n",getpid(),datos->mayor);
        printf("[%d] El promedio es: %lf\n",getpid(),datos->media);
        printf("[%d] El mas repetido es: %d\n",getpid(),datos->repetido);
        shmctl(shmid, IPC_RMID, NULL);
    }
    if(i == 2){
        pause();
        repetido(datos);
        kill(hijos[i-1],SIGUSR1);
        shmdt(shm_ptr);
    }
    if(i == 1){
        pause();
        numeroMayor(datos);
        kill(hijos[i-1],SIGUSR1);
        shmdt(shm_ptr);
    }
    if(i == 0){
        pause();
        promedio(datos);
        kill(getppid(),SIGUSR1);
        shmdt(shm_ptr);
    }
    return 0;
}
