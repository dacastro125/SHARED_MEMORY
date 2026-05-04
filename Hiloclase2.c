#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void*function_hilo(void *param);
void algo(int);
int sum;
pthread_mutex_t my = PTHREAD_MUTEX_INITIALIZER;


 int main()
{
	int i;
	pthread_t thread_id;

	sum=0;

	printf("previa creacion de hilos sum=%d\n",sum);

	pthread_create(&thread_id,NULL,function_hilo,NULL);
	for (i = 0; i < 10000; i++)
	{
		algo(16);
	pthread_mutex_lock(&my);		
		sum++;
	pthread_mutex_unlock(&my);
	}

	pthread_join(thread_id,NULL);
	printf("HIlo principal sum=%d\n",sum );
	return 0;
}
void*function_hilo(void*param){
	int i;
	printf("Hilos %lu\n",pthread_self());
	for ( i = 0; i < 10000; i++)
	{
		algo(16);
	pthread_mutex_lock(&my);		
		sum++;
	pthread_mutex_unlock(&my);	
	}

	pthread_exit(0);
}

void algo(int n){
	usleep(n);
}