#include <stdio.h>
#include <math.h>
#define N 10

int main(int argc, char const *argv[])
{
    int i;
    float x[N], y[N], xy=0, sx=0, sy=0, x2=0, y2=0, mx, my, cov, desvx, desvy, corr;

    printf("\n================================================================\n");
    printf("Calculo del coeficiente de correlación de 2 variables");
    printf("\n================================================================\n");


    for (i = 0; i < N; i++){
        printf("X[%d]: ", i);
        scanf("%f", &x[i]);
        sx += x[i];
        x2 += pow(x[i],2);
    }

    printf("\n\n");

    for (i = 0; i < N; i++){
        printf("Y[%d]: ", i);
        scanf("%f", &y[i]);
        sy += y[i];
        y2 += pow(y[i],2);
    }

    printf("\n\n");

    //coef xy
    for (i = 0; i < N; i++){
        xy += x[i]*y[i];
    }

    //medias
    mx = sx/N;
    my = sy/N;

    //covarianza
    cov = (xy/N) - (mx*my);

    //desv estandar X y Y
    desvx = sqrt((x2/N) - pow(mx,2));
    desvy = sqrt((y2/N) - pow(my,2));

    corr = cov/(desvx*desvy);

    printf("XY: %f\n", xy);
    printf("Media de X: %f\n", mx);
    printf("Media de Y: %f\n", my);
    printf("Covarianza: %f\n", cov);
    printf("Desviacion estandar de X: %f\n", desvx);
    printf("Desviacion estandar de Y: %f\n", desvy);
    printf("Coeficiente de correlación: %f\n", corr);

    return 0;
}