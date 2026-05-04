#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const * argv[]){
    int i=0,padre = getpid();

    for(i=0;i<5;i++){
        fork();
    }

    /* 2^N-1 */

     if(getpid()==padre){
        char b[50];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
    }else{
        sleep(1);
    }

return 0;
}