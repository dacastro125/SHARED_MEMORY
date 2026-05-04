#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * fh(void *);
int c=0;
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;

int main(){
pthread_t hid[5];
int i, j;

for(i=0;i<5;i++)
	pthread_create(&hid[i], NULL, fh, NULL);


for(j=0;j<5;j++)
	pthread_join(hid[j], NULL);

printf("c: %d\n",c);
return 0;
}

void * fh(void *arg){
int i;
	for(i=0;i<10;i++){
		pthread_mutex_lock(&m);
		c=c+1;
		pthread_mutex_unlock(&m);
		usleep(100000);
	}
	pthread_exit(NULL);
}
