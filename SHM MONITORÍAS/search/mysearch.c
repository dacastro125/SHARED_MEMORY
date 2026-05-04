#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include <sys/wait.h>

/*
    gcc -DDEBUG -o a.out mysearch.c 
    gcc mysearch.c -o a.out 
*/

void readFile(const char *file, int **vec, int *size);
void createSHM(int *x_shm, int **X, int size);

void showtree();

int main(int argc, char **argv){
    if(argc < 4){
        perror("Send file, number of SubProcess and number to search\n");
        return EXIT_FAILURE;
    }
    int *vec, size, chunk;
    int n_children = atoi(argv[2]), child_id;
    pid_t root = getpid();

    int search_number = atoi(argv[3]);

    int shm_vec_id, shm_inc_id, shm_data_id, shm_pid_id; //vector_id and incidents_id
    int *shm_vec, *shm_inc, *shm_data, *shm_pid;
    
    readFile(argv[1], &vec, &size);
    chunk = size / n_children;
    printf("Chunk: %d\n", chunk);

    createSHM(&shm_vec_id, &shm_vec, size);
    for(int i=0; i < size; i++) shm_vec[i] = vec[i]; //copy the vector
    free(vec);

    createSHM(&shm_inc_id, &shm_inc, n_children);
    createSHM(&shm_pid_id, &shm_pid, n_children);
    createSHM(&shm_data_id, &shm_data, n_children * chunk);

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(getpid() == root){
        showtree();
        int found_count, total = 0, pos;
        pid_t child;
        for(int i=0; i < n_children; i++) wait(NULL);

        #ifdef DEBUG
            printf("DEBUG shm_pid: ");
            for (int k = 0; k < n_children; k++)
                printf("%d ", shm_pid[k]);
            printf("\n");

            printf("DEBUG shm_inc: ");
            for (int k = 0; k < n_children; k++)
                printf("%d ", shm_inc[k]);
            printf("\n");

            printf("DEBUG shm_data: ");
            for (int k = 0; k < n_children * chunk; k++){
                //if(!shm_data[k]) continue;
                printf("%d ", shm_data[k]);
            }                
            printf("\n\n");
        #endif

        for(int i=0; i < n_children; i++){
            found_count = shm_inc[i];
            child = shm_pid[i];
            for(int j=0; j < found_count; j++){
                pos = shm_data[i * chunk + j];   
                printf("Found by child PID[%d] in the position [%d]\n", child, pos);
                total++;
            }
        }

        printf("Total incidents %d\n", total);

        shmdt(shm_vec);
        shmdt(shm_inc);
        shmdt(shm_data);
        shmdt(shm_pid);

        shmctl(shm_pid_id, IPC_RMID, 0);
        shmctl(shm_data_id, IPC_RMID, 0);
        shmctl(shm_inc_id, IPC_RMID, 0);
        shmctl(shm_vec_id, IPC_RMID, 0);
    }else{
        usleep(10000);
        int start = chunk * child_id;
        int end = (child_id == n_children-1) ? size : (child_id+1) *chunk;
        int count = 0;

        shm_pid[child_id] = getpid();

        for(int i=start; i < end; i++){
            if(shm_vec[i] == search_number){
                //  printf("child %d pos: %d\n", getpid(), i);
                shm_data[start + count] = i;   
                count++;            
            }
        }
        shm_inc[child_id] = count;
        
        shmdt(shm_vec);
        shmdt(shm_data);
        shmdt(shm_pid);
        shmdt(shm_inc);
    }
    
    return EXIT_SUCCESS;
}

void createSHM(int *x_shm, int **X, int size){
    size_t vec_sise = size * sizeof(int);

    *x_shm = shmget(IPC_PRIVATE, vec_sise, 0666 | IPC_CREAT);
    if(*x_shm == -1){perror("Error in shmget"); exit(1);}
    
    *X = (int*) shmat(*x_shm, 0, 0);
    if(*X == NULL){perror("Error in shmat"); exit(1);}
}

void readFile(const char *file, int **vec, int *size){
    FILE *fl = fopen(file, "r");
    if(!fl){perror("Fail fopen\n"); exit(1);}

    fscanf(fl, "%d", size);
    printf("Size: %d\n", *size);

    *vec = (int*) malloc(*size * sizeof(int));
    if(!*vec){exit(1);}

    for(int i=0; i < *size; i++){
        fscanf(fl, "%d", &(*vec)[i]);
    }

    fclose(fl);
}

void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);	
}