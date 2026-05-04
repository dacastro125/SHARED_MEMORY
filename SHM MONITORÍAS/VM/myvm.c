#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_LEN 256
#define MAX_REG 64
#define MAX_VALUES 256

typedef struct {
    char region[MAX_REG];
    int minutes;
} Result;

typedef struct {
    Result results[MAX_VALUES];
    int count;
    int is_busy;
} SharedResults;


void showtree();

void readFile(const char *file, char ***vec, int *size);
int digit(const char *s);
void createSHMv(int *x_shm, void **X, size_t size);

int main(int argc, char **argv){
    if(argc < 3){
        perror("Send file and childs\n");
        return EXIT_FAILURE;
    }

    char **lines;
    int n;
    int pos = 0;

    int n_children = atoi(argv[2]), child_id;
    pid_t root = getpid();

    int text_shm_id, index_shm_id, results_shm_id;
    char *shared_text;
    int *offsets;
    SharedResults *shared;
    results_shm_id = shmget(IPC_PRIVATE, sizeof(SharedResults), IPC_CREAT | 0666);
    shared = (SharedResults *) shmat(results_shm_id, NULL, 0);
    shared->count = 0;

    readFile(argv[1], &lines, &n);
    printf("Lines read: %d\n", n);

    size_t total_chars = 0;
    for(int i = 0; i < n; i++){
        total_chars += strlen(lines[i]) + 1;
    }

    createSHMv(&text_shm_id, (void**)&shared_text, total_chars * sizeof(char));
    createSHMv(&index_shm_id, (void**)&offsets, n * sizeof(int));
    
    for(int i = 0; i < n; i++){
        offsets[i] = pos;
        strcpy(shared_text + pos, lines[i]);
        pos += strlen(lines[i]) + 1;
    }

    for(int i = 0; i < n; i++) free(lines[i]);
    free(lines);

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(root == getpid()){
        showtree();

        for(int i=0; i < n_children; i++) wait(NULL);

        int max = 0;
        char max_region[32];

        //printf("%d processed and %d childs\n", shared->count, n_children);
        if(shared->count != n_children){
            perror("CONCURRENCE");
            exit(-1);
        } 

        for(int i = 0; i < shared->count; i++){
            if (shared->results[i].minutes > max) {
                max = shared->results[i].minutes;
                strcpy(max_region, shared->results[i].region);
            }
        }
        printf("Region with the longest downtime: %s (%d minutes)\n", max_region, max);
        
        shmdt(shared_text);
        shmdt(offsets);
        shmdt(shared);
        shmctl(text_shm_id, IPC_RMID, NULL);
        shmctl(index_shm_id, IPC_RMID, NULL);
        shmctl(results_shm_id, IPC_RMID, NULL);
    }else{
        
        /*printf("Child (%d) reading from shared memory\n", getpid());
        for(int i = 0; i < n; i++){
            printf("%d: %s\n", i, shared_text + offsets[i]);
        }*/

        /*
        Lines read: 2
        word1\0word2\0
        for(int i = 0; i < total_chars; i++){
            if (shared_text[i] == '\0'){
                printf("\\0\n");
            }else{
                printf("%c", shared_text[i]);
            }                
        }
        printf("\n");
        */
        char **vm_value;
        int n_elements;
        char reg[MAX_LEN], buff[MAX_LEN], *tok, *ptr;
        int min, total = 0;

        readFile(shared_text + offsets[child_id], &vm_value, &n_elements);

        printf("Child (%d) reading from shared memory: %s\n", getpid(), shared_text + offsets[child_id]);
        for(int i=0; i < n_elements; i++){
            strcpy(buff, vm_value[i]);
            //printf("Row: %s\n", buff);
            tok = strtok_r(buff, ";", &ptr);
            while(tok != NULL){
                //printf("Tok: %s\n", tok);
                //use...
                if(strcmp(tok, "us-east-1") == 0){
                    strcpy(reg, "us-east-1");
                }else if(strcmp(tok, "eu-west-2") == 0){
                    strcpy(reg, "eu-west-2");
                }else if(strcmp(tok, "sa-east-1") == 0){
                    strcpy(reg, "sa-east-1");
                }else if(digit(tok)){
                    min = atoi(tok);
                }
                tok = strtok_r(NULL, ";", &ptr);
            }
            printf("Region: %s, min: %d\n", reg, min);
            total += min;
        }
        printf("\n");
        printf("Region: %s, Total min: %d\n", reg, total);
        strcpy(shared->results[shared->count].region, reg);
        shared->results[shared->count].minutes = total;
        if(shared->is_busy) usleep(500);
        shared->is_busy = 1;
        shared->count++;
        shared->is_busy = 0;
        
        for(int i = 0; i < n_elements; i++) free(vm_value[i]);
        free(vm_value);
        shmdt(shared_text);
        shmdt(shared);
        shmdt(offsets);
    }   

    return EXIT_SUCCESS;
}

int digit(const char *s){
    if(s == NULL || *s == '\0') return 0;

    for(int i = 0; s[i] != '\0'; i++){
        if (!isdigit((unsigned char)s[i])) return 0; 
    }
    return 1;
}


void createSHMv(int *x_shm, void **X, size_t size){
    *x_shm = shmget(IPC_PRIVATE, size, 0666 | IPC_CREAT);
    if(*x_shm == -1){
        perror("Error in shmget"); 
        exit(1);
    }
    *X = shmat(*x_shm, 0, 0);
    if(*X == (void*)-1){
        perror("Error in shmat"); 
        exit(1);
    }
}

void readFile(const char *file, char ***vec, int *size){
    char buff[MAX_LEN];
    FILE *fl = fopen(file, "r");
    if(!fl){perror("Fail reading file\n"); exit(1);}

    fscanf(fl, "%d", size);
    fgetc(fl);
    //printf("Size: %d\n", *size);

    *vec = (char**) malloc(*size * sizeof(char*));
    if(!*vec){perror("Fail malloc rw\n");exit(1);}

    for(int i=0; i < *size; i++){
        fgets(buff, sizeof(buff), fl);
        buff[strcspn(buff, "\n")] = '\0'; //replace the \n by end of line \0
        //printf("buff: %s, strlen: %ld, size: %ld\n", buff, strlen(buff), sizeof(buff));
        (*vec)[i] = (char*) malloc((strlen(buff)+1) * sizeof(char));
        strcpy((*vec)[i], buff);
    }

    /*for(int i = 0; i < *size; i++){
        printf("%s ---- and len %ld\n", (*vec)[i], strlen((*vec)[i]));
    }*/

    fclose(fl);
}

void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);	
}