#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>

int main()
{
    printf("------\n");
    void *ptr;//Apuntador a la variabletipo char
    int shm_id;//Id de la variable tipo char
    int shm_size = 1024;//Tamaño 

    shm_id= shmget (IPC_PRIVATE, shm_size, IPC_CREAT | 0600);//Crear el espacio de memoria
    ptr = shmat (shm_id, NULL, 0);//Asignar el espacio de memoria
    printf("Soy el proceso %d\n",getpid());
    if(fork()==0){
        printf("soy el IGUAL A 0 %d, mi padre es %d\n",getpid(),getppid());
        sleep(2);//Dormir zzz
        printf("[%d]%s\n", getpid(), (char*)ptr);//Imprime lo que se encuentra ben la direccion de memoria
        sprintf(ptr, "bye!");//Escribe directamente al espacio de memoria compartida
        shmdt(ptr);//Pasa o recibe información de la memoria compartida. 
    }
    else{
        printf("soy el DIFERENTE DE 0 %d, mi padre es %d\n",getpid(),getppid());
        sprintf(ptr, "hola mundo");//Escribe directamente al espacio de memoria compartida
        printf("[%d]%s\n", getpid(), (char*)ptr);//Imprime lo que se encuentra ben la direccion de memoria
        wait(NULL);//FINALIZA Y ESPERA LO QUE ME MANDE EL HIJO
        printf("[%d]%s\n", getpid(), (char*)ptr );//Imprime lo que se encuentra ben la direccion de memoria que escribio el hijo
        shmdt(ptr);//También sirve para destruir el segmento de memoria compartida.
        shmctl(shm_id, IPC_RMID,0);//Elimina el segmento de memoria
    }
    return 0;
}