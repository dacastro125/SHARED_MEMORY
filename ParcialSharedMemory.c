#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

typedef struct estructura{
	int turno;
	
}turno;

int main(){

int i=0,shmid,pidhijo,turno2;

//Instancia la estructura
	turno *shm_pointer;

//creamos memoria compartida
	shmid = shmget(IPC_PRIVATE,50,IPC_CREAT|0666);
	if (shmid==-1)
	{
              printf("Error creando la memoria compartida");
        }

	shm_pointer=shmat(shmid,NULL,0);
	if (shm_pointer==(void*)-1)
        {
        	printf("Error al anexar la memoria compartida");	
        }

	for (i=1; i<=4; i++){
		if((pidhijo=fork())==0){
			
			break;	
		}
	}//fin for 


if (i==5){
	
	turno2=5;
	shm_pointer->turno=turno2;
	shmdt(shm_pointer);			
	printf("Principal tid [%d]\n",getppid());
	shmctl(shmid,IPC_RMID,NULL);					
	
}//fin padre


if (i==1){
	
	turno2=1;			
	printf("Proceso 1 tid  [%d]\n",getpid());
	shm_pointer->turno=turno2;
	shmdt(shm_pointer);

}//fin proceso 1

if (i==2){
	
	turno2=2;	
	printf("Proceso 2 tid  [%d]\n",getpid());
	shm_pointer->turno=turno2;
	shmdt(shm_pointer);

}//fin proceso 2

if (i==3){
	
	turno2=3;				
	printf("Proceso 3 tid  [%d]\n",getpid());
	shm_pointer->turno=turno2;
	shmdt(shm_pointer);
	
}//fin proceso 3

if (i==4){
	
	turno2=4;			
	printf("Proceso 4 tid  [%d]\n",getpid());
	shm_pointer->turno=turno2;
	shmdt(shm_pointer);
}//fin proceso 4   

while(shm_pointer->turno!=turno2){
printf("Proceso [%d] tid  [%d]\n",turno2,getpid());
shm_pointer->turno++;
shmdt(shm_pointer);
}



}//fin main 

