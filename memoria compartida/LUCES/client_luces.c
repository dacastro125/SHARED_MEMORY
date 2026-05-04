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

void manejador(int signal){}

void acciones(Luz *shm_luz, int i){
	
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
	printf("\n");

}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1,manejador); // se le asigna un manejador a sigusr1

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
	printf("CLIENTE [%d]\n\n", getpid());

	for (int i = 0; i < N_HIJOS; i++)
	{
		hijos[i] = fork();
		if (hijos[i] == 0)
		{
			pause();
			while(shm_luz->mensaje[i].sw_msj == 1){
				printf("Entro\n");
				acciones(shm_luz, i);
				shm_luz->mensaje[i].sw_msj = 0;
				shm_luz->sw = 0;
				kill(getppid(),SIGUSR1);
				pause();
			}

			break;
		}
	}

	if (getpid() == padre)
	{
		usleep(1000);
		do
		{	
			printf("%d\n", shm_luz->sw);
			while(shm_luz->sw != 1){}
			printf("%d\n", shm_luz->sw);
			if (shm_luz->salir != 1)
			{
				printf("ya%d\n", *shm_PID);
				kill(hijos[1], SIGUSR1);
				printf("ya%d\n", *shm_PID);
				pause();
				while(shm_luz->sw != 1){}
			}
		} while (shm_luz->sw == 1 && shm_luz->salir !=1);

		while(shm_luz->sw != 0){} // esperando a que se desactiven los hijos

		for (int i = 0; i < N_HIJOS; i++) // se espera a que todos los hijos terminen
		{
			kill(hijos[i], SIGUSR1);
			wait(NULL);
		}

		shmdt(shm_luz);
		shmdt(shm_PID);
		shmctl(shm_id, IPC_RMID, 0);
		shmctl(shm_idPID, IPC_RMID, 0);
		printf("[%d] FINALIZANDO.......... \n", getpid());
	}
	return 0;
}