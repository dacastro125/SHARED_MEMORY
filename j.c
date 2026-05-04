#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

void funcion_principal(){

	printf("Soy el proceso -> %d \n", getpid());

}

int main(int argc, char *argv[]){
	pid_t sig,v[]={-1,-1},padre;
	padre = getpid();
	int j;	

	signal(SIGUSR1, funcion_principal); 

	 if((sig=fork())==0){
	    if((sig=fork())==0){										 						      
                for(j=0;j<2;j++){
		   if((v[j]=fork())==0){
		      if(j==1){
		   	sig = v[0];
		       }
		       break;
	           }
	            sig=v[j];
		}									
	    }
         }
	
	if(getpid() == padre){

	   printf("\nSoy el padre -> %d\n",padre);
	   usleep(10000);
	   kill(sig, SIGUSR1);
	   wait(NULL);

	}else{
	      pause();
		if(v[0]==0){
			
		}else{
		      kill(sig, SIGUSR1);
		}
	}
}



