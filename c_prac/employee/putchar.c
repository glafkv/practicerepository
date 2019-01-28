#include <stdio.h>

int main()
{
	char ch;
	
	printf("Please enter a character: ");
	ch = getchar();
	
	printf("The character you entered is: ");
	putchar(ch);
	putchar('\n');

	return (0);
}
