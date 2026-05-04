#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int Vec[] = {1,2,1,4,1,4,5,9,5,1,2,2};
int VecC = 12;

int main(int argc, char *argv[]){
	int i = 0;
	int rhijo1 = 0, rhijo2 = 0;
	pid_t hijo1;
	pid_t hijo2;
	if((hijo1 = fork()) == 0){
		int Suma1 = 0;
		printf("Hijo 1 (%d, hijo de %d)\n",  getpid(), getppid());
		for(i=0;i<VecC/2;i++){
			Suma1 = Suma1 + Vec[i];
		}
		printf("Suma 1 = %d\n",Suma1);
		exit(Suma1);
	}
	else{
		if((hijo2 = fork()) == 0){
			int Suma2 = 0;
			printf("Hijo 2 (%d, hijo de %d)\n",  getpid(), getppid());
			for(i=(VecC/2);i<VecC;i++){
				Suma2 = Suma2 + Vec[i];
			}
			printf("Suma 2 = %d\n",Suma2);
			exit(Suma2);
		}
		else{
			waitpid(hijo1, &rhijo1, 0);
			waitpid(hijo2, &rhijo2, 0);
			printf("Padre (%d, hijo de %d)\n", getpid(), getppid());
			printf("El resultado es %d\n", ((rhijo1>>8)+(rhijo2>>8)));
		}
	}
}
