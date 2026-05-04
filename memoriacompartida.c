#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>


#define MAX_SIZE 10000000

int main(){
	double *a, *b, *result;
	int i;
	int shm_idA, shm_idB, shm_idR;
	int shm_size = MAX_SIZE*sizeof(double);


	shm_idA = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0600);
	shm_idB = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0600);
	shm_idR = shmget(IPC_PRIVATE, sizeof(double), IPC_CREAT | 0600);
	a = shmat (shm_idA, 0, 0);
	b = shmat (shm_idB, 0, 0);
	result = shmat (shm_idR, 0, 0);

	for(i=0; i<MAX_SIZE; i++){
		a[i] = i;
		b[i] = i - 0.5;
	}

	if(!fork()){
		double temp = 0.0;
		for(i=0;i<MAX_SIZE; i++){
			temp += a[i] + b[i];
		}
		*result = temp;
		printf("[%d]%f\n", getpid(), temp );
		shmdt(a);
		shmdt(b);
		shmdt(result);
	}
	else{
		wait(NULL);
		printf("[%d]%f\n", getpid(), *result );
		shmdt(a);
		shmdt(b);
		shmdt(result);
		shmctl(shm_idA, IPC_RMID, 0);
		shmctl(shm_idB, IPC_RMID, 0);
		shmctl(shm_idR, IPC_RMID, 0);
	}
return 0;
}