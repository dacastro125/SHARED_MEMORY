#include<stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<time.h>


int main(){
    int tb1[2],tb2[2],tb3[2],tb4[2];
    pipe(tb1);
    pipe(tb2);
    pipe(tb3);
    pipe(tb4);
    int sw;
    
    
    if(fork()==0){
    	//hijo 1
    close(tb1[0]);
	close(tb2[1]);
	close(tb3[0]);
	close(tb3[1]);	
    close(tb4[0]);
	close(tb4[1]);	
      
    int  i,buster[4];
	time_t	inicio=time(NULL);
    time_t  final=inicio;
	do{
		srand(time(NULL));
		
		for(i=0;i<4;i++){
			buster[i]=rand()%7;
		    if(buster[i]>5){
		    sw=1;	 	
			}
			break;
		}
	    	
       	
	final=time(NULL);	
	}while((final-inicio)<60 && sw==0);

 	write(tb1[1],&sw,sizeof(sw));
		
/*	if(sw==1){
		//cuando no esta bien
		write(tb1[1],&sw,sizeof(sw));
		
		
		}else{
		//cuando todo esta bien		
		write(tb1[1],&sw,sizeof(sw));
	}
	
*/	
   read(tb2[0],&sw,sizeof(sw));
   if(sw==0){
   	printf("Hijo1 Murio");
   }
	}else if(fork()==0){
		//hijo 2
		close(tb1[0]);
	    close(tb1[1]);
		close(tb2[0]);
		close(tb2[1]);
		close(tb3[0]);
		close(tb4[1]);
	    
	    read(tb4[0],&sw,sizeof(sw));
	    if(sw==1){
	    printf("Abortar");
	    	
		}else{
		printf("Liberar capsula");
		
		}
	}else{
		//padre
		close(tb1[1]);
		close(tb2[0]);
		close(tb3[1]);
		close(tb4[0]);
		
		read(tb1[0],&sw,sizeof(sw));
		if(sw==1){
			//cuando no esta bien
		    write(tb4[1],&sw,sizeof(sw));  
		
		}else{
		    //cuando todo esta bien 
		    
		    
		    write(tb2[1],&sw,sizeof(sw));
		    wait(NULL);
		    write(tb4[1],&sw,sizeof(sw));
		    wait(NULL);
		}
		
		
	
	}
    
    
    
    
    
    return 0;
}

