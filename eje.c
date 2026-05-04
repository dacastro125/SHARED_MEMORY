#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

int **m_a,**m_b,**m_r;

int N;


void * f(void*arg);

int main(int argc, char const *argv[])
{
	pthread_t *h;
    int i,j,*fila;

    printf("ingrese el tamaño de la matriz .:. \n");
    scanf("%d",&N);

    m_a = (int**)malloc(sizeof(int*)*N);
    m_b = (int**)malloc(sizeof(int*)*N);
    m_r = (int**)malloc(sizeof(int*)*N);

    for(i=0; i<N; i++){
	    m_a[i] =  (int*)malloc(sizeof(int)*N);
	    m_b[i] =  (int*)malloc(sizeof(int)*N);
	    m_r[i] =  (int*)malloc(sizeof(int)*N);
    }
    int data;
    for ( i = 0; i < N; i++ )
	{
		for ( j = 0; j < N; j++ )
	    {
            scanf("%d",&data);
            m_a[i][j] = data;
            m_b[i][j] = data;
	    }   
	}

	h = (pthread_t*)malloc(sizeof(pthread_t)*N);

	for ( i = 0; i < N; i++ )
	{
		fila = (int*)malloc(sizeof(int));
		*fila = i;
		pthread_create(&h[i],NULL,f,(void*)fila);
	}

    for ( i = 0; i < N; i++ )
	{
		pthread_join(h[i],NULL);
    }

    printf("\n");
    for ( i = 0; i < N; i++ )
	{
		for ( j = 0; j < N; j++ )
	    {
            printf("\t%d",m_r[i][j]);
	    }
	    printf("\n");
	}


	return 0;
}

void * f(void*arg){
    
    int fila = *(int*)arg,x,j;

	for(j = 0; j < N; j++){
			m_r[fila][j]=0;
		for(x = 0; x < N; x++){
		    m_r[fila][j] = m_r[fila][j] + (m_a[fila][x] * m_b[x][j]);
		}
	}	

	return NULL;
}
