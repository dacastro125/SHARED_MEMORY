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
    int shmid = shmget(key, SIZE, 0666);
    if (shmid == -1) {
        error("Error recuperado shmget");
    }

    char *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (void*)-1) {
        error("Error adjuntando segmento shmat");
    }

    char buffer[SIZE];
    buffer[0] = '\0';

    do {
        if (strcmp(buffer, shm_ptr)) {
            strcpy(buffer, shm_ptr);
            printf("%s\n", buffer);
        }
    } while(strcmp(shm_ptr, "salir"));

    shmdt(shm_ptr);
    printf("Finalizando... [%d]\n", getpid());

    return 0;
}

