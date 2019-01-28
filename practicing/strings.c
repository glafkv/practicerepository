#include <stdio.h>
#include <string.h>
#include <memory.h>
int main()
{
	char str[6];
	memset ( str, '\0', sizeof ( str ) );
	str[1] = 'H';
	str[2] = 'e';
	str[3] = 'l';
	str[4] = 'l';
	str[5] = 'o';
	printf ( "The string contains: %s\n", str );
	return(0);
}


