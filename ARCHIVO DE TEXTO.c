#include <stdio.h>
#include <stdlib.h>


int main()
{
	FILE *archivo;
	int n,i=0,index=0;
	int vector[100];
	
	
	archivo = fopen("prueba.txt","r");
	
	if (archivo == NULL){

		printf("\nError de apertura del archivo. \n\n");
    }else{
        
	    
	    printf("\nEl contenido del archivo de prueba es \n\n");
	    
	    
	    while (feof(archivo) == 0){ 
		    fscanf(archivo,"%d;",&n); 	
			vector[index] = n;  index ++;	
		}
	   
	   int v[index];
	   
	   for( i=0; i<index; i++) { v[i]=vector[i];  }
	   
	   for( i=0; i<index;i++ )  printf("%d\n",v[i]);
     	   
    }
    
        
	fclose(archivo);
        
   
        
	return 0;
}

