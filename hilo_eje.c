#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int turno=0,s,r,m;
void * funcion_manejadore(void *);
	struct nodo
	{
		int a,b;
	};
int main(){
int i;
pthread_t idhilo[3];
struct nodo *N;
N=(struct nodo*)malloc(sizeof(struct nodo));
	printf("Digite A\n");
	scanf("%d",&N->a);
	printf("Digite B\n");
	scanf("%d",&N->b);
for(i=0;i<3;i++){
	
	turno++;
	pthread_create(&idhilo[i],NULL,funcion_manejadore,(void*)N);

	}
}
void* funcion_manejadore(void *param){
	if(turno==1){
		s=((struct nodo *)param)->a+((struct nodo *)param)->b;

	}else if(turno==2){
		r=((struct nodo *)param)->a-((struct nodo *)param)->b;
	}else{
		m=s*r;
		printf("Resultado %d \n",m);
	}





}
