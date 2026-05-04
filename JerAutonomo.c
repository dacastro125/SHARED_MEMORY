#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handler(int sig){}

int main(int argc, char const *argv[])
{
	pid_t p = getpid(), h;
	int i;

	if ((h = fork()) == 0)// si es el hijo
	{
		for (i = 0; i < 2; i++)
		{
			if(fork() == 0){
				break;
			}
		}
	}

	if (p == getpid()){// si es el padre que imprima la jerarquia
		//sleep(1);
		printf("padre = %d\n",getpid());
		char buff[20];
		sprintf(buff, "pstree -c -n -p %d",getpid());
		system(buff);
		wait(NULL);
	}
	else if (i == 0 || i == 1){// si es alguno de los dos hijos
		
		for (i = 0; i < 3; i++){
				if(fork() == 0){
				break;
				}
			}
		char buff[20];
		sprintf(buff, "pstree -c -n -p %d",getpid());
		system(buff);
	}
	else if(h == getpid() ){
		printf("primer hijo %d\n",getpid() );
		for (i = 0; i < 2; i++)
		{
			wait(NULL);
		}
	}
	else{
		printf("nietos %d\n",getpid() );
	}


	return 0;
}