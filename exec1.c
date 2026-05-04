#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	
	int h = fork();
	if (h == 0) {
		printf("Este es el hijo\n");
		char * const args[] = {"Hola desde exec1", NULL};
		execv("exec2", args);
	} else {
		printf("Este es el padre\n");
	}

	return 0;
}