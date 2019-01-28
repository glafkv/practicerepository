#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
	FILE * file;
	file = fopen("customers", "a+");
	float current = 0;
	float arrive = 0;
	float service = 0;

	srand(time(NULL));

	for(current = 0; current <= 3600; current = current + 5){
		arrive = current;
		service = rand() % 301 + 100;	
		fprintf(file, "%f \t %f \n", arrive, service);
	}
	
	fclose(file);
	return 0;

}
