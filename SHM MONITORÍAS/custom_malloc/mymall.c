#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_SIZE 4096
#define MAX_VEC 64

int TOTAL_SIZE = 0;

typedef enum {
    LOCK,
    UNLOCKED,
} Values;

typedef struct {
    pid_t pid;
    int offset;
    int length;
} VecInfo;

typedef struct {
    int lock;
    int offset;
    int size;
    int nvecs;
    VecInfo info[MAX_VEC]; 
    unsigned char memory[];
} SharedMem;

void createSHMv(int *x_shm, void **X, int n, size_t sizeof_t);
void* shm_malloc(SharedMem *shm, int size);
void shm_free(SharedMem *shm, void *ptr);

int main(int argc, char **argv){
    if(argc < 3){
        perror("Send size memory and children\n");
        return EXIT_FAILURE;
    }
    int mem_size = atoi(argv[1]);
    int n_children = atoi(argv[2]), child_id;
    pid_t root = getpid();
    SharedMem *shared_vec;
    int vec_shm_id;

    createSHMv(&vec_shm_id, (void**)&shared_vec, mem_size, sizeof(SharedMem));
    TOTAL_SIZE = sizeof(SharedMem) + mem_size * sizeof(unsigned char);

    printf("Struct: %ld and Total segment: %d bytes\n", sizeof(SharedMem), TOTAL_SIZE);

    shared_vec->lock = UNLOCKED;
    shared_vec->offset = 0;
    shared_vec->size = 0;
    shared_vec->nvecs = 0;

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(root == getpid()){
        long int sum=0;

        for(int i=0; i < n_children; i++) wait(NULL);

        printf("\n");
        for(int i=0; i <n_children; i++){
            printf("Sum of the data from child %d: ", shared_vec->info[i].pid);
            int *data = (int*)(shared_vec->memory + shared_vec->info[i].offset);
            for(int j = 0; j < shared_vec->info[i].length; j++){
                sum += data[j];
            }       
            printf("%ld\n", sum);
            sum = 0;
        }

        printf("\nSummary of allocations:\n");
        for (int i = 0; i < shared_vec->nvecs; i++){
            printf("PID %d -> offset %d, len %d\n", shared_vec->info[i].pid, shared_vec->info[i].offset, shared_vec->info[i].length);
        }
        printf("total elements in file: %d\n", shared_vec->size);

        shmdt(shared_vec);
        shmctl(vec_shm_id, IPC_RMID, NULL);    
    }else{
        int n, *vec, sum=0;
        char file[64];
        FILE *fl;

        sprintf(file, "data_%d.in", child_id);
        //printf("Child: %d, is reading: %s\n", child_id, file);

        fl = fopen(file, "r");
        if(!fl){
            perror("file");
            exit(1);
        }

        fscanf(fl, " %d", &n);

        while(shared_vec->lock == LOCK);
        
        shared_vec->lock = LOCK;
        shared_vec->size += n;
        shared_vec->lock = UNLOCKED;

        vec = (int*) shm_malloc(shared_vec, n * sizeof(int));
        if (!vec) {
            printf("Child %d: without memory\n", getpid());
            fclose(fl);
            exit(1);
        }

        for(int i=0; i < n; i++){
            fscanf(fl, "%d", &vec[i]);
            sum += vec[i];
        }
        printf("Sum child %d: %d\n", child_id, sum);

        shm_free(shared_vec, vec);
        fclose(fl);
        shmdt(shared_vec);
    }

    return EXIT_SUCCESS;
}

void* shm_malloc(SharedMem *shm, int size){
    while(shm->lock == LOCK);
    shm->lock = LOCK; //my turn

    if(shm->offset + size > TOTAL_SIZE){
        fprintf(stderr, "Not enough shared memory: requested %d, remaining %d\n", size, shm->size - shm->offset);
        shm->lock = UNLOCKED;
        return NULL;
    }

    void *ptr = shm->memory + shm->offset;
    shm->offset += size;
    shm->info[shm->nvecs].pid = getpid();
    shm->info[shm->nvecs].offset = shm->offset - size;
    shm->info[shm->nvecs].length = size / sizeof(int);
    shm->nvecs++;

    shm->lock = UNLOCKED;
    return ptr;
}

void shm_free(SharedMem *shm, void *ptr){
    (void)shm;
    (void)ptr;
}

void createSHMv(int *x_shm, void **X, int n, size_t sizeof_t){
    size_t size_v = sizeof_t + n * sizeof(unsigned char);

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