#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <signal.h>
#define N_HIJOS 5

typedef struct Content
{	
	int pid;
	int COM; // identificador de la compuerta
	int OP; // operacion que se puede realizar / cerrar=0; abrir=1
	int Time; // INMEDIATAMENTE = 0; ECORTA = 1; ELARGA =2
			// 0, 5, 10 SEGUNDOS RESPECTIVAMENTE
	int sw;
}Content;


typedef struct Compuerta
{
	Content Content[N_HIJOS];	
	int sw_c;
	int salir;
}Compuerta;

void manejador(int sig){}

//funcion para realizar operaciones

void acciones(Compuerta *shm_comp, int i){
	int tempo =0;
							if (shm_comp->Content[i].OP == 1)
							{	
								if (shm_comp->Content[i].Time == 0)
								{
									printf("[%d] La compuerta numero %d se abrira INMEDIATAMENTE... \n",getpid(), i);
									printf("[%d] La compuerta numero %d se acaba de abrir...\n",getpid(), i);
								}else if (shm_comp->Content[i].Time == 1)
								{
									printf("[%d] La compuerta numero %d se abrira en 1 segundo... \n",getpid(), i);

									usleep(1*1000000);
									printf("[%d] La compuerta numero %d se acaba de abrir...\n",getpid(), i);
								}else if (shm_comp->Content[i].Time == 2)
								{	
									printf("[%d] La compuerta numero %d se abrira en 5 segundos... \n",getpid(), i);
									for (int i = 5; i > tempo; i--)
									{
										printf("%d\n", i);
										usleep(1*1000000);
									}
									printf("[%d] La compuerta numero %d se acaba de abrir...\n",getpid(), i);
								}
							}else{
								if (shm_comp->Content[i].Time == 0)
								{
									printf("[%d] La compuerta numero %d se cerrara INMEDIATAMENTE... \n",getpid(), i);
									
									printf("[%d] La compuerta numero %d se acaba de cerrar...\n",getpid(), i);
								}else if (shm_comp->Content[i].Time == 1)
								{
									printf("[%d] La compuerta numero %d se cerrara en 1 segundo... \n",getpid(), i);
									usleep(1*1000000);
									printf("[%d] La compuerta numero %d se acaba de cerrar...\n",getpid(), i);
								}else if (shm_comp->Content[i].Time == 2)
								{
									printf("[%d] La compuerta numero %d se cerrara en 5 segundos... \n",getpid(), i);
									for (int i = 5; i > tempo; i--)
									{
										printf("%d\n", i);
										usleep(1*1000000);
									}
									printf("[%d] La compuerta numero %d se acaba de cerrar...\n",getpid(), i);
								}
							}
							printf("\n");

}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1,manejador); // se le asigna un manejador a sigusr1

	pid_t hijos[N_HIJOS];
	int shm_id;
	int shm_size = sizeof(Compuerta);
	int padre = getpid();
	Compuerta *shm_comp;
	int *pid_c;
	int shm_idpid_c;
	int key = ftok(".", 2016);

	shm_id = shmget(key,shm_size,IPC_CREAT | 0666);
	shm_idpid_c = shmget(key,shm_size,IPC_CREAT | 0666);
	shm_comp = shmat(shm_id, NULL, 0);
	pid_c = shmat(shm_idpid_c, NULL,0);

	//shm_comp->salir = 0;

	// se crean los hijos y se pausan, estando a la espera de una señal para realizar su respectiva operacion
	printf("CLIENTE [%d]\n\n", getpid());
	for (int i = 0; i < N_HIJOS; i++)
	{
		hijos[i] = fork(); 
		if (hijos[i] == 0)
		{	
			pause();								
			while(shm_comp->Content[i].sw == 1){ // ciclo para realizar las operaciones correspondientes
				
				acciones(shm_comp,i);	//realiza las acciones indicadas
				shm_comp->Content[i].sw = 0; // se desactiva el sw para decir que ya realizo su operacion
				shm_comp->sw_c = 0; // se desactiva el sw general y se manda una señal al padre 
				kill(getppid(), SIGUSR1);
				pause(); // se pausa a la espera de otra operacion o terminación
			}
			break;
		}
	}

	if (getpid() == padre)
	{
			usleep(1000);
			

			//if (shm_comp->salir == 0)
			
				do{
					while(shm_comp->sw_c != 1){} // ciclo para esperar a que el usuario ingrese la operacion
					if (shm_comp->salir != 1)
					{
						kill(hijos[*pid_c], SIGUSR1); // se manda la señal al hijo para que realice la operación
						pause();						// se pausa a la espera de una respuesta del hijo
						while(shm_comp->sw_c != 1){}
					}
					
					
				}while(shm_comp->sw_c == 1 && shm_comp->salir != 1);
			
			
			
		
		while(shm_comp->sw_c != 0){} // esperando a que se desactiven los hijos
		
		for (int i = 0; i < N_HIJOS; i++) // se espera a que todos los hijos terminen
		{
			kill(hijos[i], SIGUSR1);
			wait(NULL);
		}
		
	
		shmdt(shm_comp);
		shmdt(pid_c);
		shmctl(shm_id, IPC_RMID, 0);
		shmctl(shm_idpid_c, IPC_RMID, 0);
		printf("[%d] FINALIZANDO.......... \n", getpid());
	}

	

	return 0;
}