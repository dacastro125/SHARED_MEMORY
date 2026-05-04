#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <math.h>
#include <time.h>

#define n 10000


int main(void)
{

    struct timespec tic, toc;
    double Elapsed;
    double suma1=0;
    double suma2=0;
    double MSE;
    int i,x,p;
    time_t t;
    int numero = 1;
    double *a, *b;
    a = (double *) calloc(n, sizeof(double));
    b = (double *) calloc(n, sizeof(double));

    int cons = n/numero;
    int limI[numero];
    int limF[numero];
    pid_t pid;
    int h[numero];
    int fd[numero][2];

    for(p=0; p<numero; p++) {
        pipe(fd[p]);
    }

    for(x=0; x<n; x++ ) {
        a[x] = x/(double)(x + 1);
        b[x] = (x + 1)/(double)(x + 2);
        
     
    }

    for(x = 0; x < n; x++) {
    
    }


    printf("-------------\n");

    printf("Soy el proceso %d\n", getpid());

    for(i = 1; i <= numero; i++) {
        limI[i] = i * cons;
        limF[i] = (limI[i] + cons) - 1;
    }

    clock_gettime(CLOCK_REALTIME, &tic);

    for(i = 0; i < numero; i++)
    {
        h[i] = fork();
        
        if(h[i] == 0) {

            printf("soy el hijo %d  %d, mi padre es %d\n",i,getpid(),getppid());

            if(i == 0) {
                close(fd[i][0]);
                
                for(x = 0; x < cons; x++) {
                    double cuadrado = 0;
                    
                    cuadrado = ((a[x] - b[x])*(a[x]-b[x]));
                    suma1 = suma1+cuadrado;
                    
                    
                    
                    write(fd[i][1], &suma1, sizeof(double));
                }
            } else {
                close(fd[i][0]);
                
                for(x = limI[i]; x <= limF[i]; x++) {
                    double cuadrado = 0;
                    
                    cuadrado = ((a[x]-b[x])*(a[x]-b[x]));
                    suma2 = suma2+cuadrado;
                    

                    write(fd[i][1], &suma2, sizeof(double));
                }
            }
            break;
        } else {
            close(fd[i][1]);
            for(x=0; x<n; x++) {
                if(i==0) {
                    read(fd[i][0], &suma1, sizeof(double));
                } else {
                    read(fd[i][0], &suma2, sizeof(double));
                }
            }

            wait(NULL);

            if(i == (numero - 1)) {
                printf("SUMA 1 = %.60f\n",suma1);
                printf("SUMA 2 = %.60f\n",suma2);
                
                MSE=(suma1+suma2)/n;
                
                printf("MSE = %.60f\n",MSE);

                clock_gettime(CLOCK_REALTIME, &toc);
                Elapsed = (toc.tv_sec-tic.tv_sec)+((toc.tv_nsec-tic.tv_nsec)/(double)1E9);
                
                printf("Tiempo: %.10f\n", Elapsed);
            }
        }
    }
    return 0;
}
