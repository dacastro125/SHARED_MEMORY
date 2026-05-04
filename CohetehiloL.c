#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

void *fh1(void*);
void *fh2(void*);
int tem_critica=0,sw=0,Buster[4],ran=0,j=0,k=0,termi=0;
int main(){
pthread_t hilo[2];
int i=0,l=0;
printf("Se inicia Lanzamiento\n");
pthread_create(&hilo[0],NULL,fh1,NULL);
pthread_create(&hilo[1],NULL,fh2,NULL);

while(k<5){
  sleep(1);
  if(tem_critica==1){break;}
  k++;
}
if(termi==1)
 {sw=1;}

for(l=0;l<2;l++){
  pthread_join(hilo[l],NULL);
}
return 0;}

void *fh1(void *arg){
  for(;;){
  if(termi==1)
    {
      if(tem_critica==0)
       {printf("Mision Exitosa\n");}
      else{
       printf("Abortar Mision\n");}   
      break;
    }
  }
pthread_exit(NULL);
}
void *fh2(void *arg){
 int sw2=0;
 printf("Se inicia Verificacion de Buster\n");
 for(;;){ 
    for(j=0;j<4;j++){
       ran=1+rand()%4;
       printf("Buster[%d]=%d\n",j+1,ran);   
       if(ran>=5){sw2=1;}
    }
    if(sw2==1){tem_critica=1;break;}
    if(k>=5){break;}
  }      
 termi=1; 
 pthread_exit(NULL);
 }
