#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#define SIZE 50

void Error (char *msj){
	printf("%s\n", msj);
	exit(1);
}

int main(void){
	int shmid, pidhijo;
	char *shm_pointer, buff[SIZE];

int key = ftok(".", 2016);
shmid = shmget (key, SIZE, 0666);
if (shmid == -1 ) Error ("error recuperando shmget");
shm_pointer = shmat (shmid, NULL, 0);
if (shm_pointer == (void *)-1) Error ("adjuntando segmento shmat");

buff[0] ='\0';
	do{
		if (strcmp(buff, shm_pointer)){
			strcpy(buff, shm_pointer);
			printf ("%s\n", buff);
		}
	}while (strcmp(shm_pointer,"salir"));

shmdt(shm_pointer);
return 0;
}
