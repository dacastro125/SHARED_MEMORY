#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *funcion_maneja_hilo(void *);
typedef unsigned long int tipo_hilo;

int *vector,tama,n,i;

int main()
{
    pthread_t pidhilo[3];
    printf("Ingrese el tamaño del vector ");
    scanf("%d",&tama);
    vector=(int*)malloc(sizeof(int)*tama);
    
    printf("tamaño %d \n",tama );
    for (i = 0; i < tama; ++i)
    {
        printf("vector[%d]=",i);
        scanf("%d",&vector[i]);
    }
    /*for (i = 0; i < tama; ++i)
    {
        printf("vector[%d]=%d",i,vector[i]);
        
    } */

    int *d;
    for ( i = 0; i < 3; i++)
    {
        d=(int*)malloc(sizeof(int));
        *d=i+1;
        pthread_create(&pidhilo[i], NULL, funcion_maneja_hilo, (void*)d);
    }

    for(i=0;i<3;i++){
         
    pthread_join(pidhilo[i],NULL);
    }
    
       
    
    return 0;
}



void *funcion_maneja_hilo(void *param)
{
        int h=*(int*)param;
        if (h==1)
        {
            int q=0;
            for ( i = 0; i <tama ; i++)
            {
                if (q<vector[i])
                {
                    q=vector[i];
                }
            }
            printf("Mayor:%d",q);
        }else if (h==2)
        {
          int r=vector[0];
            for ( i = 0; i <tama ; i++)
            {
                if (r>vector[i])
                {
                    r=vector[i];
                }
            }
            printf("Menor:%d",r);
        }else{
         int suma=0;
         for ( i = 0; i < tama; i++)
         {
             suma=suma+vector[i];
         }
         int prom=suma/tama;
         printf("el promedio es:%d",prom );
        }

  
	pthread_exit(0);
}