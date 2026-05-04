#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/shm.h>

void abortar(){
    printf("Abortar Mision.\n");
}

void lanzarCapsula(){
    printf("Capsula Lanzada.\n");
}

void error(char* mensaje){
    printf("%s",mensaje);
    exit(1);
}
void manejador(){

}
int main(){
        signal(SIGUSR1,manejador);

        srand(time(NULL));
        int shmid = shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0666);
        if(shmid == -1){
            error("Error obteniendo la memoria");
        }
        void *shm_ptr = shmat(shmid,NULL,0);

        if(shm_ptr == (void*)-1){
            error("Error adjutando la memoria");
        }

        int *senal = (int*)shm_ptr;
        *senal = 0;
        int hijo1,hijo2;

        if((hijo1 = fork())){
            if((hijo2 = fork())){
                usleep(1000);
                printf("[%d] Soy el padre\n",getpid());

                time_t inicio = time(NULL);
                time_t _final = inicio;
                do{
                    _final = time(NULL);
                    kill(hijo1,SIGUSR1);

                    pause();
                    if(*senal == 1){
                        kill(hijo2,SIGUSR1);
                        kill(hijo1,SIGUSR1);
                        break;
                    }

                }while((_final-inicio)<60);

                if(*senal == 0){
                    *senal = 1;
                    kill(hijo2,SIGUSR1);
                    kill(hijo1,SIGUSR1);
                }
                pause();
            }else{
                printf("[%d] Soy el hijo 2\n",getpid());

                pause();
                if(*senal == 1){
                    abortar();
                    lanzarCapsula();
                }else{
                    lanzarCapsula();
                }
                kill(getppid(),SIGUSR1);
            }
        }else{
            printf("[%d] Soy el hijo 1\n",getpid());
            int i = 0;
            int buster[4];
            int respuesta = 0;

            pause();
            while(*senal == 0){
                for(i = 0;i<4;i++){
                    buster[i] = rand()%7;
                    if(buster[i]>5){
                        *senal = 1;
                        break;
                    }
                }
                kill(getppid(),SIGUSR1);
                pause();
            }
        }

    return 0;
}
