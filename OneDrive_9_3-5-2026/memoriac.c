#include <stdio.h> //Para funciones de I/O como Printf
#include <stdlib.h> //Para malloc, calloc, free
#include <sys/shm.h> //Para funciones de manejo de memoria compartida
#include <sys/ipc.h> //Necesario para flags IPC

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
    size_t size;
    size = rows * sizeof(void *); //indexSize
    size += (cols * rows * sizeElement); //Data size
    return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement){
    int i;
    size_t sizeRow = cols * sizeElement;
    m[0]= m + rows;
    for (i = 1; i < rows; i++){
        m[i] = (m[i-1]+sizeRow);
    }
}

int main(int argc, char const *argv[]){
    double **matrix = NULL;
    int Rows = 10, Cols = 10;
    size_t sizeMatrix = sizeof_dm(Rows, Cols, sizeof(double));
    int shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
    matrix = shmat(shmId, NULL, 0);
    create_index((void*)matrix, Rows, Cols, sizeof(double));
    
    //Accediendo a los elementos de la matriz para asignar valores.
    for (int i = 0; i < Rows; i++){
        for (int j = 0; j < Cols; j++){
            matrix[i][j] = (i * Cols) + j; //Asignar un valor de ejemplo basado en la posición.
        }
    }
    
    //Imprimiendo los elementos de la matriz para verificar su correcta asignación.
    for (int i = 0; i < Rows; i++){
        for (int j = 0; j < Cols; j++){
            printf("%f", matrix[i][j]);
        }  
        printf("\n");
    }   

    //Desconectar el segmento de memoria compartida
    shmdt(matrix);

    //Eliminar el segmento de memoria compartida
    shmctl(shmId, IPC_RMID, NULL);
    
    return 0;
}