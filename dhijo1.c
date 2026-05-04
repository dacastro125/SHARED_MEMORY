#include <stdio.h>


int main(){
	
	int i,j;
	
	i=fork();
	

	if(i==0){
		j=fork();
		if(j==0){
			printf("soy el hijo1");
		}
		else{
			printf("soy el hijo2");
		}
	}
	else{
		printf("soy el padre");
	}
	return 0;
}
