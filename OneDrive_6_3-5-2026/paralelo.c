#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define MAX_PROCESSES 3

// Estructura para almacenar resultados en memoria compartida
struct SharedMemory {
    int count; // Conteo total de ocurrencias
    int positions[100]; // Posiciones donde se encontró el valor
    int pos_index; // Índice de la última posición insertada
};

// Función para contar ocurrencias en un subvector
void searchInSubvector(int *subvector, int size, int value, struct SharedMemory *sharedMem, int offset) {
    for (int i = 0; i < size; i++) {
        if (subvector[i] == value) {
            sharedMem->positions[sharedMem->pos_index++] = offset + i;
            sharedMem->count++;
        }
    }
}

int main() {
    int N, value, vector[100];
    FILE *file = fopen("datos.txt", "r");

    if (!file) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Leer la cantidad de elementos y el vector desde el archivo
    fscanf(file, "%d", &N);
    for (int i = 0; i < N; i++) {
        fscanf(file, "%d", &vector[i]);
    }
    fclose(file);

    printf("Ingrese el valor a buscar: ");
    scanf("%d", &value);

    // Crear memoria compartida
    int shm_id = shmget(IPC_PRIVATE, sizeof(struct SharedMemory), IPC_CREAT | 0666);
    struct SharedMemory *sharedMem = (struct SharedMemory *) shmat(shm_id, NULL, 0);
    sharedMem->count = 0;
    sharedMem->pos_index = 0;

    int elements_per_process = N / MAX_PROCESSES;

    // Crear procesos hijos
    for (int i = 0; i < MAX_PROCESSES; i++) {
        int start = i * elements_per_process;
        int end = (i == MAX_PROCESSES - 1) ? N : start + elements_per_process;

        if (fork() == 0) { // Proceso hijo
            searchInSubvector(&vector[start], end - start, value, sharedMem, start);
            shmdt(sharedMem); // Desconectar la memoria compartida en el proceso hijo
            exit(0);
        }
    }

    // Esperar a que terminen los procesos hijos
    for (int i = 0; i < MAX_PROCESSES; i++) {
        wait(NULL);
    }

    // Imprimir resultados
    printf("El valor %d se encontró %d veces en las posiciones: ", value, sharedMem->count);
    for (int i = 0; i < sharedMem->pos_index; i++) {
        printf("%d ", sharedMem->positions[i]);
    }
    printf("\n");

    // Liberar memoria compartida
    shmdt(sharedMem);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}