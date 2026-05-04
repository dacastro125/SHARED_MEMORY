#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#define NH 4

typedef struct barrier_t {
    int c;
    int total;
    int released;
} barrier_t;

void barrier_wait(barrier_t* b){

    __sync_and_fetch(&b->c, 1);
    if(b->c == b->total){
        b->released = 1;
    }else{
        while(!b->released) usleep(1000);
    }

}

void copy_matrix(int **SRC, int **DEST, int f, int c)
{
    for (int i = 0; i < f; i++)
    {
        for (int j = 0; j < c; j++)
        {
            DEST[i][j] = SRC[i][j];
        }
    }
}

int **read_f(const char *fname, int *f, int *c, int *d)
{
    FILE *fp = fopen(fname, "r");
    if (!fp)
    {
        perror("file");
        abort();
    }
    int ff, cc, dd;

    fscanf(fp, "%d", &dd);
    fscanf(fp, "%d", &ff);
    fscanf(fp, "%d", &cc);
    *f = ff;
    *c = cc;
    *d = dd;

    int **m = (int **)malloc(sizeof(int *) * ff);
    for (int i = 0; i < ff; i++)
    {
        m[i] = (int *)malloc(sizeof(int) * cc);
    }

    for (int i = 0; i < ff; i++)
    {
        for (int j = 0; j < cc; j++)
        {
            fscanf(fp, "%d", &m[i][j]);
        }
    }
    fclose(fp);
    return m;
}

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        perror("args");
        return 1;
    }
    int f, c, d;
    int **m = read_f(argv[1], &f, &c, &d);

    int shmid = shmget(IPC_PRIVATE, f * c * sizeof(int) + f * sizeof(int), 0666 | IPC_CREAT);
    void *base = shmat(shmid, NULL, 0);
    int **M = (int **)base;
    int *dM = (int *)((char *)base + f * sizeof(int *));
    for (int i = 0; i < f; i++)
    {
        M[i] = &dM[i * c];
    }

    int shmid2 = shmget(IPC_PRIVATE, f * c * sizeof(int) + f * sizeof(int), 0666 | IPC_CREAT);
    void *baseR = shmat(shmid2, NULL, 0);
    int **R = (int **)baseR;
    int *dR = (int *)((char *)baseR + f * sizeof(int *));
    for (int i = 0; i < f; i++)
    {
        R[i] = &dR[i * c];
    }

    int shmid_status = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);
    int *status = shmat(shmid_status, NULL, 0); // Variable para que el padre avance al siguiente dia
                                                // Y los hijos esperen
                                                // 0 -> padre tiene el control
                                                // 1 -> hijos tienen el control
    int shmid_turn = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);
    int *turn = shmat(shmid_turn, NULL, 0);

    int shm_b = shmget(IPC_PRIVATE, sizeof(barrier_t), 0666 | IPC_CREAT);
    barrier_t *bar = shmat(shm_b, NULL, 0);

    *status = 0;
    *turn = NH;

    copy_matrix(m, M, f, c);

    for (int i = 0; i < f; i++)
    {
        for (int j = 0; j < c; j++)
        {
            R[i][j] = 0;
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }
    copy_matrix(M, R, f, c);

    pid_t padre = getpid();

    int i = 0;
    for (; i < NH; i++)
        if (!fork())
            break;

    if (padre == getpid())
    {
        printf("Turno del padre\n");
        int current_day = 0;
        while (current_day < d)
        {

            while (*status == 1 && *turn != NH)
                ;

            printf("Dia: %d\n\n", current_day);
            for (int i = 0; i < f; i++)
            {
                for (int j = 0; j < c; j++)
                {
                    printf("%d ", R[i][j]);
                }
                printf("\n");
            }
            copy_matrix(R, M, f, c);
            printf("\n\n");

            current_day++;
            *status = 1;
            *turn = 0;
        }

        for(int i = 0; i < NH; i++) wait(NULL);
    }
    else{

        #define D 1
        int c_d = 0;
        while (c_d < d)
        {

            while (*status == 0 && *turn != i)
                ;
            printf("Hijo %d desbloqueado\n", i);
            if (i == 0)
            { // De baja a media
                for (int ii = 0; ii < f; ii++)
                {
                    for (int jj = 0; jj < c; jj++)
                    {
                        int i_x, d_x;
                        int i_y, d_y;
                        i_x = ii - D;
                        d_x = ii + D;
                        i_y = jj - D;
                        d_y = jj + D;
                        int cont_neigh = 0;
                        for (int it = i_x; it < d_x; it++)
                        {
                            for (int jt = i_y; jt < d_y; jt++)
                            {
                                if (it >= 0 && jt >= 0 && it < f && jt < c && it != ii && jt != jj)
                                {
                                    if (M[it][jt] >= 1){
                                        //printf("Vecino alto o bajo\n");
                                        cont_neigh++;
                                    }
                                }
                            }
                        }
                        if (cont_neigh >= 4)
                        {
                            printf("Cambiando de baja a media\n");
                            R[ii][jj] = 1;
                        }
                    }
                }
            }
            else if (i == 1)
            {
                for (int ii = 0; ii < f; ii++)
                {
                    for (int jj = 0; jj < c; jj++)
                    {
                        int i_x, d_x;
                        int i_y, d_y;
                        i_x = ii - D;
                        d_x = ii + D;
                        i_y = jj - D;
                        d_y = jj + D;

                        int low_neigh = 0;

                        for (int it = i_x; it < d_x; it++)
                        {
                            for (int jt = i_y; jt < d_y; jt++)
                            {
                                if (it >= 0 && jt >= 0 && it < f && jt < c && it != ii && jt != jj)
                                {
                                    if (M[it][jt] == 0)
                                        low_neigh++;
                                }
                            }
                        }
                        double low_prob = 0.1 + (low_neigh * 0.03);
                        int pass = rand() < low_prob;
                        if (pass)
                            R[ii][jj] = 0;
                    }
                }
            }
            else if (i == 2)
            {
                for (int ii = 0; ii < f; ii++)
                {
                    for (int jj = 0; jj < c; jj++)
                    {
                        int i_x, d_x;
                        int i_y, d_y;
                        i_x = ii - D;
                        d_x = ii + D;
                        i_y = jj - D;
                        d_y = jj + D;

                        int high_neigh = 0;

                        for (int it = i_x; it < d_x; it++)
                        {
                            for (int jt = i_y; jt < d_y; jt++)
                            {
                                if (it >= 0 && jt >= 0 && it < f && jt < c && it != ii && jt != jj)
                                {
                                    if (M[it][jt] == 2)
                                        high_neigh++;
                                }
                            }
                        }

                        if (high_neigh >= 3)
                            R[ii][jj] = 2;
                    }
                }
            }
            else if (i == 3)
            {
                for (int ii = 0; ii < f; ii++)
                {
                    for (int jj = 0; jj < c; jj++)
                    {
                        int i_x, d_x;
                        int i_y, d_y;
                        i_x = ii - D;
                        d_x = ii + D;
                        i_y = jj - D;
                        d_y = jj + D;

                        int low_neigh = 0;

                        for (int it = i_x; it < d_x; it++)
                        {
                            for (int jt = i_y; jt < d_y; jt++)
                            {
                                if (it >= 0 && jt >= 0 && it < f && jt < c && it != ii && jt != jj)
                                {
                                    if (M[it][jt] == 0)
                                        low_neigh++;
                                }
                            }
                        }
                        double low_prob = 0.15 + (low_neigh * 0.05);
                        int pass = rand() < low_prob;
                        if (pass)
                            R[ii][jj] = 1;
                    }
                }
            }
            //barrier_wait(bar);
            *turn++;
            *status=0;
            c_d++;
        }
    }

    return 0;
}