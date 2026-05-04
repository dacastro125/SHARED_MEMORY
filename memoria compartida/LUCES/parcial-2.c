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
	int pid_h;
	int sw_msj;
	int id;
	int tiempo;
	int off_on;
	int off_on_ant;
}Mensaje;

typedef struct Luz
{
	Mensaje mensaje[N_HIJOS];
	int salir;
}Luz;

void manejador(int signal){}

void acciones(Luz *shm_luz, int i){
	
	if (shm_luz->mensaje[i].off_on != shm_luz->mensaje[i].off_on_ant )
	{
		if (shm_luz->mensaje[i].off_on == 1)
		{
			printf("[%d] La Luz numero %d se encendera en %d nanosegundos... \n",getpid(), i, shm_luz->mensaje[i].tiempo);
			usleep(shm_luz->mensaje[i].tiempo);
			printf("[%d] La Luz numero %d se acaba de encender...\n",getpid(), i);
		}else{
			printf("[%d] La Luz numero %d se apagara en %d nanosegundos... \n",getpid(), i, shm_luz->mensaje[i].tiempo);
			usleep(shm_luz->mensaje[i].tiempo);
			printf("[%d] La Luz numero %d se acaba de apagar...\n",getpid(), i);
		}
	}else{
		if (shm_luz->mensaje[i].off_on == 1)
		{
			printf("[%d] La Luz numero %d ya esta encendida \n",getpid(), i);
		}else{
			printf("[%d] La Luz numero %d ya esta apagada \n",getpid(), i);
		}
	}

	printf("\n");
}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1, manejador);
	pid_t hijos[N_HIJOS];
	int padre=getpid();

	int shm_id;
	int shm_size = sizeof(Luz);
	Luz *shm_luz;

	shm_id = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0600);
	shm_luz = shmat(shm_id, NULL, 0);

	shm_luz->salir = 0;

	int i=0;
	int id_luz=0;
	int time=0;
	int estado=0;

	for(i=0;i<N_HIJOS;i++){
		hijos[i] = fork();
		if (hijos[i] == 0)
		{
			shm_luz->mensaje[i].pid_h = getpid();
			shm_luz->mensaje[i].sw_msj = 0;
			shm_luz->mensaje[i].id=i;
			shm_luz->mensaje[i].tiempo = 0;
			shm_luz->mensaje[i].off_on = -1;
			pause();
			
			while(shm_luz->mensaje[i].sw_msj == 1){
				acciones(shm_luz,i);
				shm_luz->mensaje[i].sw_msj = 0;
				kill(getppid(),SIGUSR1);
				pause();
			}
			printf("[%d] FINALIZANDO CLIENTE.......... \n", getpid());
			shmdt(shm_luz);
			break;
		}
	}

	if (padre == getpid())
	{
		usleep(1000);

		do
		{
			printf("[%d] Ingrese el ID de la luz: ", getpid());
			scanf("%d", &id_luz);
			if (id_luz>=0 && id_luz<N_HIJOS)
			{
				printf("[%d] Ingrese el tiempo: ", getpid());
				scanf("%d", &time);	
				if (time>=0 && time <=3000000)
				{
					printf("[%d] Ingrese la operacion a realizar: (0)Apagar === (1)Encender: ", getpid());
					scanf("%d", &estado);
					if (estado>=0 && estado<=1)
					{
						
						shm_luz->mensaje[id_luz].tiempo = time;
						shm_luz->mensaje[id_luz].off_on_ant = shm_luz->mensaje[id_luz].off_on;
						shm_luz->mensaje[id_luz].off_on = estado;
						shm_luz->mensaje[id_luz].sw_msj = 1;
						kill(hijos[id_luz], SIGUSR1);
						pause();
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

		// desactivamos a los hijos
		for(i=0;i<N_HIJOS;i++){
			shm_luz->mensaje[i].sw_msj = -1;
			kill(hijos[i], SIGUSR1);
			wait(NULL);
		}

		printf("[%d] FINALIZANDO SERVER.......... \n", getpid());
		shmdt(shm_luz);
		shmctl(shm_id,IPC_RMID,0);


	}

	return 0;
}