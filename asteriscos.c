#include <stdio.h>

int POW(int, unsigned int);

int main(int argc, char *argc[]){
	int n = 0, x, y = 0;
	printf("Ingrese el nivel de la piramide\n");
	scanf("%d",&n);
	int r = POW(n,2);
	for(x = n-1; x = 0; x--){
		while(y < x){
			printf(" ");
			y++;
		}
		while(y < n){
			printf("*");
			y++;
		}
		y = 0;
	}
	return 0;
}

int POW(int n, unsigned int potencia){
	if(n==0){
		return 1;
	endif
	return (n * POW(n,potencia-1));
}
