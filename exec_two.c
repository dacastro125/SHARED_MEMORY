#include <unistd.h>
#include <stdio.h>
int main(int argc, char *argv[]) 
{
  printf("> Before the call of the program '%s' with 'execlp'\n", argv[0]);
  execlp(argv[0],argv[0],NULL);
  printf("> Couldn't execute '%s'\n", argv[0]);
  return 0;
}
