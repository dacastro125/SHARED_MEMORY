#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>


unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
    size_t size;
    size = rows * sizeof(void *); //indexSize
    size += (cols * rows * sizeElement); //Data size
    return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement){
    int i;
    size_t sizeRow = cols * sizeElement;
    m[0] = m + rows;
    for(i=1; i<rows; i++){
        m[i] = (m[i-1] + sizeRow);
    }
}

void handler(int sig){
    printf("señal %d recibida. Ahora puede ejecutarse el hijo...\n", sig);
}

int main(){

    double **matrix = NULL;
    int Rows = 2, Cols = 2;
    size_t sizeMatrix = sizeof_dm(Rows,Cols,sizeof(double));
    int shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
    matrix = shmat(shmId, NULL, 0);
    create_index((void*)matrix, Rows, Cols, sizeof(double));

    pid_t pid = fork();

    if (pid < 0){
        printf("error creating process");
        return EXIT_FAILURE;
    }else if(pid == 0){
        matrix = shmat(shmId, NULL, 0);
        signal(SIGUSR1, handler);
        pause();
        for (int i = 0; i < Rows; i++){
            for (int j = 0; j < Cols; j++){
                printf("date of matrix %0.1f\n ", matrix[i][j]);
            }   
        }
        shmdt(matrix);
    }else{
        printf("Llenando datos desde el padre.\n");
        for (int i = 0; i < Rows; i++){
            for (int j = 0; j < Cols; j++){
                printf("Llene la matriz\n");
                scanf("%lf", &matrix[i][j]);
            }          
        }
        kill(pid, SIGUSR1);
        wait(NULL);
        printf("process complete\n");
    }
    shmdt(matrix);
    shmctl(shmId, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}