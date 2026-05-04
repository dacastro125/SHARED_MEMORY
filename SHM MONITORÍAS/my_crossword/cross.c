#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LEN 256

typedef struct {
    char word[MAX_LEN];
    int row;
    int column;
    char direction[10];
} Result;

typedef struct {
    Result results[100];
    int count;
    int in_use;
}Shared_Data;

unsigned int sizeof_dm(int rows, int cols, size_t sizeof_element);
void create_index(void **m, int rows, int cols, size_t sizeof_element);

void readFile(const char *file, char ***mtx , int *rows, int *columns, char ***words, int *size);
void createSHMs(int *x_shm, void **X, int rows, int columns, size_t sizeof_t);
void copyMatx(char **a, char **b, int rows, int cols);

int main(int argc, char **argv){
    if(argc < 2){
        perror("Send argument\n");
        return EXIT_FAILURE;
    }
    int n_children = 4, child_id;
    pid_t root = getpid();

    char **file_mtx, **words;
    int rows, columns, size;

    char **shared_mtx;
    int mtx_shm_id;

    Shared_Data *data;
    int data_shm;

    readFile(argv[1], &file_mtx, &rows, &columns, &words, &size);

    // parameters: id, (cast to void to use a generic)matrix, r, c, type of element in bytes
    createSHMs(&mtx_shm_id, (void**)&shared_mtx, rows, columns, sizeof(int));

    data_shm = shmget(IPC_PRIVATE, sizeof(Shared_Data), 0666 | IPC_CREAT);
    if (data_shm == -1){ perror("shmget pids"); exit(1); }
    data = (Shared_Data*) shmat(data_shm, 0, 0);
    if (data == (void*)-1){ perror("shmat pids"); exit(1); }

    data->count = 0;
    data->in_use = 0; //free to use

    copyMatx(shared_mtx, file_mtx, rows, columns);

    for(child_id=0; child_id < n_children; child_id++){
        if(!fork()) break;
    }

    if(root == getpid()){

        for(int i=0; i < n_children; i++) wait(NULL);

        for(int i=0; i < data->count; i++){
            printf("Word '%s' found. start in [%d][%d] in the %s.\n", data->results[i].word, data->results[i].row, data->results[i].column, data->results[i].direction);
        }

        shmdt(shared_mtx);
        shmctl(mtx_shm_id, IPC_RMID, NULL);
        shmdt(data);
        shmctl(data_shm, IPC_RMID, NULL);
    }else{
        int word_len, found=0;
        char *word;

        switch (child_id){
            case 0:{
                for(int w=0; w < size; w++){
                    word = words[w];
                    word_len = strlen(word);
                    found = 1;
                    
                    //printf("word: %s\n",  word);
                    for(int i=0; i < rows; i++){
                        for(int j=0; j < columns; j++){
                            if(shared_mtx[i][j] == word[0]){
                                found = 1;
                                if(j + word_len - 1 >= columns){
                                    continue;
                                }

                                for(int k=0; k < word_len; k++){
                                    //printf("Debug: %c\n",  shared_mtx[i][j + k]);
                                    if(j + k >= columns || shared_mtx[i][j + k] != word[k]){
                                        found=0;
                                        break;
                                    }
                                }

                                if(found){
                                    //printf("Word '%s' found. start in [%d][%d].\n", word, i, j);
                                    while(data->in_use){
                                        usleep(1000);
                                    }
                                    data->in_use = 1;
                                    Result *res = &data->results[data->count++];
                                    strcpy(res->word, word);
                                    res->row = i;
                                    res->column = j;
                                    strcpy(res->direction, "right");
                                    data->in_use = 0;
                                    break;
                                }

                            }
                        }
                    }
                }
                break;
            }
            case 1:{
                for(int w=0; w < size; w++){
                    word = words[w];
                    word_len = strlen(word);
                    found = 1;
                    //printf("word: %s\n",  word);
                    
                    for(int i=0; i < rows; i++){
                        for(int j=columns-1; j >= 0; j--){
                            if(shared_mtx[i][j] == word[0]){
                                found = 1;
                                if(j - word_len + 1 < 0){
                                    continue;
                                }

                                for(int k=0; k < word_len; k++){
                                    //printf("Debug: %c\n",  shared_mtx[i][j - k]);
                                    if(j - k < 0 || shared_mtx[i][j - k] != word[k]){
                                        found=0;
                                        break;
                                    }
                                }

                                if(found){
                                    //printf("Word '%s' found. start in [%d][%d].\n", word, i, j);
                                    while(data->in_use){
                                        usleep(1000);
                                    }
                                    data->in_use = 1;
                                    Result *res = &data->results[data->count++];
                                    strcpy(res->word, word);
                                    res->row = i;
                                    res->column = j;
                                    strcpy(res->direction, "left");
                                    data->in_use = 0;
                                    break;
                                }

                            }
                        }
                    }
                }
                break;
            }
            case 2:{
                for(int w=0; w < size; w++){
                    word = words[w];
                    word_len = strlen(word);
                    found = 1;
                    //printf("word: %s\n",  word);
                    
                    for(int i=0; i < rows; i++){
                        for(int j=0; j < columns; j++){
                            if(shared_mtx[i][j] == word[0]){
                                found = 1;
                                if(i + word_len - 1 >= rows){
                                    continue;
                                }

                                for(int k=0; k < word_len; k++){
                                    //printf("Debug: %c\n",  shared_mtx[i+k][j]);
                                    if(i + k >= rows|| shared_mtx[i+k][j] != word[k]){
                                        found=0;
                                        break;
                                    }
                                }

                                if(found){
                                    //printf("Word '%s' found. start in [%d][%d].\n", word, i, j);
                                    while(data->in_use){
                                        usleep(1000);
                                    }
                                    data->in_use = 1;
                                    Result *res = &data->results[data->count++];
                                    strcpy(res->word, word);
                                    res->row = i;
                                    res->column = j;
                                    strcpy(res->direction, "down");
                                    data->in_use = 0;
                                    break;
                                }

                            }
                        }
                    }
                }
                break;
            }
            case 3:{
                for(int w=0; w < size; w++){
                    word = words[w];
                    word_len = strlen(word);
                    found = 1;
                    //printf("word: %s\n",  word);
                    
                    for(int i=rows-1; i >= 0; i--){
                        for(int j=0; j < columns; j++){
                            if(shared_mtx[i][j] == word[0]){
                                found = 1;
                                if(i - word_len + 1 < 0){
                                    continue;
                                }

                                for(int k=0; k < word_len; k++){
                                    //printf("Debug: %c\n",  shared_mtx[i-k][j]);
                                    if(i - k < 0 || shared_mtx[i - k][j] != word[k]){
                                        found=0;
                                        break;
                                    }
                                }

                                if(found){
                                    //printf("Word '%s' found. start in [%d][%d].\n", word, i, j);
                                    while(data->in_use){
                                        usleep(1000);
                                    }
                                    data->in_use = 1;
                                    Result *res = &data->results[data->count++];
                                    strcpy(res->word, word);
                                    res->row = i;
                                    res->column = j;
                                    strcpy(res->direction, "up");
                                    data->in_use = 0;
                                    break;
                                }

                            }
                        }
                    }
                }
                break;
            }
        }
        shmdt(shared_mtx);
        shmdt(data);
    }    

    for(int i=0; i < size; i++){
        free(words[i]);
    }
    free(words);
    words = NULL;
    return EXIT_SUCCESS;
}

void copyMatx(char **a, char **b, int rows, int cols){
    for(int i=0; i < rows; i++){
        for(int j=0; j < cols; j++){
            a[i][j]=b[i][j];
        }
    }
    for(int i=0; i < rows; i++){
      free(b[i]);
      b[i] = NULL;
    }
    free(b);
}

void createSHMs(int *x_shm, void **X, int rows, int columns, size_t sizeof_t){
    size_t size_m = sizeof_dm(rows, columns, sizeof_t);

    *x_shm = shmget(IPC_PRIVATE, size_m, 0666 | IPC_CREAT);
    if(*x_shm == -1){
        perror("Error in shmget"); exit(1);
    }
    *X = shmat(*x_shm, 0, 0);
    if(*X == NULL){
        perror("Error in shmat"); exit(1);
    }
    create_index((void*)*X, rows, columns, sizeof_t);
}

void readFile(const char *file, char ***mtx , int *rows, int *columns, char ***words, int *size){
    char buff[MAX_LEN];
    FILE *fl = fopen(file, "r");
    if(!fl){perror("Fail reading file\n"); exit(1);}

    fscanf(fl, "%d", rows);
    fscanf(fl, "%d", columns);
    printf("Rows: %d, Columns: %d.\n", *rows, *columns);

    *mtx = (char **) malloc(*rows * sizeof(char *));
    if(!(*mtx)){perror("Fail malloc in rows file\n"); exit(1);}

    for(int i = 0; i < *rows; i++){
        (*mtx)[i] = (char *) malloc(*columns * sizeof(char));
        if(!(*mtx)[i]){
            perror("Fail malloc in columns file\n");
            exit(1);
        }
    }
    
    for(int i=0; i < *rows; i++){
        for(int j=0; j < *columns; j++){
            fscanf(fl, " %c", &(*mtx)[i][j]);
        }
    }

    for(int i=0; i < *rows; i++){
        for(int j=0; j < *columns; j++){
            printf("%c ", (*mtx)[i][j]);
        }
        printf("\n");
    }

    fscanf(fl, "%d", size);
    fgetc(fl);
    printf("Words to search: %d\n", *size);

    *words = (char **) malloc(*size * sizeof(char *));
    if(!(*mtx)){perror("Fail malloc in rows file\n"); exit(1);}

    for(int i=0; i < *size; i++){
        fgets(buff, sizeof(buff), fl);
        buff[strcspn(buff, "\n")] = '\0'; //replace the \n by end of line \0
        //printf("buff: %s, strlen: %ld, size: %ld\n", buff, strlen(buff), sizeof(buff));
        (*words)[i] = (char*) malloc((strlen(buff)+1) * sizeof(char));
        strcpy((*words)[i], buff);
    }

    /*for (int i = 0; i < *size; i++) {
        printf("%s ---- and len %ld\n", (*words)[i], strlen((*words)[i]));
    }*/

    fclose(fl);
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
