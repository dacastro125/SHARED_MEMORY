#include <stdio.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

/*
    Memoria compartida
    - shmget: Solicita el segmento de memoria compartida
    - shmat: anexa el segmento a la tabla de direcciones de un determinado proceso
    - shmdt: contrario a la funcion anterior (desanexa)
    - shmctl: Elimina el segmento de memoria compartida

    Una pelicula en 3D
    - Cada persona es un proceso

    - Proyectada (shmget)
    - Gafas (shmat)
    - Quitarse las gafas (shmdt)
    - Apagar el proyector (shmctl)

*/

typedef int data;

int main(){
    system("clear");
    data *c;
    int shmid, i; //Descriptor del segmento de memoria compartida

    shmid = shmget(IPC_PRIVATE, sizeof(data)*3, 0666);
    c = shmat(shmid, NULL, 0);

    if(fork()){ //Preguntamos por el proceso padre
        for(i = 0; i < 3; i++){
            c[i] = (i+1) * 10;
        }
        shmdt(c);
    }else{
        sleep(1);
        for(i = 0; i < 3; i++){
            printf("\n%d\n",c[i]);
        }
        shmdt(c);
        shmctl(shmid, 0, NULL);
    }

    return 0;
}