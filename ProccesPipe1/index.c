#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int main(int argc, char const *argv[]){

    pid_t pr1hjs[2],padre=getpid();
    FILE* file;
    int fd[2],fd2[2],i,flag,readbytes;
    int enteros[13];
    char caracter;
    char buffer [100];
    
    pipe(fd);
    pipe(fd2);
    
    for(i=0;i<2;i++){
        pr1hjs[i]=fork();
        if(pr1hjs[i]==0){
            flag=(i+2);
            break;
        }
    }

    if(padre==getpid()){
        char b[50];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
    }else{
        sleep(1);
    }

    if(padre==getpid()){
        sleep(1);
        file = fopen("archivo.txt","r");
        int c=0,b=0;
        char *pt;
        while((caracter = fgetc(file)) != EOF){
            buffer[c] = caracter;
            c++;
        }
        pt = strtok(buffer,",");
        while(pt != NULL){
            enteros[b]=atoi(pt);
            pt = strtok(NULL,",");
            b++;
        }
        close(fd[0]);
        close(fd2[0]);
        write(fd[1],&enteros,sizeof(enteros)+1);
        write(fd2[1],&enteros,sizeof(enteros)+1);
        close(fd[1]);
        close(fd2[1]);
        while(wait(NULL)>0);
    }else if(flag==2){
        sleep(1);
        close(fd[1]);
        read(fd[0],&enteros,sizeof(enteros)+1);
        printf("Numeros pares desde proceso hijo:");
        for(int a=0;a<(sizeof(enteros)/sizeof(int));a++){
            if(enteros[a]%2==0){
                printf("\n%d,",enteros[a]);
            }
        }
        printf("\n");
        close(fd[0]);
    }else if(flag==3){
        sleep(2);
        close(fd2[1]);
        read(fd2[0],&enteros,sizeof(enteros)+1);
        printf("Numeros impares desde proceso hijo:");
        for(int a=0;a<(sizeof(enteros)/sizeof(int));a++){
            if(enteros[a]%2!=0){
                printf("\n%d,",enteros[a]);
            }
        }
        printf("\n");
        close(fd2[0]);
    }





    return 0;
}
