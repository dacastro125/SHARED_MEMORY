#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int *vectorG;//Vector global de nuemros
int *vectorF;//Vector contendra el resultado total
int N,TamC,NumMin,NumMax,HiCa,c;// Tamaño del vector,Tamaño de los casilleros,Nummin,Numax,Total de hilos a crear, tamaño de los casilleros nuevos
int rangoI,rangoF;//Rango de los casilleros
int j,acumulador=0;
int Max(int *ve,int n);
int Min(int *ve,int n);
void * f(void*arg);



int main(){
	int i;
	printf("Ingrese el tamaño del vector");
	scanf("%d",&N);
	vectorG=(int *) calloc(N, sizeof(int));
	
    for(i=0; i<N; i++){
       
        vectorG[i]= rand()%50;
    }
    
    
    
	for(i=0; i<N; i++){
        if(i==0){
		printf("VectorG:");
		}
		printf("[%d]",vectorG[i]);
    }
    
    printf("\n");
    NumMax=Max(vectorG,N);
    NumMin=Min(vectorG,N);
    printf("maximo:%d\nminimo:%d\n",NumMax,NumMin);
    
    printf("Ingrese el tamaño de los casilleros");
    scanf("%d",&TamC);
    
    HiCa=((NumMax-NumMin)/TamC);
    pthread_t *pidhilo;
    pidhilo=(pthread_t*)calloc(HiCa,sizeof(pthread_t));

    rangoI=NumMin;
    rangoF=NumMin+TamC;

	int *d;
    for ( i = 0; i < HiCa; i++)
    {
        d=(int*)malloc(sizeof(int));
        *d=i;
        pthread_create(&pidhilo[i], NULL, f, (void*)d);
    }
    
    
     vectorF=(int *) calloc(N, sizeof(int));
     int *Cas;
     for(i=0;i<HiCa;i++){         
     pthread_join(pidhilo[i],&Cas);
    /* 
	 for(j=0;j<c;j++){
     	vectorF[acumulador]=Cas[j];
		acumulador++;
	 }*/
    
    }
    /*for(i=0; i<N; i++){
        if(i==0){
		printf("VectorF:");
		}
		printf("[%d]",vectorF[i]);
    }*/
    
    
    
	
	
	return 0;
}

int Max(int *ve,int n){
	int i;
	int num=ve[0];
	for(i=0;i<n;i++){
		if(num<ve[i]){
			num=ve[i];
		}
		
	}
	return num;
}
int Min(int *ve,int n){
	int i;
	int num=ve[0];
	for(i=0;i<n;i++){
		if(num>ve[i]){
			num=ve[i];
		}
		
	}
	return num;
}


void * f(void*arg){
	
	int h=*(int*)arg;
	int *Casillero;
	int i,j,aux;
	
	pthread_mutex_lock(&mutex);  
    c=0,aux=0;
	Casillero=(int*)calloc(0,sizeof(int));
	for(i=0; i<N; i++){
       if(vectorG[i]>=rangoI && vectorG[i]<=rangoF){
	   Casillero=(int*)realloc(Casillero,(c+1)*sizeof(int));
	   (Casillero)[c]=vectorG[i];
                c++;
	   }
        
    }
    
    printf("Inicio%d-->Fin%d\n",rangoI,rangoF);
	rangoI=(rangoF+1);
	rangoF=(rangoF+TamC)+1;

  	for(i=1;i<c;i++){
        for(j=0;j<c-1;j++){
            if(Casillero[j]>Casillero[j+1]){
               aux=Casillero[j];
               Casillero[j]=Casillero[j+1];
			   Casillero[j+1]=aux;
            }
        }
    }
	  
    for(i=0; i<c; i++){
        if(i==0){
		printf("Casillero:");
		}
		printf("[%d]",Casillero[i]);
    }
    printf("\n\n");
    
	pthread_mutex_unlock(&mutex);
	
	
	return Casillero;
	
}

