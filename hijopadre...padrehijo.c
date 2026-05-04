#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
  pid_t pid;
  int i;

  pid = fork();

  if (pid == 0){
    for (i = 0; i < 3; i++){
      //sleep (rand()%4);
      printf("-SON-%d\n",getppid());
    }
    return 0;
  }

 // sleep (rand()%4);
  printf("+FATHER+ Waiting for son's termination...\n");
  waitpid (pid, NULL, 0);
  printf("+FATHER+ ...ended\n");

  return 0;
}

