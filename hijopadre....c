#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
  pid_t pid;
  int i;

 
    for (i = 0; i < 8; i++){
	 pid = fork();
  	if (pid == 0){
     	 //sleep (rand()%4);
      	printf("-SON-%d - Pid:%d\n",i,getppid());
		break;
    		}
   // return 0;
  }

 //sleep (rand()%4);
  printf("+FATHER+ Waiting for son's termination...\n");
  waitpid (pid, NULL, 0);
  printf("+FATHER+ ...ended\n");

  //return 0;
}
