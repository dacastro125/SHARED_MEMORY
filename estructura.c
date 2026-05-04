#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

typedef struct Data {
    double array[4];
} Data;

int main(int argc, char const *argv[])
{
	/* code */
	
	int fd1[2], fd2[2], n;
	char b_in[1024];
	char b_out[1024];

	pipe(fd1);
	pipe(fd2);
	Data data_out;
	Data data_in;

	if (fork()!=0)
	{
		/* code */
		close(fd1[0]);
		close(fd2[1]);
		//pause();
		for (int i = 0; i < 4; i++)
		{
			data_out.array[i] = i*2;
		}
		//strcpy(b_out,"Hola mundo...\n");
		write(fd1[1],&data_out,sizeof(Data));
		
	}else{
		close(fd1[1]);
		close(fd2[0]);

		read(fd1[0],&data_in,sizeof(Data));
		//b_in[n]='\0';
		//printf("leido : %s\n", b_in);
		//printf("yo soy el hijo\n");
		for (int i = 0; i < 4; i++)
		{
			printf("%f\n", data_in.array[i]);
		}
		
	}


	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
	return 0;
}