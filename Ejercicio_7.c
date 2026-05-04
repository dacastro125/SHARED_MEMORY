#include <stdio.h>
#include <unistd.h>

struct msj{
	int a;
	float b;
};

int main(){
	int fd[2], n;
	char buffer[50];
	pipe(fd);
	if(fork()==0){
		close(fd[0]);
		struct msj v1;
		v1.a = 5;
		v1.b = 12.35;
		write(fd[1], &v1, sizeof(struct msj));
		
	}else{
		close(fd[1]);
		struct msj v2;
		read(fd[0], &v2, sizeof(struct msj));
		printf("a=%d b=%f\n", v2.a, v2.b);
		wait(NULL);
	}
	return 0;
}