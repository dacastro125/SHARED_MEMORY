#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    if(argc < 3){
        perror("Send n elements and number of file\n");
        return EXIT_FAILURE;
    }
    int n = atoi(argv[1]);
    int number = atoi(argv[2]);
    char filen_name[64];
    sprintf(filen_name, "data_%d.in", number);

    FILE *fl = fopen(filen_name, "w");
    if(!fl){
        perror("Error opening file\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fl, "%d\n", n);

    for(int i=1; i < n; i++){
        fprintf(fl, "%d\n", (rand() % 100) + 1);
    }

    printf("The file %s has been create success.\n", filen_name);

    fclose(fl);
    return EXIT_SUCCESS;
}