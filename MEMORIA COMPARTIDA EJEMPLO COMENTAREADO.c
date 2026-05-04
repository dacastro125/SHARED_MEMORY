#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>

int main()
{
    printf("------\n");
    void *ptr;//Apuntador a la variable tipo char
    int shm_id;//Id de la variable tipo char
    int shm_size = 1024;//Tamaño que va a tener la memoria compartida

    shm_id= shmget (IPC_PRIVATE, shm_size, IPC_CREAT | 0600);//Crear el espacio de memoria
    ptr = shmat (shm_id, NULL, 0);//Asignar el espacio de memoria
    printf("Soy el proceso %d\n",getpid());//Imprime el Id del proceso principal
    if(fork()==0){
        printf("Soy el hijo %d, mi padre es %d\n",getpid(),getppid());//El Id Hijo y Padre
        sleep(2);//Dormir zzz
        printf("[%d]%s\n", getpid(), (char*)ptr);//Imprime lo que se encuentra en la direccion de memoria que le mando el padre
        sprintf(ptr, "bye!");//Escribe directamente al espacio de memoria compartida
        shmdt(ptr);//Pasa información a la memoria compartida. 
    }
    else{
        /*
        Siempre empieza por el padre
        como no es atomico apenas que llega al wait termina y ejecuta el hijo
        cuando termina de ejecutar el hijo retoma el padre y empieza en la linea despues del wait
        */
        printf("Soy el Padre %d\n",getpid());//Imprime Id Padre
        sprintf(ptr, "hola mundo");//Escribe directamente al espacio de memoria compartida
        printf("[%d]%s\n", getpid(), (char*)ptr);//Imprime lo que se encuentra en la direccion de memoria
        wait(NULL);//FINALIZA Y ESPERA LO QUE ME MANDE EL HIJO
        printf("[%d]%s\n", getpid(), (char*)ptr );//Imprime lo que se encuentra en la direccion de memoria que escribio el hijo
        shmdt(ptr);//También sirve para destruir el segmento de memoria compartida.
        shmctl(shm_id, IPC_RMID,0);//Elimina el segmento de memoria
    }
    return 0;
}