#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <time.h>

int vectorG[16]={31,23,14,26,8,36,4,21,4,7,1,43,32,12,21,7},N=16,TamC;

void * f(void*arg);

int main(){
	
	
	pthread_t *h;
	int HiCa,NumMax,NumMin,i,j;
	
	/*printf("Ingrese el tamaño del vector");
	scanf("%d",&N);
	vectorG=(int *) calloc(N, sizeof(int));
	
	srand(time(NULL));
    for(i=0; i<N; i++){    
        vectorG[i]= rand()%50;
        printf("[%d]",vectorG[i]);
    }*/
    	
	printf("\n\nIngrese el tamaño de los casilleros");
    scanf("%d",&TamC);
	
	NumMax = vectorG[0];
	NumMin = vectorG[0];
	
	for( i=1; i<N; i++){
		if(vectorG[i]>NumMax){ NumMax = vectorG[i]; }
		if(vectorG[i]<NumMin){ NumMin = vectorG[i]; }
	}
	
	HiCa=((NumMax-NumMin)/TamC);
	h = (pthread_t*)calloc(HiCa,sizeof(pthread_t));
	
	int *l,laux = NumMin;
    for ( i = 0; i < HiCa; i++){
        l = (int*)malloc(sizeof(int)); 
        *l = laux;
        laux =  laux + TamC + 1;
       // printf(" limites hilo %d  %d : %d\n",i,*l,*l+TamC);
        pthread_create(&h[i], NULL, f, (void*)l);
    }
    
	int *Cas;
    for(i=0;i<HiCa;i++){        
	    pthread_join(h[i],(void*)&Cas);
	    //printf("\n");
	    for(j=0;j<N;j++){
	    	if(Cas[j]!=0){
			    printf("%d  ",Cas[j]);
			}
		}
     }
	
	
	return 0;
}


 
void * f(void*arg){
	
	int li = *(int*)arg;
    int ls = li + TamC;
    
	int *vaux,i,j,a=0;
 
    vaux = (int*)calloc(N,sizeof(int));
    for(i=0; i<N; i++){ vaux[i]=0; } 
    
    for(i=0; i<N; i++){ 
       if( vectorG[i]>=li && vectorG[i]<=ls){
		  vaux[a] = vectorG[i];
		  a++;
	   }
    } 
     
    int aux; 
    for(i=0;i<N;i++){
        for(j=i;j<N;j++){
            if(vaux[i]>vaux[j]){
               aux=vaux[j];
               vaux[j]=vaux[i];
			   vaux[i]=aux;
            }
        }
    } 
     
     
    return vaux; 	
}


