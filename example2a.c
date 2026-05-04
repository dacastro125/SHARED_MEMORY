#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#define SIZE 50

void error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}

int main(int argc, char *argv[]) {

    int key = ftok(".", 2016);
    int shmid = shmget(key, SIZE, IPC_CREAT|0666);
    if (shmid == -1) {
        error("Error creando shmget");
    }

    char *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (void*)-1) {
        error("Error adjuntando segmento shmat");
    }

    char buffer[SIZE];
    buffer[0] = '\0';

    do {
        scanf("%s", buffer);
        strcpy(shm_ptr, buffer);
        printf("%s\n", buffer);
    } while(strcmp(buffer, "salir"));

    shmctl(shmid, IPC_RMID, NULL);
    printf("Finalizando... [%d]\n", getpid());

    return 0;
}
