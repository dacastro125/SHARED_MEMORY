#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

unsigned int sizeof_dm(int rows, int cols, size_t sizeof_element);
void create_index(void **m, int rows, int cols, size_t sizeof_element);

void readFile(const char *file, int ***mtx , int *rows, int *columns);
void createSHMs(int *x_shm, void ***X, int rows, int columns, size_t sizeof_t);
void copyMatx(int **a, int **b, int rows, int cols);

float blurFilter(int **mtx, int rows, int columns, int dx, int dy, int kernel_size, int child_id);

int main(int argc, char **argv){
    if(argc < 3){
        perror("Send file .in and the number of Subprocess\n");
        return EXIT_FAILURE;
    }
    int n_children = atoi(argv[2]), child_id;
    pid_t root = getpid();

    int chunk;

    int **file_mtx;
    int rows, columns;

    int **shared_mtx;
    float **result_mtx;
    int mtx_shm_id, result_shm_id;

    readFile(argv[1], &file_mtx, &rows, &columns);

    createSHMs(&result_shm_id, (void***)&result_mtx, rows, columns, sizeof(float)); //result typ float
    createSHMs(&mtx_shm_id, (void***)&shared_mtx, rows, columns, sizeof(int));  //base mtx
    copyMatx(shared_mtx, file_mtx, rows, columns);
    
    for(int i=0; i < rows; i++){
      free(file_mtx[i]);
      file_mtx[i] = NULL;
    }
    free(file_mtx);
    *file_mtx = NULL;

    chunk = rows / n_children;    

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(root == getpid()){
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < columns; j++){
                printf("[%d]\t", shared_mtx[i][j]);
            }
            printf("\n");
        }

        for(int i=0; i < n_children; i++) wait(NULL);

        printf("Result Matrix after the Blur Filter:\n");
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < columns; j++){
                printf("[%.4f]\t", result_mtx[i][j]);
            }
            printf("\n");
        }

        shmdt(shared_mtx);
        shmctl(mtx_shm_id, IPC_RMID, NULL);
        shmdt(result_mtx);
        shmctl(result_shm_id, IPC_RMID, NULL);
    }else{
        int start = child_id * chunk;
        int end = child_id == n_children-1 ? rows : (child_id + 1 ) * chunk;
        
        int kernel_size = 3; //3x3
        float result = 0;

        for(int i=start; i < end; i++){
            for(int j=0; j < columns; j++){
                result = blurFilter(shared_mtx, rows, columns, i, j, kernel_size, child_id);
                result_mtx[i][j] = result;
            }
        }
        shmdt(shared_mtx);
        shmdt(result_mtx);
    }

    return EXIT_SUCCESS;
}

float blurFilter(int **mtx, int rows, int columns, int dx, int dy, int kernel_size, int child_id){
    int start_R = dx - kernel_size, end_R = dx + kernel_size;
    int start_C = dy - kernel_size, end_C = dy + kernel_size;
    float result = 0;
    int count = 0;

    for(int i=start_R; i < end_R; i++){
        for(int j=start_C; j < end_C; j++){
            if((i >= 0 && i < rows) && (j >= 0 && j < columns)){
                result += mtx[i][j];
                count++;
            }            
        }
    }
    //printf("child id [%d]. Count in [%d][%d] = %d\n", child_id, dx, dy, count);
    return result / (kernel_size * kernel_size);
}

void copyMatx(int **a, int **b, int rows, int cols){
    for(int i=0; i < rows; i++){
        for(int j=0; j < cols; j++){
            a[i][j]=b[i][j];
        }
    }
}

void createSHMs(int *x_shm, void ***X, int rows, int columns, size_t sizeof_t){
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