#include<ctype.h>
#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>
#include <conio.h>

bool Redimensionar(int* ptr,size_t tam){
	int *nuevo;
	nuevo=realloc(ptr,tam*sizeof(int*));
	ptr=nuevo;
return true;
}


int main(){
bool redi;
int *vector; 
int tam;
int i,x;
printf("Dame tamaño del vector: "); 
scanf("%d", &tam); 

vector = (int *)malloc(sizeof(int)*tam); 
if(vector==NULL) { 
printf("Error de asignacion de memoria\n"); 
getch(); 
}
 
for( i=0; i<tam; i++){
	printf("ingrese numero");
	scanf("%i",&vector[i]);
}  


for(x=0;x<=tam;x++){
	if(x==tam){
	  	printf("NULL");
	  }else{
	  
		printf("%i<->",vector[x]);
}
	  
	
	}


	redi=Redimensionar(vector,5);
	if(redi==true){
	printf("\n");
	printf("si se pudo");
	}
	
	for(x=0;x<=5;x++){
	if(x==5){
	  	printf("NULL");
	  }else{
	  
		printf("%i<->",vector[x]);
}
	  
	
	}
	
	
	
	printf("\n");
	
	system("pause");
	return 0;


 

 

 
}

