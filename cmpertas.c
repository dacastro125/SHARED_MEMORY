 #include <stdio.h>
#include <unistd.h>
#include <string.h>
struct msj
{
int com;
int op;
int t;

};
int main(){
pid_t padre;
int fd[2], n, fd1[2],i,fd2[2],fd3[2],j,s,ok,vec[10];
char buff[50];
struct msj orden;
pipe(fd);
pipe(fd1);
pipe(fd2);
pipe(fd3);
for (i=0; i<4; i++)
{
if((vec[i]=fork())==0)
{
break;
}
if(i==3)
{
if(orden.com==1)
{
 printf("\n hijo  2 %d Recibe:\n",getpid());

//tuberia 2
    close (fd1[1]);
    n= read(fd1[0], &orden, sizeof(struct msj));
    buff[n]='\0';
    if(orden.op==0)
    {
    printf("Cerrando en %d segundos\n",orden.t);
    usleep((orden.t)*100000);
       }
       else
       {
            if(orden.op==1)
            {
            printf("Abriendo en %d segundos\n",orden.t);
            usleep((orden.t)*100000);
             }
       }
    break;
}
}
}
if(padre==getpid()){
printf("Padre: %d\n", getpid());
//wait(NULL);
//tuberia 2
    close (fd[1]);
    n= read(fd[0], &orden, sizeof(struct msj));
    buff[n]='\0';
    printf("%d %d %d: \n",orden.com,orden.op,orden.t);
}
else
{
for (i=0; i<4; i++)
{
if(vec[i]==0)
{
if(i==0)
{
printf("Digite la Compuerta\n");
scanf("%d",&orden.com);
printf("Digite la Operacion \n");
scanf("%d",&orden.op);
printf("Didgite la Tiempo\n");
scanf("%d",&orden.t);
//tuberia 1
    close(fd[0]);

    write(fd[1], &orden, sizeof(struct msj));
    printf("id: %d \n", getpid());
}
    if(orden.com==1)
    {
    close(fd1[0]);
    write(fd1[1], &orden, sizeof(struct msj));
   // printf("id: %d \n", getpid());
    break;
    }

}
}

}



return 0;
}
