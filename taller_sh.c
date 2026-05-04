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
	int i=0, j=0, count = 0;
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < cols; ++j)
		{
			m[i][j] = ++count;
		}
	}
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
	
	
	
	pid_t pidhijo;
	signal(SIGUSR1, manejador);
	
	
	int shm_id;
	int shm_size;
	size_t sizeMatrix;
	double** matrix;

	int rows=0;
	int cols=0;

	printf("IGRESE EL NUMERO DE FILAS: ");
	scanf("%d", &rows);

	printf("IGRESE EL NUMERO DE COLUMNAS: ");
	scanf("%d", &cols);

	sizeMatrix = sizeof_dm(rows,cols,sizeof(double));
	shm_id = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0600);
	matrix = shmat(shm_id, NULL, 0);

	create_index((void*)matrix, rows, cols, sizeof(double));
	int count =0;
	pidhijo = fork();

	if (pidhijo != 0) // padre
	{
		usleep(1000);
		llenarMatrix(rows,cols,matrix);
		kill(pidhijo, SIGUSR1);
		pause();
		printMatrix(rows,cols,matrix);
		wait(NULL);
		shmdt(matrix);
		shmctl(shm_id, IPC_RMID, 0);

		
	}else{ // hijo
		pause();
		printMatrix(rows,cols,matrix);
		printf("\n");
		matrix[2][2] = -99.0;
		kill(getppid(), SIGUSR1);
		shmdt(matrix);
	}

return 0;
}