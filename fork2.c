#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int Vec [20]={1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};
int status;
int suma1,suma2;

int main (){

if (fork()!=0){
        int i=0;
	printf ("esta en el primer hijo\n");
              
	        for(i=0;i<10;i++){
                 suma1=suma1+Vec[i];
	     	}
	exit(suma1);	
}
 
	printf("%d\n",suma1>>8);
}
