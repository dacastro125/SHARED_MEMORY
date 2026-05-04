#include<stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/wait.h>

//si es 0 inmediato si 1 esperar 5 seg si es 2 esperar 5 seg


typedef struct Msj{
int com;
int op;
int tim;
int Sw;
}Msj;


int main(){
	
	
	
	int shmid,i=0,j=0;
         int pidhijo;
         Msj *shm_pointer;
        
        shmid = shmget(IPC_PRIVATE, sizeof(Msj), IPC_CREAT|0666);
        if(shmid == -1) printf("error creando shmget\n");
        
        shm_pointer = shmat(shmid, NULL, 0);
        if(shm_pointer == (void *)-1)printf("adjuntando segmento shmat");
	
	  if(fork()==0){
	  //h1
	  while(shm_pointer->com!=1){}
	  if(shm_pointer->op==1){
	  if(shm_pointer->tim==0){
	  	printf("Abrir");
	  }else if(shm_pointer->tim==1){
	   usleep(5*1000000);
	  printf("Abrir");
	  }else if(shm_pointer->tim=2){
	  	usleep(10*1000000);
	  printf("Abrir");
	  }
	  	 
	  } else{
	   if(shm_pointer->tim==0){
	  	printf("Cerrar");
	  }else if(shm_pointer->tim==1){
	   usleep(5*1000000);
	  printf("Cerrar");
	  }else if(shm_pointer->tim=2){
	  	usleep(10*1000000);
	   printf("Cerrar");
	  }
	  }
	  
	 
	 
	 
	  }else if(fork()==0){
	  //h2
	  while(shm_pointer->com!=2){}
	  
	  
	  
	  
	  
	  }else if(fork()==0){
	  //h3
	  while(shm_pointer->com!=3 ){}
	  
	 
	 
	  }else if(fork()==0){
	  //h4
	  while(shm_pointer->com!=4 ){}
	  
	 
	 
	  }else if(fork()==0){
	  	//h5
	  while(shm_pointer->com!=5 ) {}
	  
	  	
	  }else{
	  	//padre
	  	printf("ingrese operacion");
	  	scanf("%d",&shm_pointer->op);
	  	printf("\nIngrese tiempo de la operacion");
	  	scanf("%d",&shm_pointer->tim);
	  	printf("\nIngrese compuerta\n");
	  	scanf("%d",&shm_pointer->com);
	  	
	  
	  	
	  }
	  
	   
	
	return 0;
} 