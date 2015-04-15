// COMP 2717 Assignment Six - indexFunc.c
// Author: H Lorenzana

#include "indexFunc.h"

// Reads the data file provided
void indexDataFile(char *dataFile) {
	FILE *accountsFilePtr;
	FILE *indexFile;

	int readCount;
	int writeCount;
	int dataRecordCount;
	int indexRecordCount;
	long filePos;

	IndexHeader indexHeader;
	IndexRecord *indexRecordsLastName;
	IndexRecord *indexRecordsBalanceASC;
	IndexRecord *indexRecordsBalanceDSC;
	CustomerFile customer = { 0, "", "", 0.0, 0.0 };

	if ((accountsFilePtr = fopen(dataFile, "rb")) == NULL) {
		printf("Unable to open file.\n\n");
		exit(EXIT_FAILURE);
	}
	else {

		// Open index file for writing
		if ((indexFile = fopen("accounts.cdx", "wb")) == NULL) {
			printf("ERROR - can not open/create index file.\n\n");
			exit(EXIT_FAILURE);
		}

		// Calculate record count in data file
		dataRecordCount = fileSize(accountsFilePtr) / sizeof(CustomerFile);

		// Allocate arrays for INDEX RECORDS
		indexRecordsLastName = (IndexRecord *)calloc(dataRecordCount, sizeof(IndexRecord));
		indexRecordsBalanceASC = (IndexRecord *)calloc(dataRecordCount, sizeof(IndexRecord));
		indexRecordsBalanceDSC = (IndexRecord *)calloc(dataRecordCount, sizeof(IndexRecord));

		// Populate indexHeader
		strcpy(indexHeader.appName, APP_NAME);
		indexHeader.recCount = dataRecordCount;

		rewind(accountsFilePtr);
		filePos = ftell(accountsFilePtr);
		indexRecordCount = 0;

		// Read first record
		readCount = fread(&customer, sizeof(CustomerFile), 1, accountsFilePtr);

		// Process all data records
		while (!feof(accountsFilePtr) && (readCount == 1)) {
			// Populate index record from data record
			strcpy(indexRecordsLastName[indexRecordCount].key.lastName, customer.lastName);
			indexRecordsLastName[indexRecordCount].filePos = filePos;

			indexRecordsBalanceASC[indexRecordCount].key.accountBalance = customer.accountBalance;
			indexRecordsBalanceASC[indexRecordCount].filePos = filePos;

			indexRecordsBalanceDSC[indexRecordCount].key.accountBalance = customer.accountBalance;
			indexRecordsBalanceDSC[indexRecordCount].filePos = filePos;

			// Increment
			indexRecordCount++;

			// Store file pos before next read advances it
			filePos = ftell(accountsFilePtr);

			// Read next record
			readCount = fread(&customer, sizeof(CustomerFile), 1, accountsFilePtr);
		}

		// DEBUG: Index Records
		//for (int counter = 0; counter < indexRecordCount; counter++) {
		//	printf("%s - %d\n", indexRecordsLastName[counter].key.lastName, indexRecordsLastName[counter].filePos);
		//}
		//
		//for (int counter = 0; counter < indexRecordCount; counter++) {
		//	printf("%f - %d\n", indexRecordsBalanceASC[counter].key.accountBalance, indexRecordsLastName[counter].filePos);
		//}
		//
		//for (int counter = 0; counter < indexRecordCount; counter++) {
		//	printf("%f - %d\n", indexRecordsBalanceDSC[counter].key.accountBalance, indexRecordsLastName[counter].filePos);
		//}

		// SORT array of index records in memory
		qsort(indexRecordsLastName, indexRecordCount, sizeof(IndexRecord), compareLastName);
		qsort(indexRecordsBalanceASC, indexRecordCount, sizeof(IndexRecord), compareBalanceASC);
		qsort(indexRecordsBalanceDSC, indexRecordCount, sizeof(IndexRecord), compareBalanceDSC);

		printf("indexHeader Size: %d\nindexRecordsLastName Size: %d\n\n"
			, sizeof(IndexHeader)
			, sizeof(IndexRecord));

		printf("## Creating Index File ##\n");

		long filePosAfterFWrite;
		filePosAfterFWrite = ftell(indexFile);
		printf("Start Pos Before Write: %d\n", filePosAfterFWrite);

		// WRITE INDEX HEADER to file
		writeCount = fwrite(&indexHeader, sizeof(IndexHeader), 1, indexFile);

		filePosAfterFWrite = ftell(indexFile);
		printf("Pos after IndexHeader write: %d\n", filePosAfterFWrite);

		// WRITE IndexRecord indexRecordsLastName to file
		writeCount += fwrite(indexRecordsLastName, sizeof(IndexRecord), indexRecordCount, indexFile);

		filePosAfterFWrite = ftell(indexFile);
		printf("Pos after IndexRecord write1: %d\n", filePosAfterFWrite);

		// WRITE IndexRecord indexRecordsBalanceASC to file
		writeCount += fwrite(indexRecordsBalanceASC, sizeof(IndexRecord), indexRecordCount, indexFile);

		filePosAfterFWrite = ftell(indexFile);
		printf("Pos after IndexRecord write2: %d\n", filePosAfterFWrite);

		// WRITE IndexRecord indexRecordsBalanceDSC to file
		writeCount += fwrite(indexRecordsBalanceDSC, sizeof(IndexRecord), indexRecordCount, indexFile);

		filePosAfterFWrite = ftell(indexFile);
		printf("Pos after IndexRecord write3: %d\n", filePosAfterFWrite);

		// SUMMARY
		printf("%d Records Processed\n\n", writeCount);
		printf("FileSize: %d\n\n", fileSize(indexFile));

		// DEBUG: Index Records
		//for (int counter = 0; counter < indexRecordCount; counter++) {
		//	printf("%s - %d\n", indexRecordsLastName[counter].key.lastName, indexRecordsLastName[counter].filePos);
		//}
		//
		//for (int counter = 0; counter < indexRecordCount; counter++) {
		//	printf("%f - %d\n", indexRecordsBalanceASC[counter].key.accountBalance, indexRecordsLastName[counter].filePos);
		//}
		//
		//for (int counter = 0; counter < indexRecordCount; counter++) {
		//	printf("%f - %d\n", indexRecordsBalanceDSC[counter].key.accountBalance, indexRecordsLastName[counter].filePos);
		//}

		// FREE memory
		free(indexRecordsLastName);
		free(indexRecordsBalanceASC);
		free(indexRecordsBalanceDSC);

		// CLOSE file
		fclose(accountsFilePtr);
		fclose(indexFile);
	}
}

// Calculates the size of a file
long fileSize(FILE *input)
{
	long orgPos;
	long startPos;
	long endPos;

	orgPos = ftell(input);          // save orig file position
	rewind(input);
	startPos = ftell(input);
	fseek(input, 0, SEEK_END);
	endPos = ftell(input);
	fseek(input, orgPos, SEEK_SET); // restore orig position

	return(endPos - startPos);
}

// Compare last name
int compareLastName(const void *left, const void *right) {

	IndexRecord *pLeft = (IndexRecord *)left;
	IndexRecord *pRight = (IndexRecord *)right;

	return strcmp(pLeft->key.lastName, pRight->key.lastName);
}

// Compare balance ASC
int compareBalanceASC(const void *left, const void *right) {

	int returnValue = 0;

	IndexRecord *pLeft = (IndexRecord *)left;
	IndexRecord *pRight = (IndexRecord *)right;

	if (pLeft->key.accountBalance > pRight->key.accountBalance) {
		returnValue = 1;
	}
	else if (pLeft->key.accountBalance < pRight->key.accountBalance) {
		returnValue = -1;
	}

	return returnValue;
}

// Compare balance DSC
int compareBalanceDSC(const void *left, const void *right) {

	int returnValue = 0;

	IndexRecord *pLeft = (IndexRecord *)left;
	IndexRecord *pRight = (IndexRecord *)right;

	if (pLeft->key.accountBalance > pRight->key.accountBalance) {
		returnValue = 1;
	}
	else if (pLeft->key.accountBalance < pRight->key.accountBalance) {
		returnValue = -1;
	}

	return returnValue * -1;
}