#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Tuberias

int main(int argc, char * argv)
{
int tub1[2],tub2[2];
pipe(tub1);
pipe(tub2);

	FILE *archivo;
	int n,i=0,index=0;
	int v[2];
	
	
	archivo = fopen("prueba.txt","r");
	
	if (archivo == NULL){

		printf("\nError de apertura del archivo. \n\n");
    }else{
        
	    
	    printf("\nEl contenido del archivo de prueba es \n\n");
	    
	    
	    while (feof(archivo) == 0){ 
		    fscanf(archivo,"%d;",&n); 	
			v[index] = n;  index ++;	
		}
	   
	   
	   for( i=0; i<index; i++) { 
	   		v[i]=v[i]; 
	   	 }
	   
	   for( i=0; i<index;i++ )
	     printf("%d\n",v[i]);
     	   
    	}
    

if(fork()==0)
{
	//HIJO1
close(tub1[1]);
close(tub2[0]);

int suma;
read(tub1[0], &v, sizeof(index));
suma=v[0]+v[1];
write(tub2[1], &suma, sizeof(suma));
}
else{
	//PADRE
	close(tub1[0]);
	close(tub2[1]);
	
	int v[index], r1;
	
	write(tub1[1], &v, sizeof(v));
	
	read(tub2[0], &r1, sizeof(r1));
	
	printf("suma: %d\n", r1);
	
}    
	fclose(archivo);
        
	return 0;
}


