#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/shm.h>
#include <ctype.h>
#include <string.h>
#include <wait.h>


int main(int argc, char * argv[]){
    int shmid, i,sizeBuffer,k = 0,flag;
    int countNumbers=0,countLetters=0,countCommas=0;
    char *c,caracter;
    pid_t pr1hjs [3], padre=getpid();
    FILE* file;


    file = fopen("archivo.txt","r");
    fseek(file,0,SEEK_END);
    sizeBuffer = ftell(file);
    fseek(file,0,SEEK_SET);
    c = (char*) malloc(sizeBuffer*sizeof(char));
    
    shmid = shmget(IPC_PRIVATE,sizeof(char)*sizeBuffer,0666);
    c = shmat(shmid,NULL,0);

    while((caracter = fgetc(file)) != EOF){
        c[k] = caracter;
        k++;
    }

    for(i=0;i<3;i++){
        pr1hjs[i]=fork();
        if(pr1hjs[i]==0){
            flag=(i+2);
            break;
        }
    }

    if(padre==getpid()){
        shmdt(c);
        while(wait(NULL)>0);
    }

    if(flag==2){
        sleep(1);
        for(int j=0;j<sizeBuffer;j++){
            if(isdigit(c[j])){
                countNumbers++;
            }
        }
        printf("La cantidad de numeros que hay son: %d\n",countNumbers);
        shmdt(c);
    }else if(flag==3){
        sleep(2);
        for(int j=0;j<sizeBuffer;j++){
            if(isalpha(c[j])){
                countLetters++;
            }
        }
        printf("La cantidad de letras que hay son: %d\n",countLetters);
        shmdt(c);
    }else if(flag==4){
        sleep(3);
        for(int j=0;j<sizeBuffer;j++){
            if(!isdigit(c[j]) && !isalpha(c[j])){
                countCommas++;
            }
        }
        printf("La cantidad de comas y puntos que hay son: %d\n",countCommas);
        shmdt(c);
        shmctl(shmid,0,NULL);
    }

    return 0;
}