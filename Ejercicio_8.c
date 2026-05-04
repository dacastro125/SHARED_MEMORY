#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
	int fd[2][2], hijos[2], n, x;
	char buffer[50];
	pipe(fd[0]);
	pipe(fd[1]);
	printf("Digite un mensaje (maximo 50 caracteres):\n");
	scanf("%s", buffer);
	for(x = 0; x < 2; x++){
		if(!(hijos[x]=fork())){
			break;
		}
	}
	if(x == 2){
		close(fd[0][0]);
		close(fd[1][0]);
		close(fd[1][1]);
		write(fd[0][1], buffer, strlen(buffer));
		wait(NULL);
		wait(NULL);
	}
	if(x == 0){
		close(fd[0][1]);
		close(fd[1][0]);
		n = read(fd[0][0], buffer, 50);
		buffer[n] = '\0';
		printf("%d leido %s\n", getpid(), buffer);
		write(fd[1][1], buffer, strlen(buffer));
	}
	if(x == 1){
		close(fd[0][0]);
		close(fd[0][1]);
		close(fd[1][1]);
		n = read(fd[1][0], buffer, 50);
		buffer[n] = '\0';
		printf("%d leido %s\n", getpid(), buffer);
	}
	return 0;
}