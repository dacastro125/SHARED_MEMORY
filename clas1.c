#include <stdio.h>
#include <unistd.h>

struct msj{

  int a;
  float b;
};

int main(){

    int n, fd[2];
    pipe(fd);

    if(fork()==0){
      close(fd[0]);
      struct msj V1;
      V1.a=5;
      V1.b=12.35;
      write(fd[1],&V1,sizeof(struct msj));
}
else{
    close(fd[1]);
    struct msj V2;
    read(fd[0],&V2,sizeof(struct msj));
    printf("a=%d  b=%f\n", V2.a,V2.b); 
    wait(NULL); 

}

return 0;

}

