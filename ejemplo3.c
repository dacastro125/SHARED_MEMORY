#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main (){
  int status;
if(fork()==0)
      exit (5);

else {

    wait(&status);
    printf("%d\n",status>>8);
    
}
 return 0;

}
