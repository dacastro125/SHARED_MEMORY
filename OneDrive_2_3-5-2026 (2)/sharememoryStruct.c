#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

typedef struct Solucion{
    float promedio;
    float mayor;
    float menor;
    float suma2;
    float factorial;
    
}Solucion;


int main(){
    int vector[3]={1,2,3,};
    int shmid;
    Solucion *share_pointer;    // Instancia una estructura 
    shmid = shmget(IPC_PRIVATE,sizeof(Solucion),IPC_CREAT | 0600);
    if(shmid == -1){
        printf("Error en la creacion de la memoria compartida\n");
    }
    share_pointer = shmat(shmid,NULL,0); // anexar a la memoria compartida la estructura y asi todos los hijos podran utilizarla 
    if(share_pointer==(void *)-1){
        printf("Error al anexar la memoria compartida");
    }

    if(fork()==0){
        float suma = 0,promedio = 0;
        for (int i = 0; i < 3; i++)
        {
            suma+=vector[i];
        }
        promedio = suma/((float)3);
        share_pointer->promedio = promedio;
        shmdt(share_pointer);  
    }else if(fork()==0){
        int mayorE = vector[0];
        for (int i = 0; i < 3; i++)
        {
            if(mayorE<vector[i]){
                mayorE = vector[i];
            }
        }
        share_pointer->mayor = mayorE;
        shmdt(share_pointer);  
    }else if (fork()==0)
    {
        int menorE = vector[0];
        for (int i = 0; i < 3; i++)
        {
            if (menorE>vector[i])
            {
                menorE = vector[i];
            }   
        }
        share_pointer->menor = menorE;
        shmdt(share_pointer); 
    }else if(fork()==0){
        float suma2;
        for (int i = 0; i < 3; i++)
        {
            suma2+=vector[i];
        }
        share_pointer->suma2 = suma2;
        shmdt(share_pointer);
    }else if(fork()==0){
        float factorial = 1;
        for (int i = 0; i < 3; i++)
        {
            factorial = factorial * vector[i];
        }
        share_pointer->factorial = factorial;
        shmdt(share_pointer);
    }else{
        for (int i = 0; i < 5; i++)
        {
            wait(NULL);
        }

        printf("Hola soy el padre con PID %d\n",getpid());
        
        printf("El promedio es: %f\n",share_pointer->promedio);
        printf("El menor es: %f\n",share_pointer->menor);
        printf("El mayor es: %f\n",share_pointer->mayor);
        printf("La suma es: %f\n",share_pointer->suma2);
        printf("El factorial es: %f\n",share_pointer->factorial);

        shmctl(shmid,IPC_RMID,0);
    }
    return 0;
}