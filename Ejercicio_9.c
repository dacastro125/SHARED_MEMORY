#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
	int fd[3][2], hijos[3], n, x, *vector, tamVec, i;
	char buffer[50];
	pipe(fd[0]);
	pipe(fd[1]);
	pipe(fd[2]);
	printf("Digite el tamaño del vector: ");
	scanf("%d", &tamVec);
	vector = malloc(sizeof(int)*tamVec);
	printf("Digite los valores del vector:\n");
	for(i = 0; i < tamVec; i++){
		printf("vector[%d]=", i);
		scanf("%d", &vector[i]);
	}
	for(x = 0; x < 3; x++){
		if(!(hijos[x]=fork())){
			break;
		}
	}
	if(x == 3){
		close(fd[0][1]);
		close(fd[1][1]);
		close(fd[2][1]);
		double promedio;
		int mayor, result[2];
		read(fd[0][0], &promedio, sizeof(double));
		read(fd[1][0], &mayor, sizeof(int));
		read(fd[2][0], &result, sizeof(int)*2);
		printf("El promedio del vector es: %f\n", promedio);
		printf("El numero mayor del vector es: %d\n", mayor);
		printf("El numero que mas se repite del vector es: %d, con %d vece(s) de repeticion\n", result[0], result[1]);
		wait(NULL);
		wait(NULL);
		wait(NULL);
	}
	if(x == 0){
		close(fd[0][0]);
		close(fd[1][0]);
		close(fd[1][1]);
		close(fd[2][0]);
		close(fd[2][1]);
		double suma = 0, promedio;
		for(i = 0; i < tamVec; i++){
			suma += vector[i];
		}
		promedio = suma / tamVec;
		write(fd[0][1], &promedio, sizeof(double));
	}
	if(x == 1){
		close(fd[0][0]);
		close(fd[0][1]);
		close(fd[1][0]);
		close(fd[2][0]);
		close(fd[2][1]);
		int mayor = 0;
		for(i = 0; i < tamVec; i++){
    			if(vector[i] > mayor){
    				mayor = vector[i];
    			}
		}
		write(fd[1][1], &mayor, sizeof(int));
	}
	if(x == 2){
		close(fd[0][0]);
		close(fd[0][1]);
		close(fd[1][0]);
		close(fd[1][1]);
		close(fd[2][0]);
		int repitente = 0, *frecuencia, result[2], mayorFrecuencia = -9999;;
		frecuencia = malloc(sizeof(int)*tamVec);
		for(i = 0; i < tamVec; i++){
			frecuencia[i] = 0;
		}
		int comparador, j;
		for(i = 0; i < tamVec; i++) {
			comparador = vector[i];
			for(j = 0; j < tamVec; j++) {
				if(vector[j] == comparador){
					frecuencia[i]++;
				}
			}
			comparador = -1;
		}
		for(i = 0; i < tamVec; i++){
			if(frecuencia[i] > repitente){
				mayorFrecuencia = frecuencia[i];
				repitente = vector[i];
			}
		}
		result[0] = repitente;
		result[1] = mayorFrecuencia;
		write(fd[2][1], result, sizeof(int)*2);
	}
	return 0;
}