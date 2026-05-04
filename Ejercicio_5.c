#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
	int fd[2], n;
	char buffer_in[1024];
	char buffer_out[1024];
	pipe(fd);
	//Copia la cadena de caracteres a buffer_out
	strcpy(buffer_out, "Hola mundo\n");
	//Escribe en el archivo fd[1] el contenido de buffer_out (hay que definir el tamaño que se desea escribir en bytes)
	//No se hace referencia a buffer_out ya que un vector es un puntero a su primera posicion
	write(fd[1], buffer_out, strlen(buffer_out));
	//Lee del archivo fd[0] y lo captura en buffer_in (se debe mandar el tamaño en bytes que va a leer)
	//read es una funcion bloqueante. Hasta que haya algo que leer bloqueara el proceso o si se esta escribiendo cuando se intenta leer
	n = read(fd[0], buffer_in, 1024);
	//\0 define el fin de la impresion en pantalla, ya que se leen 1024 bytes, lo cual, generaria basura en el restante del vector
	buffer_in[n] = '\0';
	printf("Leida %s\n", buffer_in);
	return 0;
}