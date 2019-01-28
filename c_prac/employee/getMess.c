#include <stdio.h>
#include <string.h>

int main()
{
	const int sz = 80;
	char str[sz];

	printf("Please enter a message: ");
	fgets(str, sz, stdin);
	str[strlen(str)-1] = '\0';
	
	printf("The message you entered is: ");
	puts(str);
	
	return (0);
}
