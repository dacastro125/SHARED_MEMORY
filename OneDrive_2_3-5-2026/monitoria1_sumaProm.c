#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main(){
    system("clear");
    int i, n;
    int *turno, *vector;
    float *result;
    int shm_id_turno, shm_id_vector, shm_id_result;

    shm_id_turno = shmget(IPC_PRIVATE, sizeof(int), 0600);
    turno = shmat(shm_id_turno, NULL, 0);
    shm_id_result = shmget(IPC_PRIVATE, 2*sizeof(float), 0600);
    result = shmat(shm_id_result, NULL, 0);

    FILE *f = fopen("file.txt", "r");
    if (!f){
        printf("Error al abrir el archivo\n");
        exit(-1);
    }
    int c=0;
    fscanf(f, "%d", &n);
    while (!feof(f)){
        fscanf(f, "%d", &n);
        c++;
    }
    fseek(f, 0, 0); // se vuelve a colocar el puntero al inicio del archivo

    shm_id_vector = shmget(IPC_PRIVATE, c*sizeof(int), 0600);
    vector = shmat(shm_id_vector, NULL, 0);

    pid_t root = getpid();
    *turno = 2;
    for (i=0; i<2; i++) if (!fork()) break;

    if(root==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
    }else{
        sleep(3);
    }

    if (i == 2){
        printf("\n\n");
        printf("soy el padre [%d] y cargo los elementos del archivo al vector\n", getpid());
        for (int a=0; a<c; a++) fscanf(f, "%d", &vector[a]);
        fclose(f);
        *turno = 0;
        for (int b=0; b<2; b++) wait(NULL); //para esperar que cada uno de los hijos termine
        printf("\nSoy el padre y muestro los resultados:\n");
        printf("La suma de los elemento es %.2f\n", result[0]);
        printf("El promedio de los elemento es %.2f\n", result[1]);

        shmctl(shm_id_turno, IPC_RMID, 0);
        shmctl(shm_id_vector, IPC_RMID, 0);
        shmctl(shm_id_result, IPC_RMID, 0);
    }else{
        if (i == 0){
            while (*turno != i) sleep(1); //mantiene al proceso en esta linea mientras no sea su turno
            printf("soy el hijo %d [%d] y realizo la suma\n", i ,getpid());
            for (int d=0; d<c; d++) result[0] += vector[d];
            *turno += 1;
        }else{
            while (*turno != i) sleep(1); //cuando i vale 1
            printf("soy el hijo %d [%d] y saco el promedio\n", i ,getpid());
            result[1] = result[0] / c;
        }

    }
    shmdt(turno); shmdt(vector); shmdt(result);

    return 0;
}
