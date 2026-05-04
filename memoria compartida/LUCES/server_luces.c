#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <signal.h>
#define N_HIJOS 5

typedef struct Mensaje
{
	int sw_msj;
	int id;
	int tiempo;
	int off_on;
}Mensaje;

typedef struct Luz
{
	int sw;
	Mensaje mensaje[N_HIJOS];
	int salir;
}Luz;

int main(int argc, char const *argv[])
{
	int id_luz=-1;
	int time = 0;
	int op=-1;

	int padre = getpid();
	pid_t hijos[N_HIJOS];
	

	int shm_id;
	int shm_size = sizeof(Luz);
	Luz *shm_luz;

	int shm_idPID;
	int *shm_PID; 

	int key = ftok(".",2016);

	shm_id=shmget(key, shm_size, IPC_CREAT | 0666);
	shm_luz = shmat(shm_id,NULL,0);

	shm_idPID = shmget(key, shm_size, IPC_CREAT | 0666);
	shm_PID = shmat(shm_idPID, NULL, 0);
	shm_luz->salir=0;

	printf("SERVIDOR [%d]\n\n", getpid());
	for (int i = 0; i < N_HIJOS; i++)
	{
		hijos[i] = fork();
		if (hijos[i] == 0)
		{
			shm_luz->mensaje[i].sw_msj = 0;
			shm_luz->mensaje[i].id = i;
			shm_luz->mensaje[i].tiempo = 0;
			shm_luz->mensaje[i].off_on = -1;
			break;
		}
	}


	if (padre == getpid())
	{
		for (int i = 0; i < N_HIJOS; i++)
		{
			wait(NULL);
		}

		do
		{
			printf("[%d] Ingrese el ID de la luz: ", getpid());
			scanf("%d", shm_PID);

			if (*shm_PID>=0 && *shm_PID<=N_HIJOS)
			{
				printf("[%d] Ingrese el tiempo: ", getpid());
				scanf("%d", &time);	
				if (time>=0 && time <=3000000)
				{
					printf("[%d] Ingrese la operacion a realizar: (0)Apagar === (1)Encender: ", getpid());
					scanf("%d", &op);
					if (op>=0 && op<=1)
					{
						printf("[%d] Procesando la petición.....\n", getpid());

						if (shm_luz->mensaje[*shm_PID].off_on==op)
						{
							switch(op){
								case 0:
										printf("[%d] La luz ya se encuentra apagada...\n", getpid());
										break;
								case 1:
										printf("[%d] La luz ya se encuentra encendida...\n", getpid());
										break;
							}
						}else{
							
							shm_luz->mensaje[*shm_PID].tiempo = time;
							shm_luz->mensaje[*shm_PID].off_on = op;
							shm_luz->mensaje[*shm_PID].sw_msj = 1;
							shm_luz->sw= 1;
							usleep(time);
							printf("[%d] Operación exitosa.....\n", getpid());
						}
					}else{
						printf("[%d] Error...!\n", getpid());
						printf("[%d] Operacion invalida\n", getpid());	
					}
				}else{
					printf("[%d] Error...!\n", getpid());
					printf("[%d] Tiempo invalido\n", getpid());
				}
			}else{
				printf("[%d] Error...!\n", getpid());
				printf("[%d] ID invalido\n", getpid());
			}



			printf("[%d] Ingrese la tecla 1 para salir: ", getpid());
			scanf("%d", &shm_luz->salir);
			printf("\n");

		} while (shm_luz->salir!=1);

		shm_luz->sw = 1;

		//se desactivan todos los hijos
		for (int i = 0; i < N_HIJOS; ++i)
		{
			shm_luz->mensaje[*shm_PID].sw_msj = -1;
		}

		shm_luz->sw = 0;
		shmdt(shm_luz);
		shmdt(shm_PID);
		shmctl(shm_id, IPC_RMID, 0);
		shmctl(shm_idPID, IPC_RMID, 0);
		printf("[%d] FINALIZANDO.......... \n", getpid());
	}

	return 0;
}