#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main(){
	int fd[2],i,n[2];
	pipe(fd);

	for (i = 0; i < 2; ++i){
		if ((n[i]=fork())==0) {
			pause();
		}
		if (i==0)
		{
			close(fd[0]);
			printf(" Soy el primer hijo...%d\n",getpid());
		}else if (i==1)
		{
			close(fd[1]);
			printf(" Soy el segundo hijo...%d\n",getpid());
		}else{
			close(fd[0]);
			close(fd[2]);
			printf("Luke soy tu padre... XD..%d\n",getpid());
			
		}
	}

	return 0;
}
