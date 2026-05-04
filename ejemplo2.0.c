#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>

int main(){
pid_t idProceso;

int tuberia[2],n;
char buff[50];
if (pipe (tuberia) == -1)

	{

		perror ("No se puede crear Tuberia");

		exit (-1);

	}

idProceso=fork();
if (idProceso == -1)

	{

		perror ("No se puede crear proceso");

		exit (-1);

	}
if(idProceso>0){
close(tuberia[0]);
write(tuberia[1],"Hola",14);
printf("[%d]Escrito:Yo soy el padre\n",getpid());
wait(NULL);

}
if(idProceso == 0){
close(tuberia[1]);
n=read(tuberia[0],buff,50);
buff[n]='\0';
printf("[%d]Leido yo soy el hijo y e leido del padre  :%s \n",getpid(),buff);

}

return 0;
}
