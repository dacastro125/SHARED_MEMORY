#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>





void *funcion_imp( void *arg);

int main () {

     pthread_t hilos[20];
     

     
     int n,i; 
     
     printf("digite la cantida de hilos\n");
     scanf("%d",&n);
     
     for(i=0;i<n;i++){ 
     
       pthread_create(&hilos[i],NULL,&funcion_imp,(void *)i);
     }
     
        
       for(i=0;i<n;i++){
          pthread_join(hilos[i],NULL);
       }
       
       
return 0;

}
    
void *funcion_imp(void *arg){

int turno = (int)arg;
printf("mi turno es [%d] y mi id es[%u]", turno,(unsigned int )pthread_self());
pthread_exit(0);
 
     	

}
