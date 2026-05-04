#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SIZE 50

void Error(char *msj){
	printf("%s\n", msj);
	exit(1);
}
