#include <stdio.h>

int main()
{

	char str[80];

	printf("Please enter a message: ");
	fgets(str, sizeof(str), stdin);

	printf("The message you entered is: ");
	puts(str);
	
	return(0);
}
