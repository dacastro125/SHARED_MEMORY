#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
int i=0, tuberia[3][2],n, valor;
pipe(tuberia[0]);
pipe(tuberia[1]);
pipe(tuberia[2]);

printf("digite numero\n");
scanf("%d",&valor);

	for (i=0; i<3; i++){
		if (fork()==0){
		break;	
		}
	}

if (i==3){
close (tuberia[0][0]);
close (tuberia[1][0]);
close (tuberia[2][0]);
write (tuberia[0][1],&valor, sizeof(valor));
write (tuberia[1][1],&valor, sizeof(valor));
write (tuberia[2][1],&valor, sizeof(valor));
}
	
if (i==0){
close (tuberia[0][1]);
close (tuberia[1][1]);
close (tuberia[2][1]);
close (tuberia[1][0]);
close (tuberia[2][0]);
read (tuberia[0][0],&valor, sizeof(valor));
printf ("el primer hijo [%d] muestra %d\n",getpid(),valor+2);
wait(NULL);
}
if (i==1){
close (tuberia[0][1]);
close (tuberia[1][1]);
close (tuberia[2][1]);
close (tuberia[0][0]);
close (tuberia[2][0]);
read (tuberia[1][0],&valor, sizeof(valor));
printf ("el segundo hijo [%d] muestra %d\n",getpid(),valor+3);
wait(NULL);
}

if (i==2){
close (tuberia[0][1]);
close (tuberia[1][1]);
close (tuberia[2][1]);
close (tuberia[0][0]);
close (tuberia[1][0]);
read (tuberia[2][0],&valor, sizeof(valor));
printf ("el tercer hijo [%d] muestra %d\n",getpid(),valor+4);
wait(NULL);
}



}//FINAL

