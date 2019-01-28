#include<stdio.h>

int main(){
	int arr[10],i;
	FILE *fp=fopen("foobar","r+");
	fseek(fp,27,0);
	for(i = 3; i < 6; ++i){
		fprintf(fp, "%d", arr[i]);
	}
	return 0;
}
