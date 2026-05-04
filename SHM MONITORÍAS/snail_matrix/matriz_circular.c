#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/shm.h>

void mat_mult(int i, int j, int dim_x, int dim_y, int**A, int**B, int**R){
    for(int kk = 0; kk < dim_x; kk++){
        R[i][j] += A[i][kk] + B[kk][j];
    }
}

int main(int argc, char** argv){

    int F = 5;
    int C = 5;
    int shmidA = shmget(IPC_PRIVATE, F*C*sizeof(int) + F*sizeof(int), 0666 | IPC_CREAT);
    void* baseA = shmat(shmidA, NULL, 0);
    int ** A = (int**)baseA;
    int * dA = (int*)((char*)baseA + F*sizeof(int*));
    for(int i = 0; i < F; i++){
        A[i] = &dA[i*C];
    }

    int shmidB = shmget(IPC_PRIVATE, F*C*sizeof(int) + F*sizeof(int), 0666 | IPC_CREAT);
    void* baseB = shmat(shmidB, NULL, 0);
    int ** B = (int**)baseB;
    int * dB = (int*)((char*)baseB + F*sizeof(int*));
    for(int i = 0; i < F; i++){
        B[i] = &dB[i*C];
    }

    int shmidC = shmget(IPC_PRIVATE, F*C*sizeof(int) + F*sizeof(int), 0666 | IPC_CREAT);
    void* baseC = shmat(shmidC, NULL, 0);
    int ** R = (int**)baseC;
    int * dR = (int*)((char*)baseC + F*sizeof(int*));
    for(int i = 0; i < F; i++){
        R[i] = &dR[i*C];
    }

    for(int i = 0; i < F; i++){
        for(int j = 0; j < C; j++){
            A[i][j] = i + j;
            B[i][j] = i + j + 1;
            R[i][j] = 0;
        }
    }
    pid_t padre = getpid();

    int n_h = (int)(F+1)/2;

    int i;
    for(i = 0; i < n_h; i++){
        if(!fork()) break;
    }

    if(padre == getpid()){ //padre

        for(int i = 0; i < n_h; i++) wait(NULL);

        for(int i = 0; i < F; i++){
            for(int j = 0; j < C; j++){
                printf("%d ", R[i][j]);
            }
            printf("\n");
        }


    }else{ //Hijo

        for(int ii = i; ii < F-i; ii++){
            for(int jj = i; jj < C-i; jj++){
                if(ii == i || ii == F - i - 1 || jj == i || jj == C-i-1){
                    mat_mult(ii,jj,F,C,A,B,R);
                }
            }
        }
    }

    return 0;
}