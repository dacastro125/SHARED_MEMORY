#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<wait.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<signal.h>

void manejador(){ }
pid_t pid;
int shmId;
int j,k;
int Rows;
int Cols;
double **matrix;
unsigned int sizeof_dm(int rows, int col, size_t sizeElement){
size_t size = rows *sizeof(void *);
size += (col*rows*sizeElement); 
return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement){
    int i;
    size_t sizerow = cols*sizeElement;
    m[0] = m+rows; 
    for(i=1; i<cols; i++){
        m[i]= (m[i-1]+sizerow);
    }
}

int main(){
signal(SIGUSR1,manejador);
	
   
    pid = fork();
    
      
            if (pid != 0)
             {
			 pause();
				printf("hijo imprime\n");
				for(k=0; k<Cols; k++){
					for(int j=0; j<Rows; j++){
						printf("valor en col %d fila %d = %.6lf\n",k,j ,matrix[k][j]);
					}                       
				  }
				  printf("hijo modifica\n");
					  for(k=0; k<Cols; k++){
						for(int j=0; j<Rows; j++){
							matrix[k][j]=(double)(j+1)/(double)(j+2);
							}
						}
						usleep(1000);
						kill(pid,SIGUSR1);
							shmdt(matrix);
			}
						
			else{
				printf("Ingrese el tamano del columnas \n");	
				scanf("%d",&Cols);
				printf("Ingrese el tamano del filas.\n");
				scanf("%d",&Rows);
								
				size_t sizeMatrix = sizeof_dm(Rows,Cols,sizeof(double));
				shmId = shmget(IPC_PRIVATE,sizeMatrix,IPC_CREAT|0600);
				matrix=shmat(shmId, NULL, 0);
				create_index((void*)matrix,Rows,Cols,sizeof(double));
				printf("Padre llena \n");	
					for(k=0; k<Cols; k++){
						for(int j=0; j<Rows; j++){
								matrix[k][j]=(double)j/(double)(j+1);
							}
						}
				usleep(2500);
				kill(pid,SIGUSR1);		
				pause();						
				printf("padre imprime\n");
				for(k=0; k<Cols; k++){
					for(int j=0; j<Rows; j++){
						printf("valor en col %d fila %d = %.6lf\n",k,j ,matrix[k][j]);
							}
				}
                       
        shmdt(matrix);
        shmctl(shmId,IPC_RMID,0);
        }
        return 0;
}
