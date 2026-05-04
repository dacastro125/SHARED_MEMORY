#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>

typedef enum {
    NEUTRAL,
    EXPOSED,
    VERIFIED
} Represent;

unsigned int sizeof_dm(int rows, int cols, size_t sizeof_element);
void create_index(void **m, int rows, int cols, size_t sizeof_element);

void createSHMs(int *x_shm, int ***X, int rows, int columns);
void copyMatx(int **a, int **b, int rows, int cols);
int isExposed(int **mtx, int rows, int col, int dx, int dy); 
int isVerified(int **mtx, int rows, int col, int dx, int dy);

void sighandler(int sig);

void readFile(const char *file, int ***mtx, int *rows, int *columns, int *children, int *minutes);
void showMatx(int **mtx, int rows, int columns);
void Free(int ***mtx, int rows);

int main(int argc, char **argv){
    if(argc < 2){
        perror("Send file\n");
        return EXIT_FAILURE;
    }
    int **file_matx, rows, columns;
    int n_children, child_id;
    int minutes, round=0;

    int **st_current, **st_next;
    int mA_shm, mB_shm; 

    int pids_shm;
    pid_t *pids;
    pid_t root = getpid();

    readFile(argv[1], &file_matx, &rows, &columns, &n_children, &minutes);
    showMatx(file_matx, rows, columns);
    if(n_children != 2){
        printf("Error: number of children must be 2\n");
        return EXIT_FAILURE;
    }

    pids_shm = shmget(IPC_PRIVATE, n_children * sizeof(pid_t), 0666 | IPC_CREAT);
    if (pids_shm == -1){ perror("shmget pids"); exit(1); }
    pids = shmat(pids_shm, 0, 0);
    if (pids == (void*)-1){ perror("shmat pids"); exit(1); }

    signal(SIGUSR1, sighandler);

    createSHMs(&mA_shm, &st_current, rows, columns);
    createSHMs(&mB_shm, &st_next, rows, columns);

    copyMatx(st_current, file_matx, rows, columns);
    Free(&file_matx, rows);

    for (child_id = 0; child_id < n_children; child_id++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); exit(1); }
        if(pid == 0) break;

        pids[child_id] = pid;
        //printf("pid: %d\n", pids[child_id]);
    }

    if(getpid() == root){
        usleep(10000); 
        for (round = 0; round < minutes; ++round){
            copyMatx(st_next, st_current, rows, columns);

            //printf("parent %d sending signal to: %d\n", getpid() ,pids[0]);
            kill(pids[0], SIGUSR1);
            pause(); 

            printf("Round Number: %d\n", round+1);
            showMatx(st_next, rows, columns);

            copyMatx(st_current, st_next, rows, columns);
        }

        kill(pids[0], SIGTERM);
        kill(pids[1], SIGTERM);
        for(int i=0; i < n_children; i++) wait(NULL);

        shmdt(st_current);
        shmctl(mA_shm, IPC_RMID, NULL);
        shmdt(st_next);
        shmctl(mB_shm, IPC_RMID, NULL);

        shmdt(pids);
        shmctl(pids_shm, IPC_RMID, NULL);
    }else{
        while (1) {
            pause(); 

            if(child_id == 0){ // neutral -> exposed
                for(int i=0; i < rows; i++){
                    for(int j=0; j < columns; j++){
                        if(st_current[i][j] == NEUTRAL && isExposed(st_current, rows, columns, i, j)){
                            st_next[i][j] = EXPOSED; 
                        }
                    }
                }
                //printf("Child %d sending signal to: %d\n", getpid() ,pids[1]);
                kill(pids[child_id+1], SIGUSR1);     
            }else{ // exposed -> verified
                for(int i=0; i < rows; i++){
                    for(int j=0; j < columns; j++){
                        if(st_current[i][j] == EXPOSED && isVerified(st_current, rows, columns, i, j)){
                            st_next[i][j] = VERIFIED;
                        }
                    }
                }
                //printf("Child %d sending signal to: %d\n", getpid(), root);
                kill(root, SIGUSR1);
            }
        }
        shmdt(st_current);
        shmdt(st_next);
        shmdt(pids);
    }
    
    return EXIT_SUCCESS;
}

void sighandler(int sig){
    //printf("[%d] Sig %d captured\n", getpid(), sig);
}

int isVerified(int **mtx, int rows, int col, int dx, int dy){
    int count=0;
    float r = rand()/(float)RAND_MAX, p;
    
    for(int i=dx-1; i <= dx+1; i++){
        for(int j=dy-1; j <= dy+1; j++){
            if(i == dx && j == dy) continue;
            if(i < 0 || i >= rows || j < 0 || j >= col) continue;

            if(mtx[i][j] == VERIFIED){
                count++;
            }
        }
    }
    p = 0.15 + (count*0.05);
    return r < p ? 1 : 0;
}

int isExposed(int **mtx, int rows, int col, int dx, int dy){
    int count=0;
    for(int i=dx-1; i <= dx+1; i++){
        for(int j=dy-1; j <= dy+1; j++){
            if(i < 0 || i >= rows || j < 0 || j >= col) continue;

            if(mtx[i][j] == EXPOSED || mtx[i][j] == VERIFIED){
                count++;
            }
        }
    }

    return count >= 2 ? 1 : 0;
}

void createSHMs(int *x_shm, int ***X, int rows, int columns){
    size_t size_m = sizeof_dm(rows, columns, sizeof(int));

    *x_shm = shmget(IPC_PRIVATE, size_m, 0666 | IPC_CREAT);
    if(*x_shm == -1){
        perror("Error in shmget"); exit(1);
    }
    *X = shmat(*x_shm, 0, 0);
    if(*X == NULL){
        perror("Error in shmat"); exit(1);
    }
    create_index((void*)*X, rows, columns, sizeof(int));
}

void copyMatx(int **a, int **b, int rows, int cols){
    for(int i=0; i < rows; i++){
        for(int j=0; j < cols; j++){
            a[i][j]=b[i][j];
        }
    }
}

void readFile(const char *file, int ***mtx, int *rows, int *columns, int *children, int *minutes){
    FILE *fl = fopen(file, "r");
    if(!fl){perror("Fail fopen\n"); exit(1);}

    fscanf(fl, "%d", minutes);
    fscanf(fl, "%d", children);
    fscanf(fl, "%d", rows);
    fscanf(fl, "%d", columns);
    printf("min: %d, children: %d, rows %d, col: %d\n", *minutes, *children, *rows, *columns);

    *mtx = (int**) malloc(*rows * sizeof(int*));
    if(!(*mtx)){
        perror("fail malloc\n");
        exit(1);
    }

    for(int i=0; i < *rows; i++){
        (*mtx)[i] = (int *) calloc(*columns, sizeof(int));
        if(!(*mtx)[i]){
            perror("fail malloc\n");
            exit(1);
        }
    }

    for(int i=0; i < *rows; i++){
        for(int j=0; j < *columns; j++){
            fscanf(fl, "%d", &(*mtx)[i][j]);
        }
    }

    fclose(fl);
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

void Free(int ***mtx, int rows){
    for(int i=0; i < rows; i++){
        free((*mtx)[i]);
    }
    free((*mtx));
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