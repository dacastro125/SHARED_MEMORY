#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>


#define MAX_SIZE 10

int main(){
    printf("------\n");
	double *a, *b, *result;//Crea los vectores a,b y la variable result
	int i;//Variable para recorrer el vector
	int shm_idA, shm_idB, shm_idR;//Se crean los id de los vectores a,b y la variable result
	int shm_size = MAX_SIZE*sizeof(double);//El tamaño que debe tener la memoria compartida


	shm_idA = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0600);//Crear el espacio de memoria para el vector a
	shm_idB = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0600);//Crear el espacio de memoria para el vector b
	shm_idR = shmget(IPC_PRIVATE, sizeof(double), IPC_CREAT | 0600);//Crear el espacio de memoria para la variable result
	a = shmat (shm_idA, 0, 0);//Asignar el espacio de memoria para el vector a
	b = shmat (shm_idB, 0, 0);//Asignar el espacio de memoria para el vector b
	result = shmat (shm_idR, 0, 0);//Asignar el espacio de memoria para la variable result
	
	
	//Llena los vectores
	for(i=0; i<MAX_SIZE; i++){
		a[i] = i;
		b[i] = i - 0.5;
	}
	//Imprime los vectores
	for(i=0; i<MAX_SIZE; i++){
		printf("a[%f]\n",a[i]);
		printf("b[%f]\n",a[i]);
	}

	printf("Soy el proceso %d\n",getpid());//Imprime el Id del proceso principal
	
	if(fork()==0){
	    printf("Soy el hijo %d, mi padre es %d\n",getpid(),getppid());//El Id Hijo y Padre
		double temp = 0.0;//Acumulador
		for(i=0;i<MAX_SIZE; i++){
			temp += a[i] + b[i];
		}
		*result = temp;//asigna lo que hay en la variable temp y se lo pasa a a result
		printf("[%d]%f\n", getpid(), temp );
		shmdt(a);//Pasa información a la memoria compartida. 
		shmdt(b);//Pasa información a la memoria compartida. 
		shmdt(result);//Pasa información a la memoria compartida. 
	}
	else{
	    wait(NULL);
	    printf("Soy el Padre %d\n",getpid());//Imprime Id Padre
		printf("[%d]%f\n", getpid(), *result );
		shmdt(a);//También sirve para destruir el segmento de memoria compartida.
		shmdt(b);//También sirve para destruir el segmento de memoria compartida.
		shmdt(result);//También sirve para destruir el segmento de memoria compartida.
		shmctl(shm_idA, IPC_RMID, 0);//Elimina el segmento de memoria
		shmctl(shm_idB, IPC_RMID, 0);//Elimina el segmento de memoria
		shmctl(shm_idR, IPC_RMID, 0);//Elimina el segmento de memoria
	}
return 0;
}