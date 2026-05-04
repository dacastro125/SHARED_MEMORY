#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>


unsigned int sizeof_dm(int rows, int col, size_t sizeElement){
	size_t size = rows*sizeof(void *);
	size += (col * sizeElement);
	return size;
}

void manejador(){
	
}

void create_index( void **m, int rows,int cols, size_t sizeElement){
	int i;
	size_t sizeRow = cols*sizeElement;
	m[0] = m+rows;
	for ( i = 1; i < cols; i++)
	{
		m[i] = (m[i-1]+sizeRow);
	}
}

int main(int argc, char const *argv[])
{
	int fil, col,i,j;
	signal(SIGUSR1, manejador);
	int shmId;
	int padreid = getpid();
	double **matrix;
	printf("Ingrese el tamano del filas del vector...\n");
	scanf("%d",&fil);

	printf("Ingrese el tamano del columnas del vector...\n");
	scanf("%d",&col);

	size_t sizeMatrix = sizeof_dm(fil,col, sizeof(double));
	shmId = shmget(IPC_PRIVATE,sizeMatrix, IPC_CREAT|0600);

	matrix = shmat(shmId,0,0);

	create_index((void *)matrix,fil,col,sizeof(double));
	pid_t  hijo = fork();

	if (hijo==0)
	{
		pause();
		for (i = 0; i < fil; i++)
			{
				for (j = 0; j < col; j++)
				{	
					printf("[%f]",matrix[i][j]);
					matrix[i][j] = (double)(i-j);
					
				}
				printf("\n");
			}
			printf("\n\n");
			shmdt(matrix);
			kill(getppid(),SIGUSR1);
	}else{
			usleep(1000);
			for (i = 0; i < fil; i++)
			{
				for (j = 0; j < col; j++)
				{
					matrix[i][j] = (double)(i+j);
					
				}
				
			}
			
			kill(hijo,SIGUSR1);
			pause();
			wait(NULL);
			for (i = 0; i < fil; i++)
			{
				for (j = 0; j < col; j++)
				{
					
					printf("[%f]",matrix[i][j] );
				}
				printf("\n");
			}
			shmdt(matrix);
			shmctl(shmId, IPC_RMID,0);
	}
	

	return 0;
}