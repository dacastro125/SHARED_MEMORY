#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    typedef struct{
        int COM;
        int OP;
        int Time;
    }msg;

    pid_t *childs,padre=getpid();
    msg *mensaje;
    int shmid,flag,i,sw=0;

    //mensaje = (msg*) malloc(sizeof(msg));

    shmid = shmget(IPC_PRIVATE,sizeof(msg),0666);
    mensaje = (msg*) shmat(shmid,NULL,0);

    childs = (pid_t*) malloc(sizeof(pid_t)*5);

    for(i=0;i<5;i++){
        if(!(childs[i]=fork())){
            flag = (i+2);
            break;
        }
    }

    if(padre==getpid()){
        float segundos;
        for(;;){
            printf("\nIngrese de esta forma #-#-# los datos: ");
            scanf("%d-%d-%d",&mensaje->COM,&mensaje->OP,&mensaje->Time);
            /*printf("Ingrese al compuerta: ");
            scanf("%d",&mensaje->COM);
            printf("Ingrese la accion: ");
            scanf("%d",&mensaje->OP);
            printf("Ingrese el tiempo: ");
            scanf("%d",&mensaje->Time);*/
            //if(wait(NULL)>0){
                //shmctl(shmid,IPC_RMID,0);
            //}
            if(mensaje->Time == 0){
                segundos = 1;
            }else if(mensaje->Time == 1){
                segundos = 1;
            }else if(mensaje->Time == 2){
                segundos = 11;
            }
            sleep(segundos);
        }
    }else{
        for(;;){
            //sleep(5);
            if(mensaje->COM == (flag-1)){
                float segundos;
                if(sw!=mensaje->OP){
                    sw = mensaje->OP;
                    if(mensaje->Time == 0){
                        segundos = 0;
                    }else if(mensaje->Time == 1){
                        segundos = 0.5;
                    }else if(mensaje->Time == 2){
                        segundos = 10;
                    }


                    if(mensaje->OP == 1){
                        printf("La compuerta %d se abrira en %f segundos\n",mensaje->COM,segundos);
                        usleep(segundos*1000000);
                        printf("Compuerta %d abierta\n",mensaje->COM);
                    }else{
                        printf("La compuerta %d se cerrara en %f segundos\n",mensaje->COM,segundos);
                        usleep(segundos*1000000);
                        printf("Compuerta %d cerrada\n",mensaje->COM);
                    }
                }else{
                    char output [50];
                    if(sw==1){
                        strcpy(output,"Compuerta ya estaba abierta");
                    }else{
                        strcpy(output,"Compuerta ya estaba cerrada");
                    }
                    printf("%s\n",output);
                }
                mensaje->COM = 0;
                //break;
            }
        }

    }

    // ipcs
    // ipcrm -a

    return 0;
}
