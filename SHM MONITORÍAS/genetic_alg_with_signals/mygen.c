#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>

int finished = 0;
int n_children;

typedef struct{
    double feature1;
    double feature2;
    double fitness;
} Individual;

void createSHMv(int *x_shm, void **X, int n, size_t sizeof_t);
double getFitness(double f1, double f2);

void handler(int sig);

int main(int argc, char **argv){
    if(argc < 4){
        perror("Send File\n");
        return EXIT_FAILURE;
    }
    int child_id, children[n_children];
    pid_t root = getpid();

    int chunk;

    int n_evolutions;
    int n_individuals;

    Individual *shared_vec;
    int vec_shm_id;

    n_children = atoi(argv[1]);
    n_evolutions = atoi(argv[2]);
    n_individuals = atoi(argv[3]);  

    signal(SIGUSR1, handler); 
    createSHMv(&vec_shm_id, (void**)&shared_vec, n_individuals, sizeof(Individual)); 

    chunk = n_individuals / n_children;

    for(child_id=0; child_id < n_children; child_id++){
        if(!(children[child_id] = fork())){
            break;
        }
    }

    if(root == getpid()){
        for(int i=0; i < n_evolutions; i++){
            
        }
    }else{
        int start, end;

        start = chunk * child_id;
        end = (child_id == n_children-1) ? n_individuals : chunk * (child_id+1);
        while(1){
            pause();

            for(int i=start; i < end; i++){
                shared_vec[i].feature1 = rand()%100;
                shared_vec[i].feature2 = rand()%100;
                shared_vec[i].fitness = getFitness(shared_vec[i].feature1, shared_vec[i].feature2);
            }

            kill(root, SIGUSR1);
        }
    }

    shmdt(shared_vec);
    shmctl(vec_shm_id, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}

double getFitness(double f1, double f2){
    return (f1*f2)/2;
}

void createSHMv(int *x_shm, void **X, int n, size_t sizeof_t){
    size_t size_v = n * sizeof_t;

    *x_shm = shmget(IPC_PRIVATE, size_v, 0666 | IPC_CREAT);
    if(*x_shm == -1){
        perror("Error in shmget"); 
        exit(1);
    }
    *X = shmat(*x_shm, 0, 0);
    if(*X == NULL){
        perror("Error in shmat"); 
        exit(1);
    }
}

void handler(int sig){
    finished++;
}