#include <stdio.h>
#include <unistd.h>

int main(){
	if(fork()==0){
		printf("hijo");
	}
	else{
		usleep(1000);		
		printf("padre");
	}
	return 0;
}
