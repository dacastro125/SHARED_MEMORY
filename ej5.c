#include<stdio.h>
#include<unistd.h>
#include<signal.h>

		void manejador(){}

		int main(){

		int i,c,pidh;
			signal(SIGUSR1,manejador);
				scanf("%d",&c);
				pidh=fork();
					if(pidh==0){

						for(i=0;i<c;i++){
							pause();
								printf("hijo\n");
								kill(getppid(),SIGUSR1);
						    		}
						    } else  {

									for(i=0;i<c;i++)
										{
										usleep(100000);
										printf("Padre\n");
										kill(pidh,SIGUSR1);
										pause();
										}
								}


			   }
