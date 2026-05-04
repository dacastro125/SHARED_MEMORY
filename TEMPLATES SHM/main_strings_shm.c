#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LEN 256

void showtree();

void readFile(const char *file, char ***vec, int *size);
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

    int text_shm_id, index_shm_id;
    char *shared_text;
    int *offsets;
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
        
        shmdt(shared_text);
        shmdt(offsets);
        shmctl(text_shm_id, IPC_RMID, NULL);
        shmctl(index_shm_id, IPC_RMID, NULL);
    }else{
        
        /*printf("Child (%d) reading from shared memory\n", getpid());
        for(int i = 0; i < n; i++){
            printf("%d: %s\n", i, shared_text + offsets[i]);
        }*/

        /*
        Lines read: 2
        word1\0word2\0
        */
        
        for(int i = 0; i < total_chars; i++){
            if (shared_text[i] == '\0'){
                printf("\\0");
            }else{
                printf("%c", shared_text[i]);
            }                
        }
        printf("\n");

        shmdt(shared_text);
        shmdt(offsets);
    }   

    return EXIT_SUCCESS;
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
    FILE *fl = fopen(file, "r");
    if(!fl){perror("Fail reading file\n"); exit(1);}

    *size = 0;
    char buff[MAX_LEN];
    while(fgets(buff, sizeof(buff), fl)) (*size)++;
    rewind(fl);

    *vec = (char**) malloc(*size * sizeof(char*));
    for(int i = 0; i < *size; i++){
        fgets(buff, sizeof(buff), fl);
        buff[strcspn(buff, "\n")] = '\0';
        (*vec)[i] = malloc(strlen(buff) + 1);
        strcpy((*vec)[i], buff);
    }

    fclose(fl);
}

void showtree(){
    char cmd[20] = {""};
    sprintf(cmd, "pstree -cAlp %d", getpid());
    system(cmd);	
}