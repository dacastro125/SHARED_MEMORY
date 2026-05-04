#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <signal.h>

void Error(char *msj){// solo se define si falla añadir el segmento u obtenerlo 
	printf("asdf:%s\n",msj );
}

int main(int argc, char const *argv[])
{
	int shmid, i;
	char shm_pointer;
	int key = ftok("asdf",2013);
	
	shmid = shmget(key, 50, IPC_CREAT | 0666);
	
	shm_pointer = shmat(shmid, NULL, 0);
	
	scanf("%s", shm_pointer);
	scanf("%d", &i);
	shmctl(shmid, IPC_RMID, NULL);
	
	return 0;
}