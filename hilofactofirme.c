#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double mult(int i, int j) {
  int k;
  double p= 1.;
  for (k=i; k<=j; k++)
    p *= k;

  return p;
}

typedef struct {
  int i, j;
  pthread_t pid;
  double res;
} Args;

void *mult_thread(void *ptr) {
  Args *args= (Args*)ptr;
  args->res= mult(args->i, args->j);
  return NULL;
}

int main(int argc, char **argv) {
  int n= atoi(argv[1]);
  int l= (n+1)/2;
  Args args1, args2;

  args1.i= 1;
  args1.j= l;
  pthread_create(&args1.pid, NULL, mult_thread, &args1);

  args2.i= l+1;
  args2.j= n;
  pthread_create(&args2.pid, NULL, mult_thread, &args2);

  pthread_join(args1.pid, NULL);
  pthread_join(args2.pid, NULL);
  printf("factorial=%1.14g\n", args1.res*args2.res);
  return 0;
}
