#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

void showtree();

unsigned int sizeof_dm(int rows, int cols, size_t sizeof_element);
void create_index(void **m, int rows, int cols, size_t sizeof_element);

void readFile(const char *file, int ***mtx , int *rows, int *columns);
void createSHMs(int *x_shm, void **X, int rows, int columns, size_t sizeof_t);
void copyMatx(int **a, int **b, int rows, int cols);

int main(int argc, char **argv){
    if(argc < 3){
        perror("Send File and n_childs\n");
        return EXIT_FAILURE;
    }

    int n_children = atoi(argv[2]), child_id;
    pid_t root = getpid();

    int **file_mtx;
    int rows, columns;

    int **shared_mtx;
    int mtx_shm_id;

    readFile(argv[1], &file_mtx, &rows, &columns);

    // parameters: id, (cast to void to use a generic)matrix, r, c, type of element in bytes
    createSHMs(&mtx_shm_id, (void**)&shared_mtx, rows, columns, sizeof(int));

    copyMatx(shared_mtx, file_mtx, rows, columns);
    for(int i=0; i < rows; i++){
      free(file_mtx[i]);
      file_mtx[i] = NULL;
    }
    free(file_mtx);
    *file_mtx = NULL;

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(root == getpid()){
        showtree();

        for(int i=0; i < n_children; i++) wait(NULL);

        shmdt(shared_mtx);
        shmctl(mtx_shm_id, IPC_RMID, NULL);
    }else{

        printf("Child (%d) is reading:\n", getpid());
        for(int i=0; i < rows; i++){
            for(int j=0; j < columns; j++){
                printf("%d ", shared_mtx[i][j]);
            }
            printf("\n");
        }

        shmdt(shared_mtx);
    }
    
    return EXIT_SUCCESS;
}

void copyMatx(int **a, int **b, int rows, int cols){
    for(int i=0; i < rows; i++){
        for(int j=0; j < cols; j++){
            a[i][j]=b[i][j];
        }
    }
}

void createSHMs(int *x_shm, void **X, int rows, int columns, size_t sizeof_t){
    size_t size_m = sizeof_dm(rows, columns, sizeof_t);

    *x_shm = shmget(IPC_PRIVATE, size_m, 0666 | IPC_CREAT);
    if(*x_shm == -1){
        perror("Error in shmget"); exit(1);
    }
    *X = shmat(*x_shm, 0, 0);
    if(*X == NULL){
        perror("Error in shmat"); exit(1);
    }
    create_index((void*)*X, rows, columns, sizeof_t);
}

void readFile(const char *file, int ***mtx , int *rows, int *columns){
    FILE *fl = fopen(file, "r");
    if(!fl){perror("Fail reading file\n"); exit(1);}

    fscanf(fl, "%d", rows);
    fscanf(fl, "%d", columns);
    printf("Rows: %d, Columns: %d.\n", *rows, *columns);

    *mtx = (int**) malloc(*rows * sizeof(int*));
    if(!(*mtx)){perror("Fail malloc in rows file\n"); exit(1);}
    for(int i=0; i < *rows; i++){
        (*mtx)[i] = (int*) malloc(*columns * sizeof(int));
        if(!(*mtx)[i]){perror("Fail malloc in columns file\n"); exit(1);}
    }
    
    for(int i=0; i < *rows; i++){
        for(int j=0; j < *columns; j++){
            fscanf(fl, "%d", &(*mtx)[i][j]);
        }
    }

    for(int i=0; i < *rows; i++){
        for(int j=0; j < *columns; j++){
            printf("%d ", (*mtx)[i][j]);
        }
        printf("\n");
    }

    fclose(fl);
}

unsigned int sizeof_dm(int rows, int cols, size_t sizeof_element){
    size_t size = rows * sizeof(void*);
    size += (cols * rows * sizeof_element);
    return size;
}

void create_index(void **m, int rows, int cols, size_t sizeof_element){
    int i; 
    size_t sizeRows = cols * sizeof_element;
    m[0] = m + rows;
    for(i = 1; i < rows; i++){
        m[i] = (m[i-1] + sizeRows);
    }
}

void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);	
}