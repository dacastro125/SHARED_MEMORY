#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_LEN 256

void showtree();
void readFile(const char *file, char ***vec, int *size);
void createSHMv(int *x_shm, void **X, size_t size);

int main(int argc, char **argv){
    if(argc < 4){
        fprintf(stderr, "Usage: %s <file> <n_children> <word>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n_children = atoi(argv[2]);
    const char *word = argv[3];
    pid_t root = getpid();
    int child_id, delta;

    printf("Word to search: %s\n", word);

    // Leer archivo
    char **file_vec;
    int size;
    readFile(argv[1], &file_vec, &size);
    delta = size / n_children;

    size_t total_chars = 0;
    for(int i=0; i < size; i++) total_chars += strlen(file_vec[i]) + 1;

    int text_shm_id, offsets_shm_id, results_shm_id;
    char *shared_text;
    int *offsets, *shared_reslt;

    createSHMv(&text_shm_id, (void**)&shared_text, total_chars);
    createSHMv(&offsets_shm_id, (void**)&offsets, size * sizeof(int));
    createSHMv(&results_shm_id, (void**)&shared_reslt, n_children * sizeof(int));

    int pos = 0;
    for(int i = 0; i < size; i++){
        offsets[i] = pos;
        strcpy(shared_text + pos, file_vec[i]);
        pos += strlen(file_vec[i]) + 1;
    }

    for(int i = 0; i < size; i++)
        free(file_vec[i]);
    free(file_vec);

    for(child_id = 0; child_id < n_children; child_id++){
        if(fork() == 0) break;
    }

    if(root == getpid()){
        showtree();
        for(int i=0; i<n_children; i++) wait(NULL);

        printf("\nResults per child:\n");
        int total = 0;
        for(int i=0; i<n_children; i++){
            printf("Child %d: %d\n", i, shared_reslt[i]);
            total += shared_reslt[i];
        }
        printf("Total occurrences of \"%s\": %d\n", word, total);

        shmdt(shared_text);
        shmdt(offsets);
        shmdt(shared_reslt);
        shmctl(text_shm_id, IPC_RMID, NULL);
        shmctl(offsets_shm_id, IPC_RMID, NULL);
        shmctl(results_shm_id, IPC_RMID, NULL);
    }else{
        int start = child_id * delta;
        int end = (child_id == n_children - 1) ? size : (child_id + 1) * delta;

        int sum = 0;
        char buff[MAX_LEN], *tok;

        for(int i=start; i < end; i++){
            strncpy(buff, shared_text + offsets[i], MAX_LEN);
            buff[MAX_LEN - 1] = '\0';

            tok = strtok(buff, " ");
            while(tok){
                if(strcmp(tok, word) == 0) sum++;
                tok = strtok(NULL, " ");
            }
        }

        shared_reslt[child_id] = sum;

        shmdt(shared_text);
        shmdt(offsets);
        shmdt(shared_reslt);
        exit(0);
    }

    return EXIT_SUCCESS;
}

void createSHMv(int *x_shm, void **X, size_t size){
    *x_shm = shmget(IPC_PRIVATE, size, 0666 | IPC_CREAT);
    if(*x_shm == -1){ perror("shmget"); exit(1); }
    *X = shmat(*x_shm, 0, 0);
    if(*X == (void*)-1){ perror("shmat"); exit(1); }
}

void readFile(const char *file, char ***vec, int *size){
    FILE *fl = fopen(file, "r");
    if(!fl){ perror("readFile"); exit(1); }

    char buff[MAX_LEN];
    *size = 0;
    while(fgets(buff, sizeof(buff), fl)) (*size)++;
    rewind(fl);

    *vec = malloc(*size * sizeof(char*));
    for(int i=0; i < *size; i++){
        fgets(buff, sizeof(buff), fl);
        buff[strcspn(buff, "\n")] = '\0';
        (*vec)[i] = malloc(strlen(buff) + 1);
        strcpy((*vec)[i], buff);
    }
    fclose(fl);
}

void showtree(){
    char cmd[64];
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);
}
