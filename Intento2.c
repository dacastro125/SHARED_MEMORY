#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdio.h>
#include <wait.h>

int shm_idptr, i, num;

typedef struct Ed {
    int id_luz;
    int tiempo;
    int estado;
    int sw;
    int salir;
}
Ed;

int main() {
    printf("------------------\n");

    Ed *ptr;

    shm_idptr = shmget(IPC_PRIVATE, sizeof(Ed), IPC_CREAT | 0666);
    ptr = shmat(shm_idptr, NULL, 0);

    printf("Ingrese el numero de luces: ");
    scanf("%d", &num);

    ptr->id_luz = -1;
    ptr->tiempo = 0;
    ptr->estado = -1;
    ptr->salir = 0;
    ptr->sw = 0;

    int padre = getpid();

    int h[num];

    printf("Soy el proceso [%d] \n", getpid());

    for (i = 0; i < num; i++) {
        h[i] = fork();
        
        if (h[i] == 0) {
            int id = i;

           while(ptr->salir == 0) {
                while(id != ptr->id_luz && ptr->tiempo == 0 && ptr->estado == -1) {}
		
		printf("Soy hijo %d", ptr->id_luz);

                ptr->id_luz = -1;
                ptr->tiempo = 0;
                ptr->estado = -1;
                ptr->sw = 1;
            }

            printf("\nCerrando hijo %d...\n", i);

            break;
        }
    }

    if (padre == getpid()) {
        while (ptr->salir == 0) {
            printf("Ingrese el id del led: ");
            scanf("%d", & ptr-> id_luz);

	    if(ptr-> id_luz<num){
            printf("Ingrese el tiempo: ");
            scanf("%d", & ptr-> tiempo);

            printf("Ingrese el estado del led: ");
            scanf("%d", & ptr->estado);

            printf("\n");
            while (ptr->sw != 1) {}

            printf("Desea enviar otro mensaje (0) Si (1) No: ");
            scanf("%d", &ptr->salir);

            ptr->sw = 0;
	    }
        }

        printf("Cerrado\n");
    }

    return 0;
}
