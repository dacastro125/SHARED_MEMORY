#include <stdio.h>
#include <signal.h>

void manejador(){}

int main(){
    int c;
    signal(SIGUSR1, manejador);
    if( c=fork()){
    pause();
      printf("Hijo %d\n",getpid());
  	}else{
    printf("padre %d\n",getpid());
    kill(c, SIGUSR1);
    //usleep(5000);
    wait(NULL);
	}
    //wait(NULL);
	    
return 0;

}