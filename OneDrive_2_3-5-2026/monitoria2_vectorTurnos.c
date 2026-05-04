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
    int n; //numero de procesos
    int k; //numero de repeticiones
    int i, tam=0;
    int shm_id_vec, *vec; // vector de los turnos
    int shm_id_pos, *pos; // posicion del vector turno que indica el actual
    int shm_id_rep, *rep; // numero de repeticiones que se han realizado

    FILE *f = fopen("file2.txt", "r") ;
    if (!f) { printf("Error al abrir file \n"); exit(-1); }

    fscanf(f, "%d", &n); //sabemos que el primer numero del archivo indica el numero de procesos
    fscanf(f, "%d", &k); // el segundo numero del archivo inidica las repeticiones o iteraciones

    int inicio_vec = ftell(f); // posicion del archivo desde donde comienzan los valores de vector turnos
    int lectura;
    while(!feof(f)){
        fscanf(f, "%d", &lectura);
        tam++; // contamos el numero de elementos que tendra el vector de turnos
    }
    tam--; //al final siempre se repite el ultimo elemento del archivo, asi que se le resta 1

    printf("Se crearan %d procesos\n", n);
    printf("Se realizaran %d repeticiones o iteraciones\n", k);

    shm_id_pos = shmget(IPC_PRIVATE, sizeof(int), 0600);
    pos = shmat(shm_id_pos, 0, 0);
    shm_id_rep = shmget(IPC_PRIVATE, sizeof(int), 0600);
    rep = shmat(shm_id_rep, 0, 0);
    shm_id_vec = shmget(IPC_PRIVATE, tam*sizeof(int), 0600);
    vec = shmat(shm_id_vec, 0, 0);

    fseek(f, inicio_vec, 0); // reposicionamos el puntero para leer el archivo desde la posicion guardada
    printf("Este sera el orden que se seguira:\n");
    for (int a=0; a<tam; a++){
        fscanf(f, "%d", &vec[a]);
        printf("%d ", vec[a]);
    }
    printf("\n\n");
    fclose(f);

    pid_t root = getpid();
    *pos = 0; //inicimos leyendo el vector de turnos desde la posicion 0
    *rep = 0;
    for (i=0; i<n; i++) if (!fork()) break;

    while (*rep <  k){

        while (i != vec[*pos]); // mantiene al proceso aqui mientras no sea su turno
        if (*rep == k) break; //para que no se alcance a imprimir el primer turno luego de que rep = k

        printf("Proceso [%d] i=%d\n", getpid(), i);
        if (*pos == tam-1){
            *pos = 0;
            *rep += 1;
            printf("\n");
        }else{
            *pos += 1;
        }
    }

    // sleep(2);
    if(root==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
    }else sleep(3);


    return 0;
}
