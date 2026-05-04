#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd[2][2],hijos[2]={-1},n,x;
    char buffer[50];
    pipe(fd[0]);
    pipe(fd[1]);
    printf("Digite un mensaje(Maximo 50 caracteres):\n");
    scanf("%s",buffer);
    for(x=0; x<2 ; x++){
        if(!(hijos[x]=fork())){
           break;
}
}

    if(x == 2){
      close(fd[0][0]);//Cierro lectura de tuberia 1
      close(fd[1][0]);//Cierro escritura de tuberia 1
      close(fd[1][1]);//Cierro escritura de tuberia 2
      write(fd[0][1],buffer,strlen(buffer));//Escribo a la tuberia 1
      printf("%d Padre envio:%s\n",getpid(),buffer);//
      wait(NULL);
      wait(NULL);
}
    if(x == 0){
      close(fd[0][1]);
      close(fd[1][0]);
      n=read(fd[0][0],buffer,50);
      buffer[n]='\0';
      printf("%d Hijo[1]:%s\n",getpid(),buffer);
      write(fd[1][1],buffer,strlen(buffer));
}
    if(x==1){
    close(fd[0][0]);
    close(fd[0][1]);
    close(fd[1][1]);
    n=read(fd[0][0],buffer,50);
    buffer[n]='\0';
    printf("%d Hijo[2]:%s\n",getpid(),buffer);

}


return 0;

}
        
