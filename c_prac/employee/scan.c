#include <stdio.h>

int main()
{
	int x, y;
	char str1[20], str2[20];
	char line[80];

	printf("Enter a number, a number, a string and a string: ");
	fgets(line, sizeof(line), stdin);

	sscanf(line, "%d%d%s%s\n", &x, &y, str1, str2);
	printf("Entered data: %d %d %s %s\n", x, y, str1, str2);
	
	return (0);
}
