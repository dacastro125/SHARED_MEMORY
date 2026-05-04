#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

long double calcularPI();
long double factorial();
long double bbp();

void manejador(){
}

void error(char* mensaje){
    printf("%s",mensaje);
    exit(1);
}
int main(){
    signal(SIGUSR1,manejador);

    int shmid = shmget(IPC_PRIVATE,sizeof(long double),IPC_CREAT|0666);
    if(shmid == -1){
        error("Error obteniendo la memoria");
    }
    void *shm_ptr = shmat(shmid,NULL,0);
    if(shm_ptr == (void*)-1){
        error("Error adjuntando la memoria");
    }

    long double* datos = (long double*)shm_ptr;

    int n = 0,i = 0;
    printf("Ingrese la cantidad de iteraciones: ");
    scanf("%d",&n);
    int hijos[n];

    for(i = 0;i<n;i++){
        if(!(hijos[i] = fork())){
            break;
        }
    }
    if(i == n){
        *datos = 0.0;
        usleep(1000);
        kill(hijos[n-1],SIGUSR1);
        pause();
        long double suma = *datos;
        long double pi = 1/suma;
        printf("El valor de pi es: %.50llf",pi);
        shmctl(shmid,IPC_RMID,NULL);

    }else{
        pause();
        long double res = calcularPI(i);
        printf("%d , %.50llf\n",i,res);
        *datos = *datos + res;
        if(i == 0){
            kill(getppid(),SIGUSR1);
        }else{
            kill(hijos[i-1],SIGUSR1);
        }
        shmdt(shm_ptr);
    }

    return 0;
}

long double calcularPI(int k){
    return 12*((pow(-1,k)*factorial(6*k)*(13591409+545140134*k))/(factorial(3*k)*pow(factorial(k),3)*pow(640320,3*k+(3.0)/2)));
}

long double factorial(int k){
  if (k == 0) {
        return 1;
    } else {
        return k*factorial(k - 1);
    }
}

long double bbp(int k){
    return (4.0/(8*k+1)-2.0/(8*k+4)-1.0/(8*k+5)-1.0/(8*k+6))*1.0/(pow(16,k));
}
