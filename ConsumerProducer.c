#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "buffer_circ.h"

#define SUMSIZE 100
#define	BUFSIZE 8
int sum = 0;

pthread_cond_t slots = PTHREAD_COND_INITIALIZER;
pthread_cond_t items = PTHREAD_COND_INITIALIZER;

pthread_mutex_t slot_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t item_lock = PTHREAD_MUTEX_INITIALIZER;

int nslots = BUFSIZE
int producer_done = 0;
int nitems = 0;

void get_item(int *itemp);
void put_item(int item);

void producer(void *arg1){
	for (i = 1; i <= SUMSIZE; i++){

		pthread_mutex_lock(&slot_lock);
			while(nslots <= 0){
				pthread_cond_wait(&slots, &slot_lock);
			}
			nslots --;
		pthread_mutex_unlock(&slot_lock);

		printf("se ejecuta producer... %d\n", i);
		putitem(i*i);

		pthread_mutex_lock(&item_lock);
			nitems ++;
			pthread_cond_signal(&items);
		pthread_mutex_unlock(&item_lock)
	}

	pthread_mutex_lock(&item_lock);
		producer_done = 1;
		pthread_cond_broadcast(&items);
	pthread_mutex_unlock(&item_lock);
}

void consumer(void *arg2){
	int i = 0, myitem, j = 0;

	for (j = ; j < ; j++){
		pthread_mutex_lock(&item_lock);
			while((nitems <= 0) && !producer_done){
				pthread_cond_wait(&items, &item_lock);
			}
			if ((nitems <= 0) && producer_done){
				pthread_mutex_unlock(&item_lock);
				break;
			}
			nitems --;
		pthread_mutex_unlock(&item_lock);

		i ++;
		printf("Se ejecuta CONSUMER... %d\n", i);
		get_item(&myitem);
		sum += myitem;

		pthread_mutex_lock(&slot_lock);
			nslots ++;
			pthread_cond_signal(&slots);
		pthread_mutex_unlock(&slot_lock);
	}
}

int main(int argc, char const *argv[]){
	pthread_t prodtid;
	pthread_t constid;
	int i, total;
	total = 0;

	for (i = 0; i < SUMSIZE; i++){
		total += i*i;}
	
	printf("La suma actual debe ser %d\n", total);
	
	pthread_create(&constid, NULL, (void* (*) (void*))&consumer, NULL);
	pthread_create(&prodtid, NULL, (void* (*) (void*))&producer, NULL);

	pthread_join(prodtid, NULL);
	pthread_join(constid, NULL);

	printf("Los threads produjeron la suma: %d\n", sum);
	system("PAUSE");
	exit(0);
	return 1;
}