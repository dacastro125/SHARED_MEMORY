#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int main(int argc, char const *argv[]){

    pid_t pr1hjs[2],padre=getpid();
    FILE* file;
    int fd[2],fd2[2],i,flag,n=0,lengthFile;
    int *enteros;
    char caracter;
    char *buffer,*buffer2;
    
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
        //sleep(1);
        file = fopen("archivo.txt","r");
        int c=0,b=0;
        char *pt,*ptCount;
        
        fseek(file,0,SEEK_END);
        lengthFile = ftell(file);
        fseek(file,0,SEEK_SET);
        buffer = (char*) calloc(lengthFile,sizeof(char));
        buffer2 = (char*) calloc(lengthFile,sizeof(char));


        while((caracter = fgetc(file)) != EOF){
            buffer[c] = caracter;
            buffer2[c] = caracter;
            c++;
        }
    
        ptCount = strtok(buffer,",");
        while(ptCount != NULL){
            ptCount = strtok(NULL,",");
            n++;
        }

        enteros = (int*) calloc(n,sizeof(int));

        pt = strtok(buffer2,",");
        while(pt != NULL){
            enteros[b]=atoi(pt);
            pt = strtok(NULL,",");
            b++;
        }

        close(fd[0]);
        close(fd2[0]);
        write(fd[1],&n,sizeof(int));
        write(fd2[1],&n,sizeof(int));
        for(int m=0;m<n;m++){
            write(fd[1],&enteros[m],sizeof(int)*n);
        }

        for(int m=0;m<n;m++){
            write(fd2[1],&enteros[m],sizeof(int)*n);
        }
        close(fd[1]);
        close(fd2[1]);
        while(wait(NULL)>0);
    }else if(flag==2){
        sleep(1);
        close(fd[1]);
        close(fd2[0]);
        close(fd2[1]);
        read(fd[0],&n,sizeof(int));
        enteros = (int*) calloc(n,sizeof(int));
        for (int i = 0; i < n; ++i)
            read(fd[0], &enteros[i], sizeof(int));

        printf("Numeros pares desde proceso hijo:");
        for(int a=0;a<n;a++){
            if(enteros[a]%2==0){
                printf("\n%d,",enteros[a]);
            }
        }
        printf("\n");
        close(fd[0]);
    }else if(flag==3){
        sleep(2);
        close(fd2[1]);
        close(fd[0]);
        close(fd[1]);
        read(fd2[0],&n,sizeof(int));
        enteros = (int*) calloc(n,sizeof(int));
        for (int i = 0; i < n; ++i)
            read(fd2[0], &enteros[i], sizeof(int));

        printf("Numeros impares desde proceso hijo:");
        for(int a=0;a<n;a++){
            if(enteros[a]%2!=0){
                printf("\n%d,",enteros[a]);
            }
        }
        printf("\n");
        close(fd2[0]);
    }





    return 0;
}
