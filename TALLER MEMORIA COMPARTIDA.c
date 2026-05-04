 /*
    JUAN DAVID RODRIGUEZ CERVANTES
    2013114113
 */
 #include<stdio.h>
 #include<unistd.h>
 #include<string.h>
 #include<stdlib.h>
 #include<wait.h>
 #include<sys/types.h>
 #include<sys/shm.h>
 #include<sys/stat.h>
 #include<signal.h>

 
 pid_t pid;
 int shmId;
 int m,n;
 int Rows=3;
 int Cols=3;
 double **matrix;
 
 unsigned int sizeof_dm(int rows, int col, size_t sizeElement){
 size_t size = rows *sizeof(void *); 
 size += (col*rows*sizeElement); 
 return size;
 }

 void create_index(void **m, int rows, int cols, size_t sizeElement){
     int i;
     size_t sizerow = cols*sizeElement;
     m[0] = m+rows;
     
     for(i=1; i<cols; i++){
         m[i]= (m[i-1]+sizerow);
     }
 }

 int main(){

 size_t sizeMatrix = sizeof_dm(Rows,Cols,sizeof(double));
 shmId = shmget(IPC_PRIVATE,sizeMatrix,IPC_CREAT|0600);
 matrix=shmat(shmId, NULL, 0);// enlace
 create_index((void*)matrix,Rows,Cols,sizeof(double));

     
 for(int m=0; m<Cols; m++){
     for(int n=0; n<Rows; n++){
         matrix[m][n]=(double)n/(double)(n+1);
     }
 }
     
    pid = fork();
    if (pid != 0){  
         for(int m=0; m<Cols; m++){
             for(int n=0; n<Rows; n++){
              printf("HM[%.6lf]",matrix[m][n]);
             }
             printf("\n");
           }
        for(int m=0; m<Cols; m++){
            for(int n=0; n<Rows; n++){
                if(m==1&&n==1){
                   matrix[m][n]=600; 
                }
            }
                   
        }
                     shmdt(matrix);
    }else{
        wait(NULL);
        for(int m=0; m<Cols; m++){
             for(int n=0; n<Rows; n++){
             printf("PM[%.6lf]",matrix[m][n]);
             }
             printf("\n");
         }
                         
         shmdt(matrix);
         shmctl(shmId,IPC_RMID,0);
         }
         return 0;
 }








