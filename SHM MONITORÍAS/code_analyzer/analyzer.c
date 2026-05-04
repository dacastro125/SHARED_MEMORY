#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LINE_LEN 128
#define BUFF_LINES 3

typedef enum {
    CHARGER,
    LINES,
    KEYWORDS,
    COMMENTS
} Turn;

typedef struct {
    char buff[BUFF_LINES][MAX_LINE_LEN];
    int n_lines;
    int eof; //flag of end of file

    int total_code_lines;
    int total_keywords;
    int total_comments;

    int turn; //who's the turn
    int busy; //0 is free, 1 new data
} Shared_Data;

int main(int argc, char **argv){
    if(argc < 2){
        perror("Send File\n");
        return EXIT_FAILURE;
    }
    int n_children = 3, child_id;
    pid_t root = getpid();

    int data_shm;
    Shared_Data *data;

    data_shm = shmget(IPC_PRIVATE, sizeof(Shared_Data), 0666 | IPC_CREAT);
    if (data_shm == -1){ perror("shmget pids"); exit(1); }
    data = (Shared_Data*) shmat(data_shm, 0, 0);
    if (data == (void*)-1){ perror("shmat pids"); exit(1); }

    memset(data, 0, sizeof(Shared_Data));
    data->turn = CHARGER;

    for(child_id=0; child_id < n_children; child_id++){ //child creation
        if(!fork()) break;
    }

    if(root == getpid()){ //parent
        FILE *fl = fopen(argv[1], "r");
        if(!fl){perror("Fail to open the file."); exit(1);}
        char line_buff[MAX_LINE_LEN];
        double pos=0; //file pos

        while(!data->eof){ //end of the file
            while(data->busy);
            while(data->turn != CHARGER){
                if(data->eof && data->n_lines == 0) break;
            }
            data->n_lines = 0;
            if (pos != 0) {
                if(fseek(fl, pos, SEEK_SET)){
                    perror("fail to move");
                    exit(1);
                }
            }

            while(fgets(line_buff, MAX_LINE_LEN, fl)){
                strncpy(data->buff[data->n_lines], line_buff, MAX_LINE_LEN);
                printf("Parent is Sending: %s", line_buff);
                data->n_lines++;

                pos = ftell(fl);  
            
                if(data->n_lines == BUFF_LINES || feof(fl)) break;
            }
            printf("\n");
            //printf("\npointer in the file: %ld\n", (long)pos);

            if(feof(fl)) data->eof = 1; //is the end of file
            //printf("Is the end? %d and N_lines is: %d\n\n", data->eof, data->n_lines);

            data->busy = 1;
            data->turn = LINES;
        }

        fclose(fl);

        for(int i=0; i < n_children; i++) wait(NULL);

        printf("\nThe total of code lines is: %d\n", data->total_code_lines);
        printf("The total of keywords is: %d\n", data->total_keywords);
        printf("The total of comments is: %d\n", data->total_comments);        

        shmdt(data);
        shmctl(data_shm, IPC_RMID, NULL);
    }else{ //childs
        switch (child_id){
            case 0:{
                while(1){
                    while (data->turn != LINES); //wait turn

                    for (int i = 0; i < data->n_lines; i++) {
                        printf("Reading: %s", data->buff[i]);
                    }

                    printf("End of the read\n");
                    data->total_code_lines += data->n_lines;
                    data->turn = KEYWORDS;
                    if (data->eof) break;
                }
                break;
            }
            case 1:{
                const char *keywords[] = {"if", "while", "for", "return"};
                int size = sizeof(keywords) / sizeof(keywords[0]);
                while(1){
                    while (data->turn != KEYWORDS); //wait turn

                    for(int i=0; i < data->n_lines; i++){
                        for(int j=0; j < size; j++){
                            if(strstr(data->buff[i], keywords[j])){
                                data->total_keywords++;
                            }
                        }
                    }
                    printf("End of Keywords\n");
                    data->turn = COMMENTS;
                    if(data->eof) break;
                }

                break;
            }
            case 2:{
                while(1){
                    while (data->turn != COMMENTS); //wait turn

                    for(int i=0; i < data->n_lines; i++){
                        if(strstr(data->buff[i], "//")){
                            data->total_comments++;
                        }
                    }
                    printf("End of Comments, end of this 3 lines.\n\n");
                    data->busy = 0;
                    data->turn = CHARGER;
                    if(data->eof) break;
                }
                break;
            }
        }
        shmdt(data);
    }

    return EXIT_SUCCESS;
}