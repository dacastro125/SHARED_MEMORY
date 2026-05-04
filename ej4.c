#include<stdio.h>
#include<unistd.h>
#include<signal.h>

	int main(){
		int c,i;
		scanf("%d",&c);
			if(!fork()){
				for(i=0;i<c;i++)
					printf("hijo\n");
				}
				else{
					for(i=0;i<c;i++)
						printf("padre\n");
					}
			return 0;

		}
