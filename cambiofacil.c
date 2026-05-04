#include <stdio.h>

void imprimir(int vector[5]){
	int i;
	for(i=0;i<5;i++){
		printf("%d",vector[i]);
	}
}

void OrdenarDes(int vector[5]){
		int i,j,*Cabeza,*Recorrer,aux;
		for(i=0;i<5;i++){
		Cabeza=&vector[i];
		for(j=i+1;j<5;j++){
			Recorrer=&vector[j];
			if(*Cabeza<*Recorrer){
			aux=*Cabeza;
			*Cabeza=*Recorrer;
			*Recorrer=aux;
			}
		}
		
	}
}

void suma(int vector[5]){
int *p,*q,i,total[5];
p=vector;
q=vector;
for(i=0;i<5;i++){
total[i]=*(p+i)+*(q+i);
printf("%d",total[i]);
}

}

int main(){
	int i;
	int vector[5];
	for(i=0;i<5;i++){
	
	printf("ingrese numero");
	scanf("%d",&vector[i]);
}
    OrdenarDes(vector);
	imprimir(vector);
	printf("\n");
    suma(vector);    	

	return 0;
}
