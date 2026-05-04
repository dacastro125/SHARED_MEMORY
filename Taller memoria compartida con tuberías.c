#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define MAX_BUF_SIZE 50
#define MAX_VECTOR_SIZE 50

void writemsg(int *tb, const char *message) {
    write(tb[1], message, strlen(message));
    //printf("[%d] Escrito: %s\n", getpid(), message);
}

void readmsg(int *tb, char *message) {
    int n = read(tb[0], message, MAX_BUF_SIZE);
    message[n] = '\0';
    //printf("[%d] Leido: %s\n", getpid(), message);
}

void writeDouble(int* tb,double num){
    write(tb[1],&num,sizeof(double));
    //printf("[%d] Escrito: %lf\n",getpid(),num);
}

double readDouble(int* tb){
    double num;
    read(tb[0],&num,sizeof(double));
    //printf("[%d] Leido: %lf\n",getpid(),num);
    return num;
}

void writeDoubleArray(int* tb,double vector[],int n){
    write(tb[1],vector,sizeof(double)*n);
}

void readDoubleArray(int* tb,double vector[],int n){
    read(tb[0],vector,sizeof(double)*n);
}

double sacarPromedio(double vector[],int n){
    double suma = 0;
    int i = 0;
    for(i;i<n;i++){
        suma+=vector[i];
    }
    return suma/n;
}

double buscarMayor(double vector[],int n){
    double mayor = 0;
    int i = 0;
    for(i;i<n;i++){
        if(mayor < vector[i]){
            mayor = vector[i];
        }
    }
    return mayor;
}

double buscarRepetido(double vector[], int n){
int aux[n][2];

    int c = 0;

    int i = 0;
    for (; i < n; ++i) {
        int j = 0;
        for (; j < c; ++j) {
            if (aux[j][0] == vector[i]) {
                aux[j][1]++;
                break;
            }
        }

        if (j == c) {
            aux[c][0] = vector[i];
            aux[c][1] = 1;
            c++;
        }
    }

    int mayor = 0;
    for (i = 1; i < c; ++i) {
        if (aux[i][1] > aux[mayor][1]) {
            mayor = i;
        }
    }

    return aux[mayor][0];
}

int main(int argc, char* arcv[]){

    int tb12[2]; int tb21[2];
    int tb13[2]; int tb31[2];
    int tb14[2]; int tb41[2];
    pipe(tb12); pipe(tb21);
    pipe(tb13); pipe(tb31);
    pipe(tb14); pipe(tb41);

    if(fork()){

        if(fork()){

            if(fork()){

                printf("[%d] Soy el padre.\n",getpid());
                close(tb12[0]); close(tb21[1]);
                close(tb13[0]); close(tb31[1]);
                close(tb14[0]); close(tb41[1]);

                FILE *lectura;
                lectura = fopen("EjemploSHM.txt","r");

                double vector[MAX_VECTOR_SIZE];
                int n = 0;

                while(!feof(lectura)){
					double num = 0.0;
					fscanf(lectura,"%lf",&num);
					vector[n++] = num;
				}

				fclose(lectura);

				writeDouble(tb12,(n-1));
				writeDoubleArray(tb12,vector,n);
				writeDouble(tb13,(n-1));
				writeDoubleArray(tb13,vector,n);
				writeDouble(tb14,(n-1));
				writeDoubleArray(tb14,vector,n);

				double promedio = 0,mayor = 0,repetido = 0;
                promedio = readDouble(tb21);
                mayor = readDouble(tb31);
                repetido = readDouble(tb41);
				printf("[%d] El promedio es de %lf\n",getpid(),promedio);
				printf("[%d] El numero mayor es: %lf\n",getpid(),mayor);
				printf("[%d] El numero mas repetido es: %lf\n",getpid(),repetido);

            }else{

                printf("[%d] Soy el hijo 3.\n",getpid());
                close(tb14[1]); close(tb41[0]);
                close(tb12[0]); close(tb12[1]);
                close(tb21[0]); close(tb21[1]);
                close(tb13[0]); close(tb13[1]);
                close(tb31[0]); close(tb31[1]);

                int n = 0;
                n = (int)readDouble(tb14);
                double vector[MAX_VECTOR_SIZE],repetido = 0;
                readDoubleArray(tb14,vector,n);
                repetido = buscarRepetido(vector,n);
                writeDouble(tb41,repetido);
            }

        }else{

            printf("[%d] Soy el hijo 2.\n",getpid());
            close(tb12[0]); close(tb12[1]);
            close(tb21[0]); close(tb21[1]);
            close(tb13[1]); close(tb31[0]);
            close(tb14[0]); close(tb14[1]);
            close(tb41[0]); close(tb41[1]);

            int n = 0;
            n = (int)readDouble(tb13);
            double vector[MAX_VECTOR_SIZE],mayor = 0;
            readDoubleArray(tb13,vector,n);
            mayor = buscarMayor(vector,n);
            writeDouble(tb31,mayor);
        }

    }else{

        printf("[%d] Soy el hijo 1.\n",getpid());
        close(tb12[1]); close(tb21[0]);
        close(tb13[0]); close(tb13[1]);
        close(tb31[0]); close(tb31[1]);
        close(tb14[0]); close(tb14[1]);
        close(tb41[0]); close(tb41[1]);

        int n = 0,i = 0;
        n = (int)readDouble(tb12);
        double vector[MAX_VECTOR_SIZE],promedio = 0;
        readDoubleArray(tb12,vector,n);
        promedio = sacarPromedio(vector,n);
        writeDouble(tb21,promedio);

    }
}
