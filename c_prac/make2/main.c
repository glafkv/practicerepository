#include <stdio.h>

int square(int);
int cube(int);

int main(void)
{
	int n, s, c;
	//Asks the user for input.
	printf("Please enter a number: ");
	//Scanf takes in users input as integer n.
	scanf("%d", &n);
	
	//Calls the function and uses them from users input.
	s = square(n);
	c = cube(n);
	
	//%d is needed to display a digit. n was what was taken in, s was the function called. 
	printf("Square of %d is %d\n", n, s);
	printf("Cube of %d is %d\n", n, c);
	
	return (0);
}
