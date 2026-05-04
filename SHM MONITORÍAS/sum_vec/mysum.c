#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define MAX_RESULTS 1024

typedef struct {
    int results[MAX_RESULTS];
    int count;
    int is_use;
} SharedResults;

void showtree();

void readFileVec(const char *file, int **vec, int *n);
void createSHMv(int *x_shm, void **X, int n, size_t sizeof_t);
void copyVec(int *a, int *b, int n);

int main(int argc, char **argv){
    if(argc < 3){
        perror("Send File and n_childs\n");
        return EXIT_FAILURE;
    }
    int n_children = atoi(argv[2]), child_id, chunk;
    pid_t root = getpid();

    int *file_vec;
    int size;

    int vec_shm_id, reslt_shm_id;
    int *shared_vec;
    SharedResults *shared_results;

    readFileVec(argv[1], &file_vec, &size);

    createSHMv(&vec_shm_id, (void**)&shared_vec, size, sizeof(int));
    createSHMv(&reslt_shm_id, (void**)&shared_results, 1, sizeof(SharedResults)); //'cause we only wanna use a segment

    copyVec(shared_vec, file_vec, size);
    free(file_vec);
    file_vec = NULL;

    shared_results->count = 0;
    shared_results->is_use = 0;

    chunk = size / n_children;

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(root == getpid()){
        showtree();
        int sum=0;

        for(int i=0; i < n_children; i++) wait(NULL);
        
        for(int i=0; i < shared_results->count; i++) sum += shared_results->results[i];;

        printf("The sum of all values is: %d\n", sum);

        shmdt(shared_vec);
        shmdt(shared_results);
        shmctl(vec_shm_id, IPC_RMID, NULL);
        shmctl(reslt_shm_id, IPC_RMID, NULL);
    }else{
        int start, end, partial_sum;
        start = child_id * chunk;
        end = (child_id == n_children - 1) ? size : (child_id+1) * chunk;
        partial_sum = 0;

        printf("Child (%d) is reading:\n", getpid());
        for(int i = start; i < end; i++) partial_sum += shared_vec[i];

        printf("Child %d (%d) computed partial sum: %d\n", child_id, getpid(), partial_sum);

        //critical Section
        while(shared_results->is_use){
            printf("Child %d waiting for access...\n", child_id);
            usleep(100000);
        }
        shared_results->is_use = 1;

        printf("Child %d entering critical section.\n", child_id);
        shared_results->results[shared_results->count++] = partial_sum;
        printf("Child %d stored result at position %d.\n", child_id, shared_results->count - 1);

        shared_results->is_use = 0;
        printf("Child %d left critical section.\n\n", child_id);

        shmdt(shared_results);
        shmdt(shared_vec);
    }   

    return EXIT_SUCCESS;
}

void copyVec(int *a, int *b, int n){
    for(int i = 0; i < n; i++){
        a[i] = b[i];
    }
}

void createSHMv(int *x_shm, void **X, int n, size_t sizeof_t){
    size_t size_v = n * sizeof_t;

    *x_shm = shmget(IPC_PRIVATE, size_v, 0666 | IPC_CREAT);
    if(*x_shm == -1){
        perror("Error in shmget"); 
        exit(1);
    }
    *X = shmat(*x_shm, 0, 0);
    if(*X == NULL){
        perror("Error in shmat"); 
        exit(1);
    }
}

void readFileVec(const char *file, int **vec , int *n){
    FILE *fl = fopen(file, "r");
    if(!fl){perror("Fail reading file\n"); exit(1);}

    fscanf(fl, "%d", n);
    printf("Elements: %d\n", *n);

    *vec = (int*) malloc(*n * sizeof(int));
    if(!(*vec)){perror("Fail malloc in file\n"); exit(1);}
    
    for(int i=0; i < *n; i++){
        fscanf(fl, "%d", &(*vec)[i]);
    }

    /*for(int i=0; i < *n; i++){
        printf("%d\n", (*vec)[i]);
    }*/

    fclose(fl);
}

void showtree(){
    char cmd[64] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);	
}