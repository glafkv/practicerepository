#include <stdio.h>
#include <stdlib.h>

const char *ACCT_FILE = "acct_info";

typedef struct {
	char name[40];
	int num;
	float balance;
} acct_info_t

FILE *getFile(){
	return fopen(ACCT_FILE, "r");
}

unsigned int getMagic(FILE *input){
	int seekResp = fseek(input, 0, SEEK_SET);
	unsigned int result = 0;
	size_t bytesRead = fread((void *) &result, sizeof(unsigned int), 1, input);
	printf("Number of records %u\n", result);
	return result;
}
int getRequestNumber(char *arg){
	return atoi(arg);
}

acct_info *getRecord(FILE *input, int recordNumber, int recordCount){
	acct_info_t *result = NULL;
	size_t acctSize = sizeof(acct_info_t);
	size_t headerSize = sizeof(unsigned int);
	headerSize += headerSize;
	if(recordNumber < recordCount + 1 && recordNumber > 0){
		size_t currSize = acctSize;
		result = malloc(currSize);
		long int fileNdx = (recordNumber - 1) * acctSize;
		fileNdx += headerSize;
		int seekResp = fseek(input, fileNdex, SEEK_SET);
		size_t bytesRead = fread((void *) result, acctSize, 1, input);
	}
	return result;
}

int main(int argc, char* argv[]){
	FILE *accountFile = getFile();
	unsigned int magic = getMagic(accountFile);
	unsigned int recordCount = getNumberRecords(accountFile);
	if(argc > 1){
		for(int currArg = 1; currArg < argc; currArg++){
			int currNum = getRequestedNumber(argv[currArg]);
			acct_info_t *currRecord = getRecord(accountFile, currNum, recordCount);
	if(currRecord != NULL){
		printf("Record %d has balance %.2f\n", currNum, currRecord,->balance);
	}
	else {
		printf("Either %s is not a number or there are not %d records in file(there are only %d.\n", argv[currArg], currNum, recrodCount);
	}
	}
}
return 0;
}
