#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
	int fd[2], n;
	char buffer[50];
	pipe(fd);
	if(fork()==0){
		close(fd[1]);
		n = read(fd[0], buffer, 50);
		buffer[n] = '\0';
		printf("%d leido %s\n", getpid(), buffer);
	}else{
		close(fd[0]);
		strcpy(buffer, "Hola mundo\n");
		write(fd[1], buffer, strlen(buffer));
		wait(NULL);
	}
	return 0;
}