#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include <wait.h>

void Error (char*msj){ 	printf ("%s\n", msj); exit(1);}

int main(void){
	int shmid, pidhijo;
	char *shm_pointer;

shmid = shmget (IPC_PRIVATE, 50, IPC_CREAT|0666);
if (shmid==-1) Error ("error creando shmget\n");

shm_pointer = shmat(shmid, NULL, 0);
if (shm_pointer == (void*)-1)Error ("adjuntando segmento shmat");

if((pidhijo=fork())==0){
	usleep(10000);
	printf("[%d]leido: %s\n", getpid(), shm_pointer);
	shmdt (shm_pointer);
}else{
	strcpy(shm_pointer, "Hola memoria compartida");
	printf("[%d]Escrito: Hola memoria compartida\n", getpid());
	wait (NULL);
	shmctl(shmid, IPC_RMID, NULL);
} 
return 0;
}
