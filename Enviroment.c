#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define null NULL

extern char **environ;

int main(void){
	//obtener toda la lista de variables de entorno
	char **List = environ;
	char *var;
	while(*List != null){
		printf("%s\n", *List);
		List++;
	}
	
	//Obtener el valor de una variable especifica
	var = getenv("LOGNAME");
	printf("%s\n",var);
	return EXIT_SUCCESS;
}
