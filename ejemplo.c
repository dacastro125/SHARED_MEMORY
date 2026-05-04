#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(){
int fd[2],n;


char buffer_out[1024];
char buffer_in[1024];

pipe(fd);

strcpy(buffer_out,"hola mundo pipes");
write(fd[1],buffer_out,strlen(buffer_out));

n=read(fd[0],buffer_in,1024);
buffer_in[n]='\0';
printf("Leido: %s\n",buffer_in);
return 0;



}
