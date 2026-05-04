#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main () {

int n, i, j;
	printf ("ingrese el valor a crear: \n");
	scanf("%d", &n);
	
	int fd[n][2];
	
	for(i=0; i<n-1; i++){
		pipe(fd[i]);
	}
	
	
	
	for(i=0; i<n; i++){
		
		if(fork()!=0){
			
			for(j=0; j<n; j++){
				
				if(j!=i){
					
					close(fd[j][0]);
					close(fd[j][1]);
					
				}
				
			}
			
			close(fd[i][0]);
			//escribir
			char * mensaje="Mensaje";
			write(fd[i][1],&mensaje, sizeof(mensaje)); 			
			printf("[%d] soy el padre enviando el mensaje\n", getpid());
		
		break;
	
		}else{
			char * mensaje;
		//leer
		read(fd[i][0], &mensaje, sizeof(mensaje));
		printf("[%d] -%s\n", getpid(), mensaje);
	}
	}
return 0;
}
