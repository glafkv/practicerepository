#include <stdio.h>

int main()
{
	char firstName[30], lastName[30], location[30];
	int hours;
	float hourly_rate, earned;
	
	printf("Please enter the first and last name of the employee: ");
	scanf("%s%s", firstName, lastName);
	printf("Please enter the number of hours worked: ");
	scanf("%d",&hours);
	printf("Please enter the hourly rate: ");
	scanf("%f", &hourly_rate);
	printf("Please enter location: ");
	scanf("%s", location);
	
	earned = hours * hourly_rate;
	
	printf("%s %s earned $%.2f in %s\n", firstName, lastName, earned, location);

	return (0);
}
