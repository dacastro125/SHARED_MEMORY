#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>

#define SIZE 50
 
 void Error(char *msj){
 	printf("%s\n",msj );
 	exit(1);
 }

 void handler(){}

 int main(void)
 {
 	int shmid, pidhijo;
 	char *shm_pointer, buff[SIZE];

 	int key = ftok(".", 2016);
 	shmid = shmget(key, SIZE, 0666);
 	if(shmid==-1)Error("Error recuperand shmget...");
 	shm_pointer = shmat(shmid, NULL, 0);
 	if (shm_pointer==(void *)-1)Error("Adjuntando Segmento shmat...");
 	buff[0]='\0';
 	do{
 		if (strcmp(buff,shm_pointer))
 		{
 			strcpy(buff,shm_pointer);
 			printf("%s\n",buff );
 		}
 		
 	} while (strcmp(buff,"salir"));

 shmdt(shm_pointer);
 printf("Finalizado cliente...%d\n",getpid());

 	return 0;
 }