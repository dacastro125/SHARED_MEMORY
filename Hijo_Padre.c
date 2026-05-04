#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void manejador(int sig){}

int main(){
  int c;
  signal(SIGUSR1, manejador);
  if(!(c=fork())){
    pause();
    printf("Hijo\n");
    
  }
  else{
    printf("padre \n");
    sleep(1);
    kill(c, SIGUSR1);
    wait(NULL);
  }
return 0;

}