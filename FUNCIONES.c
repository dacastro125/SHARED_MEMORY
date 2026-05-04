#include <stdio.h>
#include <string.h>

typedef struct Nodo {
    int codigo;
    char nombre[50];
    int facultad;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;

Nodo *raiz = NULL;

int leer_archivo(char*);
void imprimir();
int guardar_archivo(char*);
int insertar_nodo(int, char*, int);
void eliminar_arbol(Nodo*);
void guardar_nodo(FILE *, Nodo*);
void insertar();

int main(int argc, char* argv[]) {

    char archivo[256]; //Nombre de archivo
    printf("Ingrese el nombre del archivo: ");
    scanf("%[^\n]s", archivo); //Leo el nombre del archivo, %[^\n]s hace que pueda leer espacios

    if (leer_archivo(archivo)) {
        printf("El archivo fue leido correctamente.\n");

        imprimir();
        insertar();
        imprimir();

        if (guardar_archivo(archivo)) {
            printf("El archivo se guardo correctamente");
        } else {
            printf("No se pudo guardar el archivo.");
        }
    } else {
        printf("No se pudo leer el archivo.\n");
    }

    eliminar_arbol(raiz);

    return 0;
}

void imprimir() {
    printf("CODIGO\tNOMBRE\tFACULTAD\n");
    in_order(raiz);
}

int leer_archivo(char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r"); //fopen abre un archivo, "r" indica que es en modo lectura

    if (archivo != NULL) { //Si archivo es NULL entonces no se pudo abrir

        char linea[256]; //Linea de archivo
        fgets(linea, 256, archivo); //fgets lee una linea completa
        //En este caso es para saltarme la primera linea del archivo que es la de los encabezados

        while (!feof(archivo)) { //feof dice si el archivo llegó al final
            int codigo = 25;
            char nombre[50] = "Holi :3";
            int facultad = 20;

            int eof = fscanf(archivo, "%d\t", &codigo); //fscanf funciona exactamente como scanf
            if (eof < 0) break;
            fscanf(archivo, "%[^\t]s", nombre);
            fscanf(archivo, "%d", &facultad);

            insertar_nodo(codigo, nombre, facultad); //cambia la función por la de insertar
        }

        fclose(archivo); //Cierro el archivo, SIEMPRE hay que cerrarlo

        return 1;
    }

    return 0;
}

int guardar_archivo(char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "w"); //fopen abre un archivo, "w" indica que es en modo escritura

    if (archivo != NULL) { //Si archivo es NULL entonces no se pudo abrir

        fprintf(archivo, "CODIGO\tNOMBRE\tFACULTAD\n"); //fprintf funciona exactamente como printf
        //solo que se pasa el archivo en el que se va a imprimir
        //imprimo el encabezado

        guardar_nodo(archivo, raiz);

        fclose(archivo); //Cierro el archivo, SIEMPRE hay que cerrarlo

        return 1;
    }

    return 0;
}

int insertar_nodo(int codigo, char *nombre, int facultad) {
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
	nuevo->codigo = codigo;
	nuevo->facultad = facultad;
	strcpy(nuevo->nombre, nombre);
	nuevo->izq = NULL;
	nuevo->der = NULL;

	if(!raiz){
		raiz = nuevo;
		return 1;
	}
	else{
		Nodo *aux = raiz;
		while(1){
			if(aux->codigo == codigo){
                free(nuevo);
				return 0;
			}
			else if(codigo < aux->codigo){
				if(aux->izq != NULL){
					aux = aux->izq;
				}
				else{
					aux->izq = nuevo;
					return 1;
				}
			}
			else{
				if(aux->der != NULL){
					aux = aux->der;
				}
				else{
					aux->der = nuevo;
					return 1;
				}
			}
		}
	}

}

void in_order(Nodo *raiz){
	if(raiz != NULL){
		in_order(raiz->izq);
		printf("%d\t%s\t%d\n", raiz->codigo, raiz->nombre, raiz->facultad);
		in_order(raiz->der);
	}
}

void guardar_nodo(FILE *archivo, Nodo *raiz) {
    if(raiz != NULL){
		guardar_nodo(archivo, raiz->izq);
		fprintf(archivo, "%.2d\t%s\t%.2d\n", raiz->codigo, raiz->nombre, raiz->facultad);
		guardar_nodo(archivo, raiz->der);
	}
}

void insertar() {
    int codigo = 0;
    char nombre[50];
    int facultad = 0;

    printf("Ingrese el codigo: ");
    scanf("%d", &codigo);

    fflush(stdin);
    printf("Ingrese el nombre: ");
    scanf("%s", nombre);

    printf("Ingrese la facultad: ");
    scanf("%d", &facultad);

    if (insertar_nodo(codigo, nombre, facultad) != 0) {
        printf("Insertado correctamente\n");
    } else {
        printf("El codigo de estudiante ya existe\n");
    }
}

void eliminar_arbol(Nodo *nodo) {
    if (nodo != NULL) {
        eliminar_arbol(nodo->izq);
        eliminar_arbol(nodo->der);

        free(nodo);
    }
}
