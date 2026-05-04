#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

#define F 3
#define C 3
#define NH 3

int main(){

    int a[F][C];
    int b[F][C];
    int v[F][C];

    for(int i = 0; i < F; i++){
        for(int j = 0; j < C; j++){
            a[i][j] = i*j + 1;
            b[i][j] = i + j - 1;
            v[i][j] = 0;
        }
    }

    pid_t root = getpid();

    int i = 0;
    for(; i < NH; i++){
        if(!fork()) break;
    }

    if(root == getpid()){

        for(int j = 0; j < NH; j++){
            wait(NULL);
        }

        FILE* f = fopen("result.txt", "r");
        if(!f){
            perror("Error al leer el archivo\n");
            return EXIT_FAILURE;
        }

        int prt[C][F];
        int fi, cl, vl;

        for(int i = 0; i < C*F; i++){
            fscanf(f, "%d,%d,%d\n", &fi,&cl,&vl);
            prt[fi][cl] = vl;
        }

        for(int i = 0; i < F; i++){
            for(int j = 0; j < C; j++){
                fprintf(stdout,"%d ", *(*(prt+i)+j));
            }
            fprintf(stdout,"\n");
        }

    }else{
        FILE* f = fopen("result.txt", "a");
        if(!f){
            perror("No se pudo abrir el archivo\n");
            abort();
        }

        if(i == 0){

            for(int j = 1; j < F; j++){
                for(int h = 0; h < j; h++){
                    for(int k = 0; k < F; k++){
                        v[i][j] += a[j][k] * b[k][h];
                    }
                    fprintf(f, "%d,%d,%d\n", j,h,v[i][j]);
                }
            }

        }else if(i == 1){

            for(int j = 0; j < F-1; j++){
                for(int h = j+1; h < C; h++){
                    for(int k = 0; k < F; k++){
                        v[i][j] += a[j][k] * b[k][h];
                    }
                    fprintf(f, "%d,%d,%d\n", j,h,v[i][j]);
                }
            }

        }else if(i == 2){

            for(int j = 0; j < F; j++){
                for(int k = 0; k < F; k++){
                    v[j][j] += a[j][k] * b[k][j];
                }
                fprintf(f, "%d,%d,%d\n", j,j,v[j][j]);
            }
        }
    }

    return EXIT_SUCCESS;
}
