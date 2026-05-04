#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include <sys/wait.h>

unsigned int sizeof_dm(int rows, int cols, size_t sizeof_element);
void create_index(void **m, int rows, int cols, size_t sizeof_element);

void createSHMs(int *x_shm, int ***X, int rows, int columns);

void showMatx(int **mtx, int rows, int columns);
int multMatx(int **A, int **B, int size, int dx, int dy);
void showChildMatx(int **mtx, int rows, int columns, int child_id, int n);

int main(int argc, char **argv){
    if(argc < 2){
        perror("Send size(n)\n");
        return EXIT_FAILURE;
    }
    int n = atoi(argv[1]);
    int **A, **B, **C;
    int rows, columns;
    int n_children, child_id;
    int mA_shm, mB_shm, mC_shm, *print_lock, lock_shm;
    pid_t root = getpid();

    rows = columns = n;

    createSHMs(&mA_shm, &A, rows, columns);
    createSHMs(&mB_shm, &B, rows, columns);
    createSHMs(&mC_shm, &C, rows, columns);

    lock_shm = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);
    print_lock = shmat(lock_shm, NULL, 0);
    *print_lock = -1;

    n_children = (n % 2 == 0) ? n/2 : n/2+1;

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(getpid() == root){
        int number1 = 2, number2 = 3;
        for(int i=0; i < rows; i++){
            for(int j=0; j < columns; j++){
                A[i][j] = number1 * i-j;
                B[i][j] = number2 * i+j;
            }
        }
        printf("A Matrix\n");
        showMatx(A, rows, columns);
        printf("B Matrix\n");
        showMatx(B, rows, columns);

        *print_lock = 0; //start the process

        for(int i=0; i < n_children; i++) wait(NULL);

        printf("Result Matrix\n");
        showMatx(C, rows, columns);

        shmdt(A);
        shmdt(B);
        shmdt(C);
        shmdt(print_lock);
        shmctl(lock_shm, IPC_RMID, 0);
        shmctl(mA_shm, IPC_RMID, 0);
        shmctl(mB_shm, IPC_RMID, 0);
        shmctl(mC_shm, IPC_RMID, 0);
    }else{
        usleep(10000);

        //printf("Child [%d]:\n", child_id);
        for(int i=child_id; i < n-child_id; i++){ //rows operations
            //printf("pos: [%d][%d]\n", child_id, i);
            C[child_id][i] = multMatx(A, B, n, child_id, i);
            C[n - child_id - 1][i] = multMatx(A, B, n, n-child_id-1, i);
            //printf("pos: [%d][%d]\n", n-child_id-1, i);
        }

        for(int i=child_id+1; i < n-child_id-1; i++){ //columns operations
            C[i][child_id] = multMatx(A, B, n, i, child_id);
            if(i != child_id){
                C[i][n- child_id -1] = multMatx(A, B, n, i, n-child_id-1);
            }
        }

        while(*print_lock != child_id){
            usleep(1000); 
        }
        printf("Result Child [%d]:\n", child_id);
        showChildMatx(C, rows, columns, child_id, n);

        (*print_lock)++;
        shmdt(A);
        shmdt(B);
        shmdt(C);
        shmdt(print_lock);
        
    }

    return EXIT_SUCCESS;
}

int multMatx(int **A, int **B, int size, int dx, int dy){
    int value = 0;
    for(int i=0; i < size; i++){
        value += A[dx][i] * B[i][dy];
    }
    return value;
}

void createSHMs(int *x_shm, int ***X, int rows, int columns){
    size_t size_m = sizeof_dm(rows, columns, sizeof(int));

    *x_shm = shmget(IPC_PRIVATE, size_m, 0666 | IPC_CREAT);
    *X = shmat(*x_shm, 0, 0);
    create_index((void*)*X, rows, columns, sizeof(int));
}

unsigned int sizeof_dm(int rows, int cols, size_t sizeof_element){
    size_t size = rows * sizeof(void*);
    size += (cols * rows * sizeof_element);
    return size;
}

void create_index(void **m, int rows, int cols, size_t sizeof_element){
    int i; 
    size_t sizeRows = cols * sizeof_element;
    m[0] = m + rows;
    for(i = 1; i < rows; i++){
        m[i] = (m[i-1] + sizeRows);
    }
}

void showChildMatx(int **mtx, int rows, int columns, int child_id, int n){
    int is_my_pos;
    for(int i=0; i < rows; i++){
        for(int j=0; j < columns; j++){
            is_my_pos = 0;

            if((i == child_id && j >= child_id && j < n - child_id) ||
                (i == n - child_id - 1 && j >= child_id && j < n - child_id)) {
                is_my_pos = 1;
            }
            else if ((j == child_id && i > child_id && i < n - child_id - 1) ||
                     (j == n - child_id - 1 && i > child_id && i < n - child_id - 1)) {
                is_my_pos = 1;
            }

            is_my_pos ? printf("[%d]\t", mtx[i][j]) : printf(" -- \t");
        }
        printf("\n");
    }
    printf("\n");
}

void showMatx(int **mtx, int rows, int columns){
    for(int i=0; i < rows; i++){
        for(int j=0; j < columns; j++){
            printf("[%d]\t", mtx[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}