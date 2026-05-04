#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#define SIZE 50


int main(int argc, char const *argv[])
{
	pid_t pidhijo;
	int shm_id;
	int shm_size = SIZE;
	char *shm_msj;
	char msj[SIZE];
	int key = ftok(".", 2016);

	shm_id = shmget(key,shm_size,IPC_CREAT | 0666);
	shm_msj = shmat(shm_id, NULL, 0);

	msj[0] = '\0';
	
	pidhijo = fork();

	if (pidhijo == 0) //hijo
	{	
		do{
			if (strcmp(msj,shm_msj))
			{
				strcpy(msj,shm_msj);
				printf("[%d] %s\n", getpid(), msj);
			}
		}while(strcmp(msj,"salir"));
		shmdt(shm_msj);
	}else{


		do{
			scanf("%s", msj);
			strcpy(shm_msj, msj);
		}while(strcmp(msj,"salir"));

		wait(NULL);


		
		
		shmdt(shm_msj);
		shmctl(shm_id, IPC_RMID, 0);
	}


	return 0;
}