#include <stdio.h>

int main ( int argc, char ** argv )
{
	char msg[] = "Hello ";
	if ( argc == 1 )
		fprintf ( stderr, "%s: %s %s\n", argv[0], msg, "world" );
	else
		fprintf ( stderr, "%s: %s %s\n", argv[0], msg, argv[1] );

	return ( 0 );
}
