#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>

typedef struct Solucion
{
    float promedio;
    float mayor;
    float menor;
    float suma2;
    float factorial;

}Solucion;


int main(){

    int vector[3] = {1,2,3};

    int shmid;

    Solucion *shm_pointer;

    shmid=shmget(IPC_PRIVATE, sizeof(Solucion),IPC_CREAT | 0600);

    if (shmid==-1)
    {
        printf("Error creando la memoria compartida");
    }
    
    shm_pointer=shmat(shmid,NULL,0);

    if (shm_pointer==(void*)-1)
    {
        printf("Error al anexar la memoria compartida");
    }

    if (fork()==0)
    {
        float suma,promedio;

        for (int i = 0; i < 3; i++)
        {
            suma=suma+vector[i];
        }
        
        promedio=suma/((float)3);

        shm_pointer->promedio=promedio;

        shmdt(shm_pointer);

    }else if (fork()==0)
    {
        int mayorE=vector[0];

        for (int i = 0; i < 3; i++)
        {
            if (mayorE<vector[i])
            {
                mayorE=vector[i];
            }

        shm_pointer->mayor=mayorE;
        
        shmdt(shm_pointer);

        }
        
    }else if (fork()==0)
    {
        int menorE = vector[0];

        for (int i = 0; i < 3; i++)
        {
            if (menorE>vector[i])
            {
                menorE=vector[i];
            }

        }

       shm_pointer-> menor=menorE;   

        shmdt(shm_pointer); 

    }else if (fork()==0)
    {
        float suma;

        for (int i = 0; i < 3; i++)
        {
            suma=suma+vector[i];
        
        }
        
        shm_pointer->suma2=suma;

        shmdt(shm_pointer);

    }else if (fork()==0)
    {
        
      float factorial=1;

      for (int i = 0; i < 3; i++)
      {
        
        factorial=factorial*vector[i];

      }
    
    shm_pointer->factorial=factorial;

    shmdt(shm_pointer);
      
    }else{

        for (int i = 0; i < 5; i++)
        {
            
            wait(NULL);

        }
        
    printf("Hola soy el padre\n");

    printf("El promedio es :\n");

    printf("\n%f\n",shm_pointer->promedio);
    printf("El mayor es :\n");

    printf("\n%f\n",shm_pointer->mayor);

    printf("El menor es :\n");

    printf("\n%f\n",shm_pointer->menor);

    printf("La suma de los vectores es :\n");

    printf("\n%f\n",shm_pointer->suma2);

    printf("El factorial es :\n");

    printf("\n%f\n",shm_pointer->factorial);

    shmctl(shmid,IPC_RMID,NULL);

    }
    while(1);
    return 0;
}