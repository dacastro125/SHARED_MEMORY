#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>

void set_m(int row, int col, double valor, double *m, int cols){
	int position = 0;
	position = ((row*cols) + col);
	m[position] = valor;
}

double get_m(int row, int col, double *m, int cols){
	int position = 0;
	double valReturn =0.0;
	position = ((row*cols) + col);
	valReturn = m[position];
	return valReturn;
}

void llenarMatriz(double* m, int rows, int cols){
	int i=0,  j =0; double count = -1;
	for ( i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			++count;
			set_m(i, j, count, m, cols);
		}
	}
}

void print_m(double* m, int rows, int cols){
	for (int x = 0; x < rows; ++x)
		{
			for (int y = 0; y < cols; ++y)
			{
				printf("%f\t", get_m(x, y, m, cols )); 
			}
			printf("\n");
		}
}

void manejador(int mysignal){}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1, manejador);
	pid_t pidhijo;
	int rows =0, cols=0;
	int mod_x=0, mod_y=0;
	int shm_id;
	int shm_size;
	int size_of_m;
	double* matrix;

	printf("Ingrese el numero de filas: ");
	scanf("%d",&rows);

	printf("Ingrese el numero de columnas: ");
	scanf("%d",&cols);

	size_of_m = rows * cols;
	shm_size = size_of_m * sizeof(double);

	shm_id = shmget(IPC_PRIVATE, shm_size , IPC_CREAT | 0600);
	matrix = shmat(shm_id, NULL, 0);

	//llenarMatriz(matrix, rows, cols);
	//print_m(matrix, rows, cols);

	pidhijo = fork();
	if (pidhijo == 0)
	{
		pause();
		print_m(matrix, rows, cols);
		mod_x=mod_y=2;
		set_m(mod_x,mod_y,-99.0,matrix,cols);
		kill(getppid(), SIGUSR1);
		shmdt(matrix);
	}else{
		usleep(100);
		llenarMatriz(matrix, rows, cols);
		kill(pidhijo, SIGUSR1);
		pause();
		printf("\n");
		print_m(matrix, rows, cols);
		wait(NULL);
		shmdt(matrix);
		shmctl(shm_id,IPC_RMID,0);
	}
	return 0;
}