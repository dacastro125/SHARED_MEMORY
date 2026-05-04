#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

typedef struct msj{
  int com;
  int op;
  int ti;
  int sw;
}msj;


void ejecutar(struct msj * x){
  printf("Compuerta %d ",x->com);
  if(x->op==1){ printf("se abrira "); }
  else{ printf("se cerrara "); }
 int t;
  if(x->ti==0){ printf("inmediatamente"); t=0; }
  else if(x->ti==1){ printf("en 5 seg."); t = 5; }
  else { printf("en 10 seg."); t=10; }
  printf("\n");
  usleep(1000000*t);
  if(x->op==1){ printf("\n abierta\n "); }
  else{ printf("\n cerrada\n "); }

}

int main(int argc, char const *argv[]) {

  int shmid;
  msj *dt;
  shmid=shmget(IPC_PRIVATE,sizeof(msj),IPC_CREAT|0666);
  if (shmid==-1){
        printf("Error creando la memoria compartida");
  }
  dt=shmat(shmid,NULL,0);

  if (dt==(void*)-1)
  {
    printf("Error al anexar la memoria compartida");
  }

    if(fork() == 0)
    {
      while( dt->sw != 1  ){}
      if(dt->com==1){
        ejecutar(dt);
      }

    }
    else if(fork() == 0)
    {
      while( dt->sw != 1  ){}
      if(dt->com==2){
        ejecutar(dt);
      }
    }
    else if(fork() == 0)
    {
      while( dt->sw != 1  ){}
      if(dt->com==3){
        ejecutar(dt);
      }
    }
    else if(fork() == 0)
    {
      while( dt->sw != 1  ){}
      if(dt->com==4){
        ejecutar(dt);
      }
    }
    else if(fork() == 0)
    {
      while( dt->sw != 1  ){}
      if(dt->com==5){
        ejecutar(dt);
      }
    }
    else
    {
      dt->com =4;
      dt->op=1;
      dt->ti=2;
      dt->sw=1;



      wait(NULL);
      wait(NULL);
      wait(NULL);
      wait(NULL);
      wait(NULL);
    }


  return 0;
}
