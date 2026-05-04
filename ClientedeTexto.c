#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#define SIZE 50

void Error (char *msj){
        printf("\n%s\n",msj);
        exit(1);
}

int  main()
{  
    int shmid,pidhijo;
    char *shm_pointer,buff[SIZE];
    int key=ftok(".",2016);
    shmid=shmget(key,SIZE,IPC_CREAT|0666);
    if (shmid==-1)
    {
    	Error("Error al crear o recuperando id del segmento de memoria compartida");
    }
    shm_pointer=shmat(shmid,NULL,0);
    if (shm_pointer==(void*)-1)
    {
    	Error("Error asignando segmento de la memoria compartida");

    }
    buff[0]='\0';
	do{
		if (strcmp(buff,shm_pointer))
        {
        strcpy(buff,shm_pointer);
        printf("%s\n",buff);
        }
        
	}while(strcmp(buff,"salir"));

   // desvincula
   shmdt(shm_pointer);

	return 0;
}