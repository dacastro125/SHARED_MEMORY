#include <stdio.h>
#include <unistd.h>

int globalNum = 84; 

int main(int argc, char *argv[]){
	
	//int globalNum = 152;
	printf("helloExec.c: and globalNum: %d\n", globalNum);
	
	globalNum++;
	printf("	My pid %d.\n", getpid());

	printf("	My arguments is:  ");
	for(int i=0;i<argc;i++){
		printf("%s, ", argv[i]);	
	}
	
	printf("\n");
return 0;
}

