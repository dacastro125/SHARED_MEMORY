#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#define N 100

typedef struct Solucion{
	float promedio;
    int vector[N];
	int tama;
	int sw;
}Solucion;


int  main()
{
	    int shmid,pidhijo;
        //Instancia una estructura
		Solucion *shm_pointer;
        //crea la memoria compartida
        shmid=shmget(IPC_PRIVATE,sizeof(Solucion),IPC_CREAT|0666);
        if (shmid==-1){
              printf("Error creando la memoria compartida");
        }
        //anexar a la memoria compartida la estructura y asi todos los hijos podran utilizarla
		shm_pointer = shmat(shmid,NULL,0);
		
        if (shm_pointer==(void*)-1)
        {
        	printf("Error al anexar la memoria compartida");	
        }
        
        

        if (fork()==0)
        {   
        	float suma,promedio;
        	//while( shm_pointer->tama==0 ){} // no hacer nada
        	while( shm_pointer->sw==0 ){}
        	for (int i = 0; i < shm_pointer->tama; ++i)
        	{
        		suma=suma+shm_pointer->vector[i];
        	}
        	promedio=suma/((float)shm_pointer->tama);
        	shm_pointer->promedio=promedio;
            shmdt(shm_pointer);
            
        }else{
            
            int i,n;
            
            printf("ingrese el tamaño del vector .: ");
            scanf("%d",&n);
            shm_pointer->tama = n;  
            
            
            for( i=0; i<n; i++ ){ 
                printf("v[%d] .: ",i); 
                scanf("%d",&shm_pointer->vector[i]);
            }
              shm_pointer->sw=1;
        //    shm_pointer->tama = n;
			
	        wait(NULL);
	        
	        printf("hola soy el padre\n");	
	        printf("\n%f\n",shm_pointer->promedio); 
	        shmctl(shmid,IPC_RMID,NULL);
         

        }

        


	return 0;
}



