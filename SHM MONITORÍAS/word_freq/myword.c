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

void readFile(const char *file, char ***vec, int *size);
void createSHMv(int *x_shm, void **X, int n, size_t sizeof_t);
void copyVec(char **a, char **b, int n);

int main(int argc, char **argv){
    if(argc < 2){
        perror("Send argument\n");
        return EXIT_FAILURE;
    }
    int n_children = atoi(argv[2]), child_id, delta;
    pid_t root = getpid();    
    char **file_vec;
    int size;

    char **shared_vec;
    int vec_shm_id;

    readFile(argv[1], &file_vec, &size);

    createSHMv(&vec_shm_id, (void**)&shared_vec, size, sizeof(char*)); 
    copyVec(shared_vec, file_vec, size);
    for(int i=0; i < size; i++){
        free(file_vec[i]);
    }
    free(file_vec);
    file_vec = NULL;

    delta = size / n_children;

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(root == getpid()){


        for(int i=0; i < n_children; i++) wait(NULL);

        for (int i = 0; i < size; i++) {
            free(shared_vec[i]);
        }
        shmdt(shared_vec);
        shmctl(vec_shm_id, IPC_RMID, NULL);
    }else{
        int start, end, len, match=0;
        char buff[MAX_LEN];
        char *tok, *tok2, line[MAX_LEN], inner[MAX_LEN], *ptr1, *ptr2;

        start = child_id * delta;
        end = child_id == n_children-1 ? size : (child_id+1) * delta;

        printf("Child: %d, is reading: \n", child_id);
        for(int i=start; i < end; i++){
            printf("i: %d. %s\n", i, shared_vec[i]);
            len = strlen(shared_vec[i]);
            strncpy(line, shared_vec[i], len);
            line[len] = '\0';

            tok = __strtok_r(line, " ", &ptr1);
            while(tok != NULL){
                //printf("tok: %s\n", tok);
                for(int j = start; j < end; j++){
                    len = strlen(shared_vec[j]);
                    strncpy(inner, shared_vec[j], len);
                    inner[len] = '\0';
                    tok2 = __strtok_r(inner, " ", &ptr2);

                    while(tok2 != NULL) {
                        if (strcmp(tok, tok2) == 0) {
                            //printf("Match found: '%s' in line %d and line %d\n", tok, i, j);
                            match++;
                        }
                        tok2 = __strtok_r(NULL, " ", &ptr2);
                    }
                }
                tok = __strtok_r(NULL, " ", &ptr1);
            }
        }
        printf("Child: %d, found: %d match\n", child_id, match);
        printf("\n");

        shmdt(shared_vec);
    }
    
    return EXIT_SUCCESS;
}

void createSHMv(int *x_shm, void **X, int n, size_t sizeof_t){
    size_t size_v = n * sizeof_t;

    *x_shm = shmget(IPC_PRIVATE, size_v, 0666 | IPC_CREAT);
    if(*x_shm == -1){
        perror("Error in shmget"); 
        exit(1);
    }
    *X = (char**) shmat(*x_shm, 0, 0);
    if(*X == (void*) -1){
        perror("Error in shmat"); 
        exit(1);
    }
}

void copyVec(char **a, char **b, int n) {
    for (int i = 0; i < n; i++) {
        a[i] = malloc(strlen(b[i]) + 1); 
        strcpy(a[i], b[i]);
    }
}

void readFile(const char *file, char ***vec, int *size){
    int only_spaces = 1, cleanCount = 0;;
    char buff[MAX_LEN];
    FILE *fl = fopen(file, "r");
    if(!fl){perror("Fail reading file\n"); exit(1);}

    *size = 0;
    while(fgets(buff, sizeof(buff), fl)) (*size)++;
    rewind(fl);

    *vec = (char**) malloc(*size * sizeof(char*));
    if(!*vec){perror("Fail malloc rw\n");exit(1);}

    for(int i=0; i < *size; i++){
        fgets(buff, sizeof(buff), fl);

        buff[strcspn(buff, "\n")] = '\0'; //replace the \n by end of line \0
        //printf("buff: %s, strlen: %ld, size: %ld\n", buff, strlen(buff), sizeof(buff));
        (*vec)[i] = (char*) malloc((strlen(buff)+1) * sizeof(char));
        strcpy((*vec)[i], buff);
    }
    
    for(int i = 0; i < *size; i++){
        only_spaces = 1;
        for(char *p = (*vec)[i]; *p != '\0'; p++){
            if(!isspace(*p)){
                only_spaces = 0;
                break;
            }
        }
        if(!only_spaces){
            (*vec)[cleanCount++] = (*vec)[i];
        }else{
            free((*vec)[i]);
        }
    }

    *size = cleanCount;

    /*for(int i = 0; i < *size; i++){
        printf("%s ---- and len %ld\n", (*vec)[i], strlen((*vec)[i]));
    }*/

    fclose(fl);
}