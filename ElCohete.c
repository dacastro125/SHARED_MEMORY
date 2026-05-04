#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void escribirEntero(int *fd,int num){
    write(fd[1],&num,sizeof(int));
}

int leerNumero(int *fd){
    int num = 0;
    read(fd[0],&num,sizeof(int));
    return num;
}

void abortar(){
    printf("Abortar Mision.\n");
}

void lanzarCapsula(){
    printf("Capsula Lanzada.\n");
}

int main(){

        srand(time(NULL));
        int tb12[2]; int tb21[2];
        int tb13[2]; int tb31[2];
        pipe(tb12);  pipe(tb21);

        if(fork()){
            pipe(tb13); pipe(tb31);
            if(fork()){
                printf("[%d] Soy el padre\n",getpid());
                close(tb12[0]);
                close(tb21[1]);
                close(tb13[0]);
                close(tb31[1]);

                long inicio = time(NULL);
                long _final = inicio;
                int senal = 0;
                do{
                    _final = time(NULL);
                    escribirEntero(tb12,0);

                    senal = leerNumero(tb21);

                    if(senal == 1){
                        escribirEntero(tb13,1);
                        escribirEntero(tb12,1);
                        break;
                    }else{
                        escribirEntero(tb12,0);
                    }
                }while((_final-inicio)<5);

                if(senal == 0){
                    escribirEntero(tb13,senal);
                    escribirEntero(tb12,1);
                }
                wait(NULL);
                wait(NULL);
            }else{
                printf("[%d] Soy el hijo 2\n",getpid());
                close(tb12[0]);
                close(tb12[1]);
                close(tb21[0]);
                close(tb21[1]);
                close(tb13[1]);
                close(tb31[0]);

                int senal = leerNumero(tb13);
                if(senal == 1){
                    abortar();
                    lanzarCapsula();
                }else{
                    lanzarCapsula();
                }
            }
        }else{
            printf("[%d] Soy el hijo 1\n",getpid());
            close(tb12[1]);
            close(tb21[0]);
            int buster[4];
            int respuesta = 0;
            int i = 0;

            int senal = leerNumero(tb12);
            while(senal == 0){
                for(i = 0;i<4;i++){
                    buster[i] = rand()%7;
                    if(buster[i]>5){
                        respuesta = 1;
                        break;
                    }
                }
                escribirEntero(tb21,respuesta);
                senal = leerNumero(tb12);
            }
        }

    return 0;
}
