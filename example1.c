#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}

int main(int argc, char *argv[]) {

    int chpid;

    int shmid = shmget(IPC_PRIVATE, 50, IPC_CREAT|0666);
    if (shmid == -1) {
        error("Error creando shmget");
    }

    char *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (void*)-1) {
        error("Error adjuntando segmento shmat");
    }

    if ((chpid = fork()) == 0) {
        usleep(10000);
        printf("[%d] Leido: %s\n", getpid(), shm_ptr);
        shmdt(shm_ptr);
    } else {
        strcpy(shm_ptr, "Hola memoria compartida");
        printf("[%d] Escrito: Hola memoria compartida\n", getpid());
        wait(NULL);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
