#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> /* <--- Incluye las definiciones de la biblioteca pthread */

#define N 4  /* establece el tamano de las matrices a multiplicar */

typedef struct _mis_parametros
{
  int inicio;
  int fin;
} mis_parametros;

float a[N][N], b[N][N], c[N][N];

void inicializa(float matriz[N][N])
{
  int i, j;
  for (i = 0; i < N; i++)
    for(j = 0; j < N; j++)
      matriz[i][j] = random()%5;
}
float a[N][N], b[N][N], c[N][N];

void imprime(char * c, float matriz[N][N])
{
  int i, j;

  printf("\n\n-------------------------\n");
  printf("%s", c);
  for (i = 0; i < N; i++){
    for(j = 0; j < N; j++){
      printf("[%.f]", matriz[i][j]);
    }
    printf("\n");
  }
  printf("-------------------------\n\n");

}

void* m_x_m(void *parametros)
{
  int i, j, k;
  int renglon_inicio;
  int renglon_fin;
  mis_parametros *datos;

  datos = (mis_parametros *) parametros;
  renglon_inicio = datos -> inicio;
  renglon_fin = datos -> fin;

  for (i = renglon_inicio; i < renglon_fin; i++)
    for (j = 0; j < N; j++)
      {
	c[i][j] = 0;
	for (k = 0; k < N; k++)
	  c[i][j] = c[i][j] + a[i][k]*b[k][j];
      }
}


int main()
{
  pthread_t thread1_id;
  pthread_t thread2_id;

  mis_parametros datos_thread1;
  mis_parametros datos_thread2;

  inicializa (a);
  inicializa (b);

  imprime("MATRIZ a:\n", a);
  imprime("MATRIZ b:\n", b);

  /* Cada hilo calcula la mitad de la matriz resultante */
  /* El hilo uno calcula los primeros N/2 renglones */
  datos_thread1.inicio = 0;
  datos_thread1.fin = N/2;

  /* el hilo dos calcula los renglones restantes */
  datos_thread2.inicio = datos_thread1.fin;
  datos_thread2.fin = N;


  pthread_create (&thread1_id, NULL, &m_x_m, &datos_thread1);
  pthread_create (&thread2_id, NULL, &m_x_m, &datos_thread2);

  pthread_join(thread1_id, NULL);
  pthread_join(thread2_id, NULL);

  imprime("MATRIZ c:\n", c);
  return 0;
}
