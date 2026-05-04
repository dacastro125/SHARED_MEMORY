#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * fh(void *);
int v[10]={1,2,3,4,5,6,7,8,9,10};

struct N{
int a, b;
};

int main(){
struct N *p;
pthread_t hid[2];
int i;

for(i=0;i<2;i++){
	p=(struct N *)malloc(sizeof(struct N));
	p->a=i*5;
	p->b=p->a+5-1;
	pthread_create(&hid[i], NULL, fh, (void *)p);
}

for(i=0;i<2;i++)
	pthread_join(hid[i], NULL);
return 0;
}

void * fh(void *arg){
	struct N *p=(struct N*)arg;
	int i, suma=0;
	for(i=p->a; i<p->b; i++)
		suma+=v[i];
	printf("suma de %d hasta %d es %d \n", p->a, p->b, suma);
	pthread_exit(NULL);
}
