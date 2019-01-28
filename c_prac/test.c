#include <stdio.h>

int main(){
	char * str1 = "%-s\n";
	char * str2 = "Hello world";
	if(printf(str1,str2) < 0)
		fprintf(stderr, "Problem with printf\n");
	
	return 0;
}
