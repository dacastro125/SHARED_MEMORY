#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define NB 4   //Numero de busters
#define TL 60 //Tiempo que dura el lanzamiento

void lanzar_capsula(void);
void abortar(void);
void exito(void);
void variar_temperatura(void);

void* subproc1(void *);
void* subproc2(void *);

int temp_buster[NB];
int alta_temp = 0;    // comunica cuando hay alta temperatura
int verificando = 1; // es 0 cuando se pasen los TL segundos

int main(){
	pthread_t pidproc1, pidproc2;

	pthread_create(&pidproc1, NULL, subproc1, NULL);
	pthread_create(&pidproc2, NULL, subproc2, NULL);

	time_t inicio, final;
	inicio = time(NULL);
	final = inicio;

	do{
		printf("[%lu]", (final - inicio) + 1);
		final = time(NULL);
	}while((final - inicio) < TL && !alta_temp);

	verificando = 0;

	pthread_join(pidproc1, NULL);
	pthread_join(pidproc2, NULL);

	return 0;
}

void* subproc1(void * param){
	int i;
	while(!alta_temp && verificando){
		variar_temperatura();
		for (i = 0; i < NB && !alta_temp; i++){
			if(temp_buster[i] > 5){
				alta_temp = 1;
			}
		}
	}
	pthread_exit(0);
}

void* subproc2(void * param){
	while(verificando){
		if(alta_temp){
			abortar();
			break;
		}
	}

	if(!alta_temp){
		exito();
	}

	pthread_exit(0);
}

void variar_temperatura(){
	int i;
	srand(time(NULL));
	for (i = 0; i < NB; i++){
		temp_buster[i] = rand()%10;
	}
}

void lanzar_capsula(){
	printf("[CAPSULA LANZADA]\n");
}

void abortar(){
	printf("\n\n\n[PROTOCOLO DE ABORTO]\n");
	lanzar_capsula();
}

void exito(){
	printf("\n\n\n[LANZAMIENTO EXITOSO]\n");
	lanzar_capsula();
}
