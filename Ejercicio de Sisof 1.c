#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>

int main (){

int tb1[2],tb2[2],tb3[2],tb4[2];
int vector[10];
pipe(tb1);
pipe(tb2);
pipe(tb3);
pipe(tb4);

if(fork()==0){
close(tb1[0]);
close(tb2[1]);
close(tb3[0]);
close(tb3[1]);
close(tb4[0]);
close(tb4[1]);
read(tb2[0],(void*)vector,sizeof(vector));
int i,suma;
float promedio;
for(i=0;i<10;i++){
suma=suma+vector[i];
}
promedio=((float)(suma)/(float)10);
write(tb1[1],&promedio,sizeof(promedio));
printf("\nsoy el hijo e enviado el primedio:\n");

}else if(fork()==0){
close(tb1[0]);
close(tb1[1]);
close(tb2[0]);
close(tb2[1]);
close(tb3[1]);
close(tb4[0]);
read(tb3[0],(void*)vector,sizeof(vector));
int i,x,cont=0,max,cont2=0;
for(i=0;i<10;i++){
for(x=i+1;x<10;x++){
if(vector[i]==vector[x]){
cont=cont+1;
}}
if(cont2<cont){
max=vector[i];   
cont2=cont;    
cont=0;
    
}}
write(tb4[1],&max,sizeof(max));
write(tb4[1],&cont2,sizeof(cont2));
printf("soy el hijo2 mandando el maximo valor");
}else{
close(tb1[1]);
close(tb2[0]);
close(tb3[0]);
close(tb4[1]);
int i;
for(i=0;i<10;i++){
printf("vector[%d]:",i);
scanf("%d",&vector[i]);
}
write(tb2[1],(void*)vector,sizeof(vector));
wait(NULL);
float promedio;
read(tb1[0],&promedio,sizeof(promedio));
printf("\nPromedio:%.3f",promedio);
printf("\n");
write(tb3[1],(void*)vector,sizeof(vector));
wait(NULL);
int max,cont2;
read(tb4[0],&max,sizeof(cont2));   
printf("\nMaximo valor:[%d]",max);
read(tb4[0],&cont2,sizeof(cont2));
printf("\nSe repitio:[%d]",cont2);
}
    




return 0;
}