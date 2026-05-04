#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <signal.h>
#define SIZE 50
#define N_HIJOS 5

typedef struct Content
{	
	int pid;
	int COM; // identificador de la compuerta
	int OP; // operacion que se puede realizar / cerrar=0; abrir=1
	int Time; // INMEDIATAMENTE = 0; ECORTA = 1; ELARGA =2
			// 0, 5, 10 SEGUNDOS RESPECTIVAMENTE
	int sw; // sw de control para cada hijo
}Content;

typedef struct Compuerta
{
	Content Content[N_HIJOS];	
	int sw_c; // sw de control general
	int salir; // opcion para salir
}Compuerta;

int main(int argc, char const *argv[])
{

	int op=0; // variable para la opcion a ingresar
	int time=0; // variable para el tiempo
	
	pid_t hijos[N_HIJOS]; // vector de hijos
	int shm_id; // id del segmento de memoria a compartir
	int shm_size = SIZE; // tamaño
	int padre = getpid(); // piud del padre
	Compuerta *shm_comp; // segmento de memoria a compartir
	int *pid_c;			// segmento de memoria a compartir
	int shm_idpid_c;
	int key = ftok(".", 2016);

	shm_id = shmget(key,shm_size,IPC_CREAT | 0666);
	shm_idpid_c = shmget(key,shm_size,IPC_CREAT | 0666);
	shm_comp = shmat(shm_id, NULL, 0);
	pid_c = shmat(shm_idpid_c, NULL,0);

	shm_comp->salir = 0;

	// se crean los hijos
	printf("SERVIDOR [%d]\n\n", getpid());
	for (int i = 0; i < N_HIJOS; i++)
	{
		hijos[i] = fork(); 
		if (hijos[i] == 0)
		{
			shm_comp->Content[i].pid= getpid();
			shm_comp->Content[i].COM= i;
			shm_comp->Content[i].OP= 0; // todas las compuertas estan cerradas
			shm_comp->Content[i].Time = 0;
			shm_comp->Content[i].sw = 0;
			shm_comp->sw_c = 0;
			break;
		}
	}

	// si es el padre
	if (getpid() == padre)
	{
		// Espera a que se creen todos los hijos
		for (int i = 0; i < N_HIJOS; i++)
		{
			wait(NULL);
		}

		do{
			printf("Ingrese el id de la compuerta: ");
			scanf("%d", pid_c);
			if (*pid_c>= 0 && *pid_c<=4)
			{
				printf("Ingrese la operacion a realizar: (0)Cerrar === (1)Abrir: ");
				scanf("%d", &op);
				if (op>= 0 && op<=1)
				{
					printf("Ingrese el tiempo: ");
					scanf("%d", &time);

					if (time>=0 && time<=2)
					{
						printf("Procesando la petición.....\n");
						if (shm_comp->Content[*pid_c].OP==op)
						{
							switch(op){
								case 0:
										printf("La compuerta ya se encuentra cerrada...\n");
										break;
								case 1:
										printf("La compuerta ya se encuentra abierta...\n");
										break;
							}
						}else{

						shm_comp->Content[*pid_c].OP= op;
						shm_comp->Content[*pid_c].Time = time;
						shm_comp->Content[*pid_c].sw = 1; // se activa el sw del hijo con id *pid_c para realizar operaciones
						shm_comp->sw_c = 1;
						
						
						switch(time){
								case 0:
										//usleep(0*1000000);
								break;
										
								case 1:
										usleep(1*1000000);
								break;
									
								case 2:
										usleep(5*1000000);
								break;
						}
						printf("Operación exitosa.....\n");
						}

						
					}else{
						printf("Tiempo Invalido...!\n");
						
					}
				}else{
					printf("Opción Invalida...!\n");
				}
				
			}else{
				printf("compuerta Invalida...!\n");
				
			}

			
			printf("Para salir presiona 1: ");
			scanf("%d", &shm_comp->salir);
			printf("\n");
		}while(shm_comp->salir != 1);

		//se activa el sw general para que el cliente espere hasta que se descativen los hijos
		shm_comp->sw_c = 1;

		// se desactivan los hijos
	for (int i = 0; i < N_HIJOS; i++)
	{
		shm_comp->Content[i].sw = -1;
	}
		shm_comp->sw_c = 0; // se activa el sw general para que el cliente pueda terminar

		shmdt(shm_comp);
		shmdt(pid_c);
		shmctl(shm_id, IPC_RMID, 0);
		shmctl(shm_idpid_c, IPC_RMID, 0);
		printf("[%d] FINALIZANDO.......... \n", getpid());
}	
	
	
		
	return 0;
}