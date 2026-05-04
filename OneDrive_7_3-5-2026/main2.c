#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>

#define NUMHIJOS 3

void cargarMatriz(FILE *file, int *nCol, int *nFilas, int ***matriz);
void llenarMatriz(FILE *file, int nCol, int nFilas, int ***matriz);
void mostrarMatriz(int nCol, int nFilas, int **matriz);
void multMatriz(int **matriz1, int **matriz2, int nFilas1, int nCol2, int hijoId, int ***matrizRes);

int main (int argc, char *argv[]) {

    if (argc < 3) {
        printf("Se deben pasar 2 argumentos aparte del ./main2\n");
        exit(EXIT_FAILURE);
    }

    int hijoId, shmId;
    int **matrizRes;
    int nCol1, nCol2, nFila1, nFila2;
    int **matriz1;
    int **matriz2;

    pid_t pidPadre = getpid();
    pid_t pid;

    FILE *file1 = fopen(argv[1], "r");
    FILE *file2 = fopen(argv[2], "r");

    cargarMatriz(file1, &nCol1, &nFila1, &matriz1);
    cargarMatriz(file2, &nCol2, &nFila2, &matriz2);

    llenarMatriz(file1, nCol1, nFila1, &matriz1);
    llenarMatriz(file2, nCol2, nFila2, &matriz2);

    mostrarMatriz(nCol1, nFila1, matriz1);
    mostrarMatriz(nCol2, nFila2, matriz2);

    shmId = shmget(IPC_PRIVATE, nCol1 * nFila2 * sizeof(int*), IPC_CREAT | S_IWUSR | S_IRUSR);

    for (hijoId = 0; hijoId < NUMHIJOS; hijoId++) {
        pid = fork();
    }

    matrizRes = shmat(shmId, 0, 0);

    if (pid == 0) {
        printf("hola");
        multMatriz(matriz1, matriz2, nFila1, nCol2, hijoId, &matrizRes);
    }
    mostrarMatriz(nCol2, nFila1, matrizRes);

    return 0;
}

void cargarMatriz(FILE *file, int *nCol, int *nFilas, int ***matriz) {
    fscanf(file, "%d", nCol);
    fscanf(file, "%d", nFilas);

    *matriz = (int **)malloc(*nFilas *sizeof(int *));

    for (int i = 0; i < *nFilas; i++) {
        (*matriz)[i] = (int *)malloc(*nCol *sizeof(int));
    }
}

void llenarMatriz(FILE *file, int nCol, int nFilas, int ***matriz){
    int num;
    for (int i = 0; i < nFilas; i++) {
        for (int j = 0; j < nCol; j++) {
            fscanf(file, "%d", &num);
            (*matriz)[i][j] = num;
        }
    }
}

void mostrarMatriz(int nCol, int nFilas, int **matriz) {
    printf("\n");
    for (int i = 0; i < nFilas; i++) {
        for (int j = 0; j < nCol; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}


void multMatriz(int **matriz1, int **matriz2, int nFilas1, int nCol2, int hijoId, int ***matrizRes) {
    //printf("%d\n", nCol2);
    for (int i = hijoId; i < nFilas1; i += 3) {
        for (int j = 0; j < nCol2; j++) {
            (*matrizRes)[i][j] = 0;

            for (int k = 0; k < nFilas1; k++) {
                (*matrizRes)[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }
}




