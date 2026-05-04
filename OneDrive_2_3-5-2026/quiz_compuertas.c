#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>

struct msg {
    int COM;
    int OP;
    int Time;
};

int main(){
    system("clear");
    int np = 5; // numero de procesos que representan las compuertas
    int i, shm_id, shm_id_salir, *salir, com, op, t;
    struct msg *m;

    shm_id = shmget(IPC_PRIVATE, sizeof(struct msg), 0600);
    m = shmat(shm_id, 0, 0);
    shm_id_salir = shmget(IPC_PRIVATE, sizeof(int), 0600);
    salir = shmat(shm_id_salir, 0, 0);
    *salir = 0;

    int estado = 0; // inicialmente las compuertas estan cerradas
    m->COM = np; // para darle el primer turno al proceso root
    pid_t root =  getpid();
    printf("Para salir ingrese: 0 0 0\n");

    for (i=0; i<np; i++) if(!fork()) break;

    while (*salir == 0){
        if (i == m->COM){
            if (root  == getpid()){
                printf("\n\t****Servidor****\nMensaje: ");
                scanf("%d %d %d", &com, &op, &t);

                if (com == 0 && op == 0 && t == 0) *salir = 1;

                if ((com >= 1 && com <= 5) && (op == 0 || op == 1) &&
                    (t >= 0 && t <= 2) ){
                    m->OP = op;
                    m->Time = t;
                    printf("La compuerta %d se %s en %d segundos\n", com,
                    (m->OP? "abrira":"cerrara"), m->Time*5);
                    usleep(m->Time*5*1000000);
                    m->COM = com-1;
                }else{
                    if (com == 0 && op == 0 && t == 0)
                        printf("\tSaliendo...\n");
                    else
                        printf("\tError: Entrada incorrecta.\n");
                }
            }else{
                printf("\n\t----Cliente %d----\n", i+1);
                printf("Compuerta %d %s\n", i+1, (m->OP? "abierta":"cerrada"));
                estado = m->OP;
                m->COM = 5; // volvemos a darle turno al proceso root (servidor)
            }
        }
    }
    shmdt(m);
    shmdt(salir);
    if (i == np){
        sleep(1);
        shmctl(shm_id, IPC_RMID, 0);
        shmctl(shm_id_salir, IPC_RMID, 0);
    }

    return 0;
}
