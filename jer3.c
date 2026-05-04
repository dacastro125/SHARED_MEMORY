#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig){}

int main(int argc, char const *argv[])
{
	int i;
	pid_t p = getpid(), h;

	if (fork() == 0){
		printf("hijo H = %d\n",getpid());
		usleep(1000);
		for (i = 0; i < 2; i++){
			if (fork() == 0){// si está en los hijos de segunda generacion
				break;
			}
		}
	}	

	else if (p == getpid()){
		usleep(1000);
		printf("padre = %d\n",getpid());
		char buff[20];
		sprintf(buff, "pstree -c -n -p %d",getpid());
		system(buff);
	}
	/*else if (h == getpid()){
		printf("hijo H = %d\n",getpid());
		usleep(1000);
		for (i = 0; i < 2; i++){
			if (fork() == 0){// si está en los hijos de segunda generacion
				break;
			}
		}*/
	else if(i == 0 || i == 1){
		
		for (i = 0; i < 3; i++){
			if (fork() == 0){
				break;
			}
		}
		usleep(1000);
		
	}

	return 0;
}