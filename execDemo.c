#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

int globalNum = 42;

int main(int argc, char *argv[]){


printf("	execDemo: My pid is %d. \n", getpid());
int globalNum = 152;

//With L: coma separated arguments
//with V: Vector (i.e..., an array of strings)
//With P: Include normal search path for executable.

char *args[]={"./helloExec","Hello", "World",NULL};
execvp(args[0], args);	

printf("Farewell cruel world\n");

return 0;
}
