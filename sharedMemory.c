#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>

int main(int argc, char const *argv[])
{
	int shm_id;
	int shm_size = 1024;
	void *ptr;
	shm_id = shmget(IPC_PRIVATE, shm_size, IPC_CREAT|0600);
	ptr = shmat(shm_id,0,0);
	if (!fork())
	{
		sleep(2);
		printf("[%d]%s\n",getpid(),(char *)ptr);
		sprintf(ptr,"bye!");
		shmdt(ptr);
	}else{
		sprintf(ptr,"Hola Mundo!");
		printf("[%d]%s\n",getpid(),(char *)ptr);
		wait(NULL);
		printf("[%d]%s\n",getpid(),(char *)ptr);
		shmdt(ptr);
		shmctl(shm_id, IPC_RMID,0);
		
	}

	return 0;
}