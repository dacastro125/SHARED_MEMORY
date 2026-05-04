#include<ctype.h>
#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>
#include <conio.h>

typedef struct vector{
size_t temp;
void **datos;//== void* datos[];arreglo dinamico  generico

}vector_t;

vector_t* Crear_Vector(size_t tamano){
	vector_t* ptr1=malloc(sizeof(vector_t));
	if(ptr1==NULL) return NULL;
	ptr1->temp=tamano;
	ptr1->datos=malloc(sizeof(void*)*tamano);
	if(ptr1->datos==NULL){
		free(ptr1);
		return NULL;
	}
	//datos ya viene siendo un vector ptr1->datos[]
	
	//limpiamos el vector
	int i;
	for( i=0; i<=tamano; i++){
		ptr1->datos[i]=NULL;
	}
	
	return ptr1;
}

bool AgregarE(vector_t* ptr1,size_t indice,void* elemento){
    if(indice<0 || indice>=ptr1->temp)	 return false;
	 ptr1->datos[indice]=elemento;
    return true;	
}



void* ObtenerE(vector_t* ptr1,size_t indice){
if(indice<0 || indice>ptr1->temp) return NULL;

return ptr1->datos[indice];

}

bool Redimiensionar(vector_t* ptr1,size_t tamano){
	void* *nuevo;
	nuevo=realloc(ptr1->datos,tamano*sizeof(void*));
	 
	ptr1->datos=nuevo;
	ptr1->temp=tamano;
	return true;
}

void destruir(vector_t* ptr1){
	free(ptr1->datos);
	free(ptr1);
}

int main(){
	bool f;
	int x,y,z;
	printf("ingrese tamaño");
	scanf("%i",&z);
	vector_t* VECTOR= Crear_Vector(z);
   
   
	//LLENANDOLO MEIDNATE FOR 
   for(y=0;y<=VECTOR->temp;y++){
   		printf("ingrese numero");
		   scanf("%i",&VECTOR->datos[y]);
	}
	
  /* char c[1];
	printf("Desea redimensionar le vector?");
	scanf("%s",&c);
	if(strcmpi(c,"n")!=0){
	printf("ingreso a redimnsioanr el vector");
		printf("\n");
		printf("\n por favor ingrese  le nuevo tamñao del vector");
		int q;
		scanf("%i",&q);
	f=Redimiensionar(VECTOR,q);
	if(f==true){
		
		for(y=0;y<=VECTOR->temp;y++){
   		printf("ingrese numero");
		   scanf("%i",&VECTOR->datos[y]);
	}
	}}*/
	
/*for(x=0;x<=VECTOR->temp;x++){
		printf("%i<->",ObtenerE(VECTOR,x));
	   if(x==VECTOR->temp){
	   	printf("NULL");
		}
	
	}*/
   
	/*
	int*a=1;
   int*b=2;
	char*c="santiago";
	AgregarE(V,0,c);
	AgregarE(V,1,a);
	AgregarE(V,2,b);
	
	int *elemento1=(int*)(ObtenerE(V,1));
	printf("%i",elemento1);
	
	char *elemento2=(char*)(ObtenerE(V,0));
	printf("%s",elemento2);
	*/	
	
	//PUEDO IMPRIMIR UN PUNTERO
	/*i=ObtenerE(VECTOR,1);
	printf("%i\n",i);
	*/
	
	//IMPRIMIR EL VECTOR
/*for(x=0;x<=VECTOR->temp;x++){
		printf("%i<->",VECTOR->datos[x]);
	   if(x==VECTOR->temp){
	   	printf("NULL");
		}
	
	}*/
	printf("\n");
	
	system("pause");
	return 0;
	
}
