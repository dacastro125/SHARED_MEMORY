#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#define N 5


void *manejador(void *);
pthread_t hilos[N];
int*  numeros;
float media;
float varianza;
int mayor;
int menor;
char nombreArch[20];
int n = 0;



int main(int argc, char const *argv[]){
	int i=0;
	int* indice;

	for (i = 0; i<N; i++){
		indice = (int*)malloc(sizeof(int));
		*indice = i;
		pthread_create(&hilos[i],NULL,manejador, (void*)indice);
	}

	for(i = 0; i<N; i++){
		pthread_join(hilos[i],NULL);
	}

	if (n != 0) {
        printf("El Nombre Del Archivo Es : [%s]\n",nombreArch);
        printf("La Media Es : [%f]\n",media);
        printf("La Varianza Es : [%f]\n",varianza);
        printf("El Menor Es : [%d]\n",menor);
        printf("El Mayor Es : [%d]\n",mayor);
	}

	return 0;
}

void *manejador(void *param){
	int* indice = (int*)param;
	if(*indice == 0){
		FILE *archivo;
        int x,i=0;
        printf("Ingrese El Nombre Del Archivo : ");
        scanf("%s",nombreArch);
   		archivo = fopen(nombreArch,"r");
	   		if (archivo == NULL){
	     		 printf("Error al abrir el archivo.\n");
	      	}else{
	      		 while (feof (archivo)==0){
	             	fscanf(archivo ,"%d",&x);
	             	n++;
	            }

                numeros = calloc(n, sizeof(int));
                rewind(archivo);
                printf("El Archivo Contiene Lo Siguiente:\n");
                while (feof (archivo)==0){
                    fscanf(archivo ,"%d",&numeros[i]);
                    printf("%d. [%d]\n",i+1,numeros[i]);
                    i++;
                }
                printf("\n");
                fclose(archivo);
			}

	}
    if(*indice == 1){
        pthread_join(hilos[0],NULL);
        if (n != 0) {
            int suma = 0;
            int i=0;
                for(i = 0; i<n; i++){
                    suma = suma + numeros[i];
                }
            media = (suma*1.0f/n);
        }
	}
	if(*indice == 2){
        pthread_join(hilos[1],NULL);
        if (n != 0) {
               int suma = 0;
               int i=0;
               for(i = 0; i<n; i++){
                    suma  = suma + pow((numeros[i]-media),2);
               }
            varianza = (suma*1.0f/n);
        }
	}

	if(*indice == 3){
        pthread_join(hilos[2],NULL);
        if (n != 0) {
               int i=0;
               menor = numeros[0];
               for(i = 0; i<n; i++){
                    if(menor>numeros[i]){
                            menor = numeros[i];
                    }
               }
        }
	}

	if(*indice == 4){
        pthread_join(hilos[3],NULL);
        if (n != 0) {
               int i=0;
               mayor = numeros[0];
               for(i = 0; i<n; i++){
                   if(mayor<numeros[i]){
                            mayor = numeros[i];
                    }
               }
        }
	}

    free(indice);
}
