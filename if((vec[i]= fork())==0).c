#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>

void manejador (int s){}

int main(){
int n; // variable para guardar el numero de procesos
pid_t pid; // guarda el pid de los procesos hijos
pid_t padre = getpid(); // guarda el pid del padre
int vec[10];
int i;

signal(SIGUSR1,manejador);
printf("Ingrese el número de procesos \n");
scanf("%d", &n);

	
		for (i=0; i<n; i++){
			if((vec[i]= fork())==0){
				pause();
				printf("Hijo %d  pid %d \n",i,getpid());				
				kill(getppid(), SIGUSR1);
				break;
			}	
		}

	
// INICIA CÓDIGO DE PRUEBA


if (padre==getpid()){
	for(i=0;i<n;i++){ 
if(i==0)
{
   printf("Padre %d\n", getpid());
		//waitpid (pid, NULL, 0);
	   kill(vec[i], SIGUSR1);
	   pause();
}
if(i>0&&i<(n))
{
 	kill(vec[i], SIGUSR1);
	   pause(); 
}
		if(i==(n-1))
{	   
	   printf("Padre %d\n", getpid());
		//waitpid (pid, NULL, 0);
	   kill(vec[i], SIGUSR1);
	   pause();
}
	}
}

// TERMINA CÓDIGO DE PRUEBA 

return 0;
}

