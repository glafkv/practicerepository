#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void phone_fmt(int argc,char *argv[])
{
	int r = 0, k, len = 0;
	int i;
	for(i = 1; i < argc; i++)
	{
		len = strlen(argv[i]);
		int *array = (int*) malloc(len * sizeof(int));
		if(array == NULL) {fputs ("Memory error", stderr); exit(2);}

		k = 0;
		int j;
		for(j = 0; j <len; j++)
		{
			if(isdigit(argv[i][j]))
			{
				r = argv[i][j] - '0';
				array[k] = r;
				k++;
			}
		}
	
	if (k < 10)
	{
		printf("Not enough digits");
		exit(-1);
	}
	printf("(%d%d%d) %d%d%d-%d%d%d%d\n" , array[0], array[1], array[2], array[3], array[4], array[5], array[6], array[7], array[8], array[9]);
	free(array);
	}
}
