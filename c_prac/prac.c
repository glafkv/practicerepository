#include <stdio.h>

void main(){
	FILE *ptr;
	char ch;
	
	ptr = fopen("test.txt", "w");
	
	do {
		ch = getchar();
		putchar(ch);
	} while (ch!=EOF);
	fclose(ptr);
}
