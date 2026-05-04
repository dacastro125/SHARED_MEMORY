#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LEN 256

typedef struct {
    char word[MAX_LEN];
    int row;
    int column;
    char direction[10];
} Result;

typedef struct {
    Result results[100];
    int count;
    int in_use;
} Shared_Data;

void showtree();

unsigned int sizeof_dm(int rows, int cols, size_t sizeof_element);
void create_index(void **m, int rows, int cols, size_t sizeof_element);
void readFile(const char *file, char ***mtx , int *rows, int *columns);
void createSHMs(int *x_shm, void ***X, int rows, int columns, size_t sizeof_t);
void copyMatx(char **dst, char **src, int rows, int cols);

int main(int argc, char **argv){
    if(argc < 3){
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n_children = atoi(argv[2]), child_id;
    pid_t root = getpid();

    char **file_mtx;
    int rows, columns;

    char **shared_mtx;
    int mtx_shm_id, data_shm;
    Shared_Data *data;

    readFile(argv[1], &file_mtx, &rows, &columns);

    createSHMs(&mtx_shm_id, (void***)&shared_mtx, rows, columns, sizeof(char));

    data_shm = shmget(IPC_PRIVATE, sizeof(Shared_Data), 0666 | IPC_CREAT);
    if (data_shm == -1){ perror("shmget data"); exit(1); }

    data = (Shared_Data*) shmat(data_shm, 0, 0);
    if (data == (void*)-1){ perror("shmat data"); exit(1); }

    data->count = 0;
    data->in_use = 0;

    copyMatx(shared_mtx, file_mtx, rows, columns);

    for(child_id=0; child_id < n_children; child_id++){
        if(fork() == 0) break;
    }

    if(root == getpid()){
        for(int i=0; i < n_children; i++) wait(NULL);

        for(int i=0; i < data->count; i++){
            printf("Word '%s' found at [%d][%d] in the %s.\n",
                   data->results[i].word, data->results[i].row, data->results[i].column, data->results[i].direction);
        }

        shmdt(shared_mtx);
        shmctl(mtx_shm_id, IPC_RMID, NULL);
        shmdt(data);
        shmctl(data_shm, IPC_RMID, NULL);
    }else{
        //use
        printf("Child %d reading element [%c]\n", child_id, shared_mtx[0][0]);

        shmdt(shared_mtx);
        shmdt(data);
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}

void copyMatx(char **dst, char **src, int rows, int cols){
    for(int i=0; i < rows; i++){
        for(int j=0; j < cols; j++){
            dst[i][j] = src[i][j];
        }
    }       

    for(int i=0; i < rows; i++){
        free(src[i]);
        src[i] = NULL;
    }
    free(src);
}

void createSHMs(int *x_shm, void ***X, int rows, int columns, size_t sizeof_t){
    size_t size_m = sizeof_dm(rows, columns, sizeof_t);
    *x_shm = shmget(IPC_PRIVATE, size_m, 0666 | IPC_CREAT);
    if(*x_shm == -1){ perror("Error in shmget matrix"); exit(1); }

    void *base = shmat(*x_shm, 0, 0);
    if(base == (void*)-1){ perror("Error in shmat matrix"); exit(1); }

    *X = (void**) base;
    create_index((void**)*X, rows, columns, sizeof_t);
}

unsigned int sizeof_dm(int rows, int cols, size_t sizeof_element){
    size_t size = rows * sizeof(void*);
    size += (rows * cols * sizeof_element);
    return size;
}

void create_index(void **m, int rows, int cols, size_t sizeof_element){
    char *data = (char*)(m + rows);
    for(int i = 0; i < rows; i++){
        m[i] = data + i * cols * sizeof_element;
    }
}

void readFile(const char *file, char ***mtx , int *rows, int *columns){
    FILE *fl = fopen(file, "r");
    if(!fl){ perror("Error opening file"); exit(1); }

    fscanf(fl, "%d", rows);
    fscanf(fl, "%d", columns);
    printf("Rows: %d, Columns: %d\n", *rows, *columns);

    *mtx = (char **) malloc(*rows * sizeof(char *));
    for(int i = 0; i < *rows; i++){
        (*mtx)[i] = (char *) malloc(*columns * sizeof(char));
    }
    
    for(int i=0; i < *rows; i++){
        for(int j=0; j < *columns; j++){
            fscanf(fl, " %c", &(*mtx)[i][j]);
        }
    }

    printf("File content:\n");
    for(int i=0; i < *rows; i++){
        for(int j=0; j < *columns; j++){
            printf("%c ", (*mtx)[i][j]);
        }
        printf("\n");
    }

    fclose(fl);
}

void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);	
}