#include <stdio.h>

int square(int);

int cube(int x)
{
	return(x * square(x));
}
