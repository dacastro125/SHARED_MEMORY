#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void * thread_handler(void*);


typedef struct Data{
    int num; //tamano del vector principal.
    int tc; // numero del hilo.
}Data;

int main() {
    srand(time(NULL));
    int i,k;
    int nt, np, num;

    printf("Ingrese el número de puntos y el numero de hilos:");
    scanf("%d%d", &np, &nt);
    num=np/nt;
    int Circle_Count=0;
    double PI = 0.0;
    
    Data ** vector_data= (Data **) calloc(nt, sizeof(Data*));
    pthread_t* vpt = (pthread_t *) calloc(nt, sizeof(pthread_t));
    
    for(i=0; i<nt; i++){
         vector_data[i]= (Data * ) calloc(nt, sizeof(Data));
         vector_data[i]->num=num;
	 vector_data[i]->tc=0;
         pthread_create(&vpt[i],NULL, thread_handler, vector_data[i]);
	
    }
    
    for(k=0; k<nt; k++){
        pthread_join(vpt[k], NULL);
        
        Circle_Count+=vector_data[k]->tc;
    }
    
    
    PI = (4.0 * Circle_Count) / (double) np;
    printf("\nPI=%.5lf\n", PI);
    //free(vpt);
    return 0;
}



void * thread_handler(void * param){
    double x,y;
    int num = ((Data*) param)->num;
    int tc=0;
    int i; 
    for(i=0; i<num; i++){
        x = (double) rand()/ (double) RAND_MAX;
        y = (double) rand()/ (double) RAND_MAX;
        if((x*x+y*y)<=1.0){ 
            tc++;
        }
    }
    printf("-%d-",  tc);
    ((Data *) param)-> tc = tc;
    pthread_exit(0);
    
}
