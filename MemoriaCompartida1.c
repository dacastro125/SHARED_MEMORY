#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <signal.h>

void Error(char *msj){// solo se define si falla añadir el segmento u obtenerlo 
	printf("asdf:%s\n",msj );
}

void manejador(int sig){}

int main(int argc, char const *argv[])
{
	int shmid, pidhijo;//entero 
	char *shm_pointer;//puntero a char, depende de lo que se comparta 
	//si se le pone IPC_PRIVATE es privada, hace que la memoria solo se comparta en procesos relacionados (creados con fork)
	shmid = shmget(IPC_PRIVATE, 50, IPC_CREAT | 0666);//shmid= id de la memoria, ipc_private: clave, funcion: ftok("cadena", int);
	// shmid recibe , comunicaicon privada, de 50 bytes, 666 read y write para todos(propietario, grupo, cualquiera)
	if (shmid == -1) Error("shmget");// falla en obtener memoria
	shm_pointer = shmat(shmid, NULL, 0);// creada la memoria, apunta a ella, con shm_pionter
	
	if(shm_pointer == (void*)-1) Error("shmat");
	pidhijo = fork();//segmentos de memoria compartida heredan descriptores abiertos

	if (pidhijo == 0){
		signal(SIGUSR1, manejador);// para evitar que el hijo lea antes de que el padre escriba, llama al padre y el escribe
		pause();// se pausa para dar llegada al padre
		printf("LEIDO:%s\n", shm_pointer);// se lee cadena y se almacena en la direccion de memoria  de ese puntero
		shmdt(&shmid);// se quita de la memoria compartida
	}else{
		scanf("%s", shm_pointer);// el padre escribe, después de que el hijo existe
		kill(pidhijo, SIGUSR1);//despierta al hijo
		wait(NULL);//espera al hijo
		shmctl(shmid, IPC_RMID, NULL);//borra memoria compartida
	}

	return 0;
}