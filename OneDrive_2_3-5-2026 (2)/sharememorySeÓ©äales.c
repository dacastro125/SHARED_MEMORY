#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_TAM 10

typedef struct Datos
{
    int n;
    float media;
    int vector[MAX_TAM];
    int mayor;
    int repetido;
}Datos;

void promedio(Datos *datos){
    int i=0;
    float suma = 0;
    for (i = 0; i < datos->n; i++)
    {
        suma+=datos->vector[i];
    }
    datos->media = suma/((float)(datos->n));
    printf("El proceso [hijo 1] con PID [%d] calcula que el promedio es: %lf\n",getpid(),datos->media);
}

void numeroMayor(Datos *datos){
    int i = 0;
    for (i = 0; i < datos->n; i++)
    {
        if (datos->mayor<datos->vector[i])
        {
            datos->mayor = datos->vector[i];
        }
    }
    printf("El proceso [hijo 2] con PID [%d] calcula que el mayor es: %d\n",getpid(),datos->mayor);
}

void manejador(){
}

int main(){
    int vector[] = {1,2,3,4,5,6,7,8};
    int n = 8;

    signal(SIGUSR1,manejador);

    int shmid = shmget(IPC_PRIVATE,sizeof(Datos),IPC_CREAT | 0600);
    if(shmid==-1){
        printf("Error al crear el segmento de memoria");
    }

    void *shm_ptr = shmat(shmid,NULL,0);
    if(shm_ptr == (void*)-1){
        printf("Error adjuntado el segmento de memoria");
    }

    Datos *datos = (Datos*)shm_ptr;
    datos->n = n;
    datos->mayor = 0;
    datos ->media = 0;

    int j = 0;
    for (j = 0; j < datos->n; j++)
    {
        datos->vector[j] = vector[j];
    }
    int hijos[2],i=0;
    for (i = 0; i < 2; i++)
    {
        if (!(hijos[i]=fork()))
        {
            break;
        }
    }

 
    if(i==2){
        usleep(1000);
        printf("Soy el padre con PID [%d] \n",getpid());
        kill(hijos[i-1],SIGUSR1);
        pause();

        printf("[%d] El mayor es: %d\n",getpid(),datos->mayor);
        printf("[%d] El promedio es: %lf\n",getpid(),datos->media);
        shmctl(shmid,IPC_RMID,0);
    }
    if(i==1){
       pause(); //Pause hasta que alguna señal despirte a este hijo de prra
       numeroMayor(datos); //Llama a la funcion 
       kill(hijos[i-1],SIGUSR1); //sirve para despetar a el hijo anterior a el
       shmdt(shm_ptr); //Desvincula el segmento de memoria compartida 
    }
    if(i==0){
        pause();
        promedio(datos);
        kill(getppid(),SIGUSR1);
        shmdt(shm_ptr);
    }
    return 0;
}


