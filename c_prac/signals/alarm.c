#include <stdio.h>
#include <unistd.h>

int main()
{
	int i;
	alarm ( 10 );
	for(i = 0; ; i++)
	{
		printf("%d\n", i);
		sleep(1);
	}
}
