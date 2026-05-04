#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

typedef struct Solucion{
    float promedio;
    float Mayor;
    float Menor;
    float suma2;    
    float factorial;

}Solucion;


int  main()
{
        int vector[3]={1,1,2};
        int shmid,pidhijo;
        //Instancia una estructura
        Solucion *shm_pointer;
        //crea la memoria compartida
        shmid=shmget(IPC_PRIVATE,sizeof(Solucion),IPC_CREAT|0666);
        if (shmid==-1){
              printf("Error creando la memoria compartida");
        }
        //anexar a la memoria compartida la estructura y asi todos los hijos podran utilizarla
        shm_pointer=shmat(shmid,NULL,0);
        
        if (shm_pointer==(void*)-1)
        {
            printf("Error al anexar la memoria compartida");    
        }

        if (fork()==0)
        {   
            float suma,promedio;
            for (int i = 0; i < 3; ++i)
            {
                suma=suma+vector[i];
            }
            promedio=suma/((float)3);
            shm_pointer->promedio=promedio;
            shmdt(shm_pointer);
        }else if (fork()==0)
        {


            int mayorE = vector[0];
            for (int i = 0; i < 3; ++i)
            {
                if(mayorE<vector[i]){
                    mayorE = vector[i];
                }
                
            }
        shm_pointer->Mayor=mayorE;  
        shmdt(shm_pointer);
        }else if (fork()==0)
        {
            int menorE = vector[0];
            for (int i = 0; i < 3; ++i)
            {
                if(menorE>vector[i]){
                    menorE = vector[i];
                }
                
            }
            shm_pointer->Menor=menorE;
            shmdt(shm_pointer);
        }

        else if (fork()==0)
        {
            float suma2;
            for (int i = 0; i < 3; ++i)
            {
                suma2=suma2+vector[i];
                
            }
            shm_pointer->suma2=suma2;
            shmdt(shm_pointer);
            

        }else if (fork()==0)
        {
            float factorial=1;
            for (int i = 0; i < 3; ++i)
            {
                factorial=factorial*vector[i];
                
            }
            shm_pointer->factorial=factorial;
            shmdt(shm_pointer);
            

        }


        else{
        wait(NULL);
        wait(NULL);
        wait(NULL); 
        wait(NULL); 
        wait(NULL); 
        

        printf("hola soy el padre\n");  
        printf("El promedio es : \n");  

        printf("\n%f\n",shm_pointer->promedio); 
        printf("El mayor es : \n"); 

        printf("\n%f\n", shm_pointer->Mayor);
        
        printf("El menor es :\n");  

        printf("\n%f\n", shm_pointer->Menor);
        
        shmctl(shmid,IPC_RMID,NULL);

        printf("La suma de los vector es  :\n");    

        printf("\n%f\n", shm_pointer->suma2);
        
        shmctl(shmid,IPC_RMID,NULL);
         
        printf("El factorial es :\n");    

        printf("\n%f\n", shm_pointer->factorial);
        
        shmctl(shmid,IPC_RMID,NULL);
         

        }

        


    return 0;
}
