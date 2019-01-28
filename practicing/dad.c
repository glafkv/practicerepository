#include<stdio.h>
#include<string.h>
int main()
{
	int i = 0, j, t = 0;
	char name[50];
	printf ( "What is your name? " );
	scanf ( "%s", name );
	for ( ; i < strlen ( name ); i++ ) 
		for ( j = 'a'; j <= name[i]; j++ )
			t++;
	printf ( "%d\n", t );
return 0;
}
