#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(){
    char archivo[50] = "/home/felipe/Escritorio/Tallersharememory/xd";
    int i,n,num_procesos,num_iteraciones;
    int *turno,*vector;
    float *result;
    int shm_id_turno,shm_id_vector,shm_id_result;

    printf("Ingrese el numero de procesos: ");
    scanf("%d",&num_procesos);

    printf("Ingrese el numero de iteraciones: ");
    scanf("%d",&num_iteraciones);

    shm_id_turno = shmget(IPC_PRIVATE,sizeof(int),0600);
    turno = shmat(shm_id_turno,NULL,0);

    shm_id_result = shmget(IPC_PRIVATE,2*sizeof(float),0600);
    result = shmat(shm_id_result,NULL,0);

    FILE *file = fopen(archivo,"r");
    if(file == NULL){
        perror("No se encontro el archivo");
        exit(-1);
    }

    int c = 0;
    fscanf(file,"%d",&n);
    while (!feof(file))
    {
        fscanf(file,"%d",&n);
        c++;
    }
    fseek(file,0,0);

    shm_id_vector = shmget(IPC_PRIVATE,sizeof(int),0600);
    vector = shmat(shm_id_vector,NULL,0);

    *turno = 0;

    for (i = 0; i < num_procesos; i++)
    {
        if(!fork()){
            for (int j = 0; j < num_iteraciones; j++)
            {
                if (i==0){
                    while(*turno!=0) usleep(100);
                    printf("Soy el proceso [%d] y realizo la suma en la iteracion %d\n",getpid(),j);
                    float suma = 0;
                    for (int d = 0; d < c; d++)
                    {
                        suma += vector[d];
                    }
                    result[0] = suma;
                    *turno = 1;
                    
                }else if(i==1){
                    while(*turno!=1) usleep(100);
                    printf("Soy el proceso [%d] y saco el promedio en la iteracion %d\n",getpid(),j);
                    float promedio = result[0]/c;
                    result[1] = promedio;
                    *turno = 0;
                }
            }
            exit(0);
        }
    }
    
    if(i==num_procesos){
        printf("Soy el padre [%d] y cargo los elementos del archivo al vector\n",getpid());
        for (int a = 0; a < c; a++) 
        {
            fscanf(file,"%d",&vector[a]);
        }
        fclose(file);
        *turno = 0;

        for (int  b = 0; b < num_procesos; b++)
        {
            wait(NULL);
        }

        printf("\nSoy el padre y muestro los resultados: \n");
        printf("La suma de los elementos es: %.2f\n",result[0]);
        printf("El promedio de los elementos es: %.2f\n",result[1]);
        
        shmctl(shm_id_result,IPC_RMID,0);
        shmctl(shm_id_turno,IPC_RMID,0);
        shmctl(shm_id_vector,IPC_RMID,0);
        
    }

    

    return 0;
}