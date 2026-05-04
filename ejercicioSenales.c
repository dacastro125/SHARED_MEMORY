#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void manejador(int numsig){


}

int main()
{

    int i=1,n=0;
    int h[5];

     printf("Digite el numero n=");
        scanf("%d",&n);
    signal(SIGUSR1,manejador);

        if ( (h[0]=fork() )==0){
            //hijo 1
            while(n>=i){
            pause();
            printf("hijo1 %d\n",getpid());
            kill(getppid(),SIGUSR1);
             i++;
            }

        }else if (( h[1]=fork() )==0){
            //hijo 2
            while(n>=i){
            pause();
            printf("hijo 2 %d\n",getpid());
            kill(h[0],SIGUSR1);
             i++;
            }
        }else if((h[2]=fork())==0){
            //hijo 3
            while(n>=i){
            pause();
            printf("soy hijo 3 %d\n",getpid());
            kill(h[1],SIGUSR1);
             i++;
            }
        }else if((h[3]=fork())==0){
            //hijo 3
            while(n>=i){
            pause();
            printf("soy hijo 4 %d\n",getpid());
            kill(h[2],SIGUSR1);
            i++;
            }

        }else if((h[4]=fork())==0){
            //hijo 3
            while(n>=i){
            pause();
            printf("soy hijo 5 %d\n",getpid());
            kill(h[3],SIGUSR1);
            i++;
            }

 	usleep(1000);

//direccionar cuando llegue al hijo 4

        }
        else{

        usleep(1000);


       while(n>=i){

            printf("soy el padre %d\n",getpid());

            kill(h[4],SIGUSR1);
            pause();


            if(i==1){

                kill(SIGTERM,getpid());

            }



            i++;
        }





}

return 0;
}
