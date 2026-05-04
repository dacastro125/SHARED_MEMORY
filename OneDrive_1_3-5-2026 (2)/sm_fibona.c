#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define MAX_SEQUENCE 10

typedef struct{
    long fib_sequence[MAX_SEQUENCE];
    int sequence_size;
} shared_data;

int main(){
    int number, smId;

    shared_data *shared_memory;
    int sizeMemory = sizeof(shared_data);

    printf("Please input a number: ");
    scanf("%d", &number);

    if (number > MAX_SEQUENCE || number < 0){
        printf("The number is too large!\n");
        return -1;
    }

    smId = shmget(IPC_PRIVATE, sizeMemory, IPC_CREAT | 0666);
    if (smId < 0){
        printf("shmget error!\n");
        return -1;
    }
    int pid = fork();
    if (pid < 0){
        printf("fork error!\n");
        exit(1);
    }else if (pid == 0){
        shared_memory = (shared_data *) shmat(smId, NULL, 0);
        printf("child started...\n");
        if (shared_memory == (void *) -1){
            printf("shmat error!\n");
            exit(1);
        }
        shared_memory->sequence_size = number;
        shared_memory->fib_sequence[0] = 0;
        shared_memory->fib_sequence[1] = 1;
        for (int i = 2; i < shared_memory->sequence_size; i++){
            shared_memory->fib_sequence[i] = shared_memory->fib_sequence[i-1]+shared_memory->fib_sequence[i-2];
        }  <
        shmdt(shared_memory);
    }else{
        shared_memory = (shared_data *) shmat(smId, NULL, 0);
        printf("parent started...\n");
        wait(NULL);
        printf(" Serie para %d: ", number);
        for (int j = 0; j < shared_memory->sequence_size; j++){
            printf(" %ld",shared_memory->fib_sequence[j]);
        }
    }
    shmdt(shared_memory);
    shmctl(smId, IPC_RMID, NULL);
    
    return EXIT_SUCCESS;
}