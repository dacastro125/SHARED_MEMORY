#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>



void Error (char *msj){
        printf("\n%s\n",msj);
}


int main(){
        int shmid,pidhijo;
        char *shm_pointer;
        
        shmid=shmget(IPC_PRIVATE,50,IPC_CREAT|0666);
        if (shmid==-1){
        Error("Error creando la memoria compartida");
        }

        shm_pointer=shmat(shmid,NULL,0);
        if (shm_pointer==(void*)-1)
        {
        Error("Error al anexar la memoria compartida");	
        }

        if ((pidhijo=fork())==0)
        {
        	usleep(1000);
        	printf("[%d]Leido:%s\n",getpid(),shm_pointer );
            shmdt(shm_pointer);
        }else{

        	strcpy(shm_pointer,"hola memoria compartida");
        	printf("\n[%d]Escrito: hola memoria compartida:",getpid());
            wait(NULL);
            shmctl(shmid,IPC_RMID,NULL);
        }

//int  shmget(key_t,size_t size,int shmflg);
//printf("\nentre");


	return 0;
}
