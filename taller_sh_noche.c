#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>


void manejador(int mysignal){}

unsigned int sizeof_dm(int rows, int col, size_t sizeElement){

size_t size = rows * sizeof(double *);
size += (col*sizeElement);
return size;	

}

void create_index(void **m, int rows, int cols, size_t sizeElement){
	int i=0;
	size_t sizeRow = cols * rows * sizeElement;
	m[0] = m+rows;
	for (i = 1; i < cols; ++i)
	{
		m[i] = (m[i-1] + sizeRow);
	}
}

void llenarMatrix(int rows, int cols, double **m){
	int i=0, j=0, count = -1;
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < cols; ++j)
		{
			m[i][j] = ++count;
		}
	}
}

double sumarColumna(int rows, int col , double **m){

	double suma = 0;
	double valReturn=0;
	int i=0;
	for ( i = 0; i < rows; i++)
	{
		suma = m[i][col];
		valReturn = suma + valReturn;
	}
	return valReturn;
}

void printMatrix(int rows, int cols, double **m){
	for (int x = 0; x < rows; ++x)
		{
			for (int y = 0; y < cols; ++y)
			{
				printf("%f\t", m[x][y]); 
			}
			printf("\n");
		}
}
int main (int argc, char *argv[]){
	
	
	
	
	signal(SIGUSR1, manejador);
	int i = 0;
	int shm_id;
	int shm_size;
	int shm_idSuma;
	int shm_sizeSuma=sizeof(double);
	int padre = getpid();
	size_t sizeMatrix;
	double** matrix;
	double* sumaCols;
	int rows=0;
	int cols=0;

	printf("IGRESE EL NUMERO DE FILAS: ");
	scanf("%d", &rows);

	printf("IGRESE EL NUMERO DE COLUMNAS: ");
	scanf("%d", &cols);

	pid_t hijos[cols];
	sizeMatrix = sizeof_dm(rows,cols,sizeof(double));
	int tam = sizeof_dm(rows,cols,sizeof(double));
	printf("%d\n", tam );
	shm_id = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0600);
	matrix = shmat(shm_id, NULL, 0);

	create_index((void*)matrix, rows, cols, sizeof(double));

	shm_idSuma = shmget(IPC_PRIVATE, shm_sizeSuma, IPC_CREAT | 0600);
	sumaCols = shmat(shm_idSuma, NULL, 0);
	
	

	for ( i = 0; i < cols; i++)
	{
		hijos[i] = fork();
		if (hijos[i] == 0)
		{
			pause();	
			sumaCols[i] = sumarColumna(rows, i, matrix);
			shmdt(matrix);
			shmdt(sumaCols);
			break;
		}
	}

	if (padre == getpid())
	{
		
		usleep(1000);
		llenarMatrix(rows,cols,matrix);
		printMatrix(rows, cols, matrix);
		
	
		for ( i = 0; i < cols; i++)
		{
		 kill(hijos[i], SIGUSR1);
		 wait(NULL);	
		 printf("[%d] suma [%d]: %f\n",getpid(),i, sumaCols[i] );
		}

		shmdt(matrix);
		shmdt(sumaCols);
		shmctl(shm_id, IPC_RMID, NULL);
		shmctl(shm_idSuma, IPC_RMID, NULL);
	}
return 0;
}