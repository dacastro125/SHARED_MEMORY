#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

long double factorial();

void writeDouble(int *fd, long double num){
    write(fd[1],&num,sizeof(long double));
}

long double readDouble(int *fd){
    long double num;
    read(fd[0],&num,sizeof(long double));
    return num;
}

long double casiPi(int k){
    return 12*((pow(-1,k)*factorial(6*k)*(13591409+545140134*k))/(factorial(3*k)*pow(factorial(k),3)*pow(640320,3*k+(3.0)/2)));
}

long double factorial(int k){
  if (k == 0) {
        return 1;
    } else {
        return k*factorial(k - 1);
    }
}

int main(){
    int i = 0, n = 0;
    printf("Ingrese la cantidad de iteraciones de la función: ");
    scanf("%d",&n);
    int tbHP[n][2];
    for(i = 0;i<n;i++){
        pipe(tbHP[i]);
    }
    for(i = 0;i<n;i++){
        if(fork()){
            break;
        }
    }
    if(i == n){
        int j = 0;
        for(j = 0;j<n;j++){
            close(tbHP[j][1]);
        }
        long double suma = 0;
        for(j = 0;j<n;j++){
            long double res =readDouble(tbHP[j]);
            suma += res;
        }
        long double pi = (1/suma);
        printf("\n[%d] Y el valor de pi es: %.51llf",getpid(),pi);
    }else{
        int j = 0;
        for (j = 0; j < n; ++j) {
            if (i != j) {
                close(tbHP[j][1]);
            }
            close(tbHP[j][0]);
        }
        long double casipi = casiPi(i);
        printf("[%d] %.51llf\n",getpid(),casipi);
        writeDouble(tbHP[i],casipi);
    }

    return 0;
}
