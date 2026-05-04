#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}

void promedio(int vector[],int n,double *media){

    int i = 0,suma = 0;
    for(;i<n;i++){
        suma += vector[i];
    }
    *media = suma/(n);
    printf("[%d] El promedio es: %lf\n",getpid(),*media);

}

void numeroMayor(int vector[],int n,int *mayor){

    int i = 0;
    for(;i<n;i++){
        if(*mayor < vector[i]){
            *mayor = vector[i];
        }
    }
    printf("[%d] El mayor es: %d\n",getpid(),*mayor);
}

int main(int argc, char *argv[]) {
    int vector[] = {1,2,3,4,5,6,7,8};
    int tam = 8;
    int chpid;

    int shmid = shmget(IPC_PRIVATE, sizeof(int)*(tam+7)+sizeof(double), IPC_CREAT|0666);
    if (shmid == -1) {
        error("Error creando shmget");
    }

    void *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (void*)-1) {
        error("Error adjuntando segmento shmat");
    }

    double* media = (double*)shm_ptr;
    int* banderaP = (int*)(media+1);
    int* bandera1 = banderaP + 1;
    int* bandera2 = bandera1 + 1;
    int* bandera3 = bandera2 + 1;
    int* mayor = bandera3 + 1;
    int* masRepetido = mayor + 1;
    int* n = masRepetido + 1;
    int* vectorAPasar = n + 1;
    *bandera1 = 1;
    *bandera2 = 1;
    *bandera3 = 1;

    if (fork()){
        if(fork()){
            if(fork()){
                printf("[%d] Soy el padre\n", getpid());
                *media = 0;
                *mayor = 0;
                *masRepetido = 0;
                *n = tam;
                *banderaP = 1;

                int i = 0;
                for(;i<*n;i++){
                    vectorAPasar[i] = vector[i];
                }
                *banderaP = 1;

                while(*bandera1 == 1){}
                while(*bandera2 == 1){}
                while(*bandera3 == 1){}

                shmctl(shmid, IPC_RMID, NULL);

            }else{
                while(*bandera2 != 1){
                }
                *bandera3 = 0;
                shmdt(shm_ptr);
            }
        }else{
            while(*bandera1 =! 1){
            }
            numeroMayor(vectorAPasar,*n,mayor);
            *bandera2 = 0;
            shmdt(shm_ptr);
        }
    } else{
        while(*banderaP != 1){
        }

        promedio(vectorAPasar,*n,media);
        *bandera1 = 0;
        shmdt(shm_ptr);
    }
    return 0;

}
