#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>

int a[6][6],b[6][6],c[6][6];

void inicializar(int matriz[6][6],int tam,int dato){

    for(int i=0;i<tam;i++){
        for(int j=0;j<tam;j++){
            matriz[i][j]=dato;
            dato+=1;
        }
    }
}

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
    size_t size = rows * sizeof (void *); // index size
    size += (cols * rows * sizeElement); // Data size
    return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement){
    size_t sizeRow = cols * sizeElement;
    m[0] = m + rows;
    for (int i=1; i<rows; i++){
        m[i] = (m[i-1] + sizeRow);
    }
}

void print_matrix(int **m, int rows, int cols){
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            printf("%d\t", m[r][c]);
        }
        printf("\n");
    }
}

void multiplicar(int x,int y,int **m,int N ){
    int k;

    c[x][y]=0;
    for(k=0; k<N; k++)
        m[x][y] += a[x][k] * b[k][y];
    
}
void recorrido(int l,int **m,int tam){

    int inicio=l,final=tam-l;
    //arriba
    for(int i=inicio;i<final;i++){
        multiplicar(l,i,m,tam);
    }
    
    //derecha
    for(int i=inicio;i<(tam-l);i++){
        multiplicar(i,(tam-l-1),m,tam);
    }
    
    //izquierda
    for(int i=inicio;i<final;i++){
        multiplicar(i,l,m,tam);
    }
    //abajo
    for(int i=inicio;i<final;i++){
        multiplicar((tam-l-1),i,m,tam);
    }
}


int main(int argc, char const *argv[]){
    inicializar(a,6,2);
    inicializar(b,6,3);
    int shm_id, tam=6;
    int **m;
    size_t sizeMatrix = sizeof_dm(tam, tam, sizeof(int));
    shm_id = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0600);
    m = shmat(shm_id, NULL, 0);
    create_index((void*)m, tam, tam, sizeof(int));

    int Nhijos=tam%2;
    Nhijos=(tam/2)+Nhijos;
    for(int i=0;i<Nhijos;i++){

        if(!fork()){
            printf("\t\t\tProceso Hijo [%d]\n", getpid());
            recorrido(i,m,tam);
            print_matrix(m, tam, tam);
            shmdt(m);       

            return 0;
        }
    }
    
    for(int i=0;i<Nhijos;i++)
        wait(NULL);
    printf("\t\t\tProceso Padre [%d]\n", getpid());
    print_matrix(m, tam, tam);
    shmdt(m);
    shmctl(shm_id, IPC_RMID, 0);
    
    return 0;
}
