#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <wait.h>
#include <math.h>



int main(int argc, char const * argv){
    int **fd,n,sizeBuffer;
    FILE *file;
    int k=0,flag,cDigit=0,cLetters=0;
    char caracter, *buffer;
    pid_t *pid, padre=getpid();

    printf("Ingrese la cantidad de procesos: ");
    scanf("%d",&n);

    fd = (int**) malloc((n+1)*sizeof(int*));
    for(int i=0;i<=n;i++){
        fd[i] = (int*) malloc(2*sizeof(int));
        pipe(fd[i]);
    }

    pid = (pid_t*) malloc(n*sizeof(pid_t));

    for(int i=0;i<n;i++){
        if(!(pid[i]=fork())){
            flag = (i+2);
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
        file = fopen("archivo.txt","r");
        fseek(file,0,SEEK_END);
        sizeBuffer = ftell(file);

        buffer = (char*) malloc(sizeBuffer*sizeof(char));

        fseek(file,0,SEEK_SET);

        while((caracter = fgetc(file)) != EOF){
            buffer[k] = caracter;
            k++;
        }

        close(fd[0][0]);
        for(int z=1;z<(n+1);z++){
            close(fd[z][1]);
            close(fd[z][0]);
        }

        write(fd[0][1],&sizeBuffer,sizeof(int));
        for(int i=0;i<sizeBuffer;i++){
            write(fd[0][1],&buffer[i],sizeof(char));
        }

        while(wait(NULL)>0);        
    }else{
        int intervalo,desde;
        close(fd[flag-2][1]);
        close(fd[flag-1][0]);
        for(int i=0;i<(n+1);i++){
            if((i!=(flag-1)) && (i!=(flag-2))){
                close(fd[i][0]);
                close(fd[i][1]);
            }
        }

        read(fd[flag-2][0],&sizeBuffer,sizeof(int));
        buffer = (char*) malloc(sizeBuffer*sizeof(char));
        for(int i=0;i<sizeBuffer;i++){
            read(fd[flag-2][0],&buffer[i],sizeof(char));
        }

        float lawea2 = (float) sizeBuffer/ (float)n;
        float papulawea = nearbyintf(lawea2);


        intervalo = (papulawea)*(flag-1);
        desde = (papulawea)*(flag-2);
        printf("Proceso %d leyo: ",getpid());
        for(int j=((papulawea)*(flag-2));j<intervalo;j++){
            printf("%c",buffer[j]);
            if(isdigit(buffer[j])){
                cDigit++;
            }else if(isalpha(buffer[j])){
                cLetters++;
            }
        }
        printf("\n");
        printf("La cantidad de letras leidas por el proceso [%d] es %d y la de numeros es %d\n",getpid(),cLetters,cDigit);
        printf("\n");

        write(fd[flag-1][1],&sizeBuffer,sizeof(int));
        for(int i=0;i<sizeBuffer;i++){
            write(fd[flag-1][1],&buffer[i],sizeof(char));
        }

    }


    return 0;
}