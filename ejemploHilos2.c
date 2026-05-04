#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t myCond = PTHREAD_COND_INITIALIZER;

typedef unsigned long int tipo_hilo;

void* funcionHilo(void *);
char palabra [100];

int main(){
	pthread_t pidhilo;
	//pthread_t *pidhilo=NULL;
	palabra[0]='\0';

	printf("Iniciando programa....\n");
	printf("Ingresa una palabra: ");
	scanf("%s",palabra);
	
	//pidhilo= (pthread_t *)calloc(1,sizeof(pthread_t));
	pthread_create(&pidhilo, NULL, funcionHilo,(void*)palabra);
	pthread_join(pidhilo,NULL);
	//free(pidhilo);


   return 0;
}

void* funcionHilo(void *param){
	char requestPalabra[100];
	printf("imprimiendo palabra desde el hilo:%s\n\n",(char *)param);
	int a=0, e=0,i=0,o=0,u=0;
	int indice=0;
	strcpy(requestPalabra, (char *)param);
	pthread_mutex_lock(&myMutex);
	for(indice=0; indice<strlen(requestPalabra);indice++){
		switch(requestPalabra[indice]){
			case 'a':
				a++;
				break;
			case 'e':
				e++;
				break;
			case 'i':
				i++;
				break;
			case 'o':
				o++;
				break;
			case 'u':
				u++;
				break;
			default:
				break;
		}
	}
	pthread_mutex_unlock(&myMutex);
	printf("La cantidad de vocales son:\n");
	printf("a:%d, e:%d, i:%d, o:%d, u:%d\n",a,e,i,o,u);
	usleep(100);
	pthread_exit(0);

}



