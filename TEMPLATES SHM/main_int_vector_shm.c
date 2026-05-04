#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

void showtree();

void readFileVec(const char *file, int **vec, int *n);
void createSHMv(int *x_shm, void **X, int n, size_t sizeof_t);
void copyVec(int *a, int *b, int n);

int main(int argc, char **argv){
    if(argc < 3){
        perror("Send File and n_childs\n");
        return EXIT_FAILURE;
    }
    int n_children = atoi(argv[2]), child_id;
    pid_t root = getpid();

    int *file_vec;
    int size;

    int *shared_vec;
    int vec_shm_id;

    readFileVec(argv[1], &file_vec, &size);

    createSHMv(&vec_shm_id, (void**)&shared_vec, size, sizeof(int)); 
    copyVec(shared_vec, file_vec, size);
    free(file_vec);
    file_vec = NULL;

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(root == getpid()){
        showtree();

        for(int i=0; i < n_children; i++) wait(NULL);

        shmdt(shared_vec);
        shmctl(vec_shm_id, IPC_RMID, NULL);
    }else{

        printf("Child (%d) is reading:\n", getpid());
        for(int i=0; i < size; i++){
            printf("%d ", shared_vec[i]);
        }
        printf("\n");

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

    for(int i=0; i < *n; i++){
        printf("%d\n", (*vec)[i]);
    }

    fclose(fl);
}

void showtree(){
    char cmd[64] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);	
}