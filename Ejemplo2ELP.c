#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 50

void Error (char *msj){
	printf("%s\n", msj);
	exit(1);
}

int main(void){
int shmid, pidhijo;
char *shm_pointer, buff[SIZE];

int key = ftok(".",2016);
shmid= shmget (key, SIZE, IPC_CREAT|0666);
if (shmid == -1) Error ("error creando shmget");
shm_pointer = shmat (shmid, NULL, 0);
if (shm_pointer == (void *)-1) Error ("adjuntando segmento shmat");

buff[0]='\0';
	do{
	scanf("%s", buff);
	strcpy(shm_pointer, buff);
	printf("%s\n", buff);
	}while (strcmp (buff, "salir"));

shmctl (shmid, IPC_RMID, NULL);
printf ("Finalizando...[%d]\n", getpid());
return 0;
}
