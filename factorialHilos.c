#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void* fact(void *);


int main(int argc, char const *argv[]){
int i, *param;
pthread_t hilo;
printf("Elija un numero:");
scanf("%d",&i);
 param=(int*)malloc(sizeof(int));
 *param=i;
pthread_create(&hilo, NULL, fact,(void*) param);

pthread_join(hilo,NULL);
	

	return 0;
} 

void* fact(void * arg){
int a= *(int*)arg;
int f=1, j;
	for(j=1; j<=a; j++){
	f*=j;	
	}
printf("El factorial es %d\n", f);
}