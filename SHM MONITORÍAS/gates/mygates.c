#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

typedef struct {
    int COM, OP, Time, done;
} Data_Handler;

int main(){
    int n_children = 5, child_id;
    int shm_id;
    Data_Handler *shm_data;
    pid_t root = getpid(); 

    shm_id = shmget(IPC_PRIVATE, sizeof(Data_Handler), 0666 | IPC_CREAT);
    if(shm_id < 0){printf("shmid error\n"); exit(1);}

    shm_data = (Data_Handler*) shmat(shm_id, 0, 0);
    if(shm_data == NULL){perror("Error in shmat"); exit(1);}

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(root == getpid()){
        int exit_flag = 0, turn;

        while(!exit_flag){
            usleep(200);
            printf("\nSelect a gate (1 to %d, or 0 to exit): ", n_children);
            scanf("%d", &turn);
            printf("Operation (0 = close, 1 = open): ");
            scanf("%d", &shm_data->OP);
            printf("Timer (seconds): ");
            scanf("%d", &shm_data->Time);

            shm_data->COM = turn;
            shm_data->done = 0;
            if(shm_data->COM == 0){
                exit_flag = 1;
                break;
            }

            while(shm_data->done == 0){
                usleep(10000);
            }
        }

        for (int i = 0; i < n_children; i++) wait(NULL);
        shmdt(shm_data);
        shmctl(shm_id, IPC_RMID, 0);

    }else{
        int isOpen = 0;
        while(1){
            if(shm_data->COM == 0) break; 
            if(shm_data->COM != child_id + 1){
                sleep(1);
                continue;
            }

            switch(shm_data->OP){
                case 1: // Open
                    if(!isOpen){
                        printf("Gate %d will open in %d seconds\n", shm_data->COM, shm_data->Time);
                        usleep(shm_data->Time * 1000000);
                        printf("Gate %d opened\n", shm_data->COM);
                        isOpen = 1;
                    } else {
                        printf("Gate %d is already open...\n", shm_data->COM);
                    }
                    break;

                case 0: // Close
                    if (isOpen) {
                        printf("Gate %d will close in %d seconds\n", shm_data->COM, shm_data->Time);
                        usleep(shm_data->Time * 1000000);
                        printf("Gate %d closed\n", shm_data->COM);
                        isOpen = 0;
                    } else {
                        printf("Gate %d is already closed...\n", shm_data->COM);
                    }
                    break;
            }

            shm_data->COM = -1; // Reset to avoid repeated execution
            shm_data->done = 1;
            usleep(10000);
        }
        shmdt(shm_data);
    }

    return EXIT_SUCCESS;
}