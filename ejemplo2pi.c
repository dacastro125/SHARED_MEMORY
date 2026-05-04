#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
void manejador(){}


int main (int argc, char *argv[]){

	int i,N,aux,del;
	pid_t hijo[4];
	double suma=0.0,auxd=0.0,delta,pi;
	int fd[4][2];
	printf("cuantos intervalos quiere preferiblemente multimplo de 4\n");
	scanf("%d",&N);
	delta=1/(double)N;
	del=N/4;
	
	for(i=0;i<4;i++){
		pipe(fd[i]);
		if((hijo[i]=fork())==0){
			close(fd[i][0]);
			if(i!=0){
				for(aux=0;aux<i;aux++){
					close(fd[aux][0]);
				}	
			}
			break;
		}else{
			close(fd[i][1]);
		}
	}
	if(i==4){
		for(aux=0;aux<4;aux++){
			wait(NULL);
		}		
		for(aux=0;aux<4;aux++){
			N=read(fd[aux][0],&auxd,sizeof(double));
			suma+=auxd;
				
		}
		pi=suma*delta;
		printf("pi=%f \n",pi);		
	}else{
		int inf,sup;
		double x,y;
		inf = del*i;
		sup = del*(i+1);
		for(aux = inf+1;aux<=sup;aux++){
			x = (double)(aux-0.5)*delta;
			y = 4.0/(1.0+(x*x));
			suma += y;	
		}
		write(fd[i][1],&suma,sizeof(double));	
		printf("termina hijo %d \n",i);
	}
	return 0;
}
