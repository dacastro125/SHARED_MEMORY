#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int c,p;
void handler();
void main(){
int k, s=0,N=0;
signal(SIGUSR1, handler);
p = getpid();
scanf("%d",&N);
int chld[N];
for(c=0; c<N; c++){
    if (!(chld[c] = fork())) 
       break;
} 
if(c < N){
  for( k=0; k<N; k++){
      pause ();
       s = chld[c‐1];
       if(s<0)
	s = p;
       	kill(s,SIGUSR1);
     }
 }
else{
  for( k=0; k<N; k++){
       kill( chld[N‐1],SIGUSR1);
       pause();
     }
 }
}
void handler()
{
  if(getpid() == p)
       printf("padre\n");
else
     printf("hijo%d\n", c);
};
