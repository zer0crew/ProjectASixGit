// COMP 2717 Assignment Six - display.c
// Author: H Lorenzana

#define _CRT_SECURE_NO_WARNINGS
#define CUST_ARRAY_SIZE 20
#define STR_BUFFER 100
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define APP_NAME "COMP 2717 A6"

typedef struct customerFile {
	int accountNumber;
	char firstName[CUST_ARRAY_SIZE];
	char lastName[CUST_ARRAY_SIZE];
	double accountBalance;
	double lastPayment;
} CustomerFile;

typedef union
{
	char lastName[30];
	double accountBalance;
} KeyType;

typedef struct
{
	KeyType		key;
	long		filePos;
} IndexRecord;

typedef struct
{
	char appName[20];
	int	recCount;
} IndexHeader;

// Function prototypes
void displayMenu();
void readFile(char *dataFile);
void readUsingIndexFile(char *dataFile, int selection);

int main(int argc, char *argv[]) {

	char *dataFile = argv[1];

	if (argc != 2) {
		printf("Incorrect parameters.\nEx: index.exe datafile.\n\n");
		exit(EXIT_FAILURE);
	}

    // Initialize IndexRecords

    // Menu Loop
	int selection = 0;
	displayMenu();

	while (selection != 5) {
		scanf("%d", &selection);
		switch (selection) {
		case 1: // LastName ASC
			readUsingIndexFile(dataFile, 1);
			break;
		case 2: // AcctBalance DESC
			readUsingIndexFile(dataFile, 2);
			break;
		case 3: // AcctBalance ASC
			readUsingIndexFile(dataFile, 3);
			break;
		case 4: // Natual Order
			readFile(dataFile);
			break;
		default:
			break;
		}
		displayMenu();
	}

} // end main()

// Reads the accounts.dat file and displays in NATURAL ORDER
void readFile(char *dataFile) {
	FILE *accountsFilePtr;
	CustomerFile customer = { 0, "", "", 0.0, 0.0 };

	if ((accountsFilePtr = fopen(dataFile, "rb")) == NULL) {
		printf("Unable to open file.\n\n");
		exit(EXIT_FAILURE);
	}
	else {

		printf("\n%-7s%-15s%-15s%-15s%-10s\n%s\n"
			, "Acct"
			, "Firstname"
			, "Lastname"
			, "Balance"
			, "Last Payment"
			, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");

		while (!feof(accountsFilePtr)) {
			fread(&customer, sizeof(CustomerFile), 1, accountsFilePtr);

			printf("%-7d%-15s%-15s%-15.2f%-10.2f\n"
				, customer.accountNumber
				, customer.firstName
				, customer.lastName
				, customer.accountBalance
				, customer.lastPayment);
		}

		printf("\n");
		fclose(accountsFilePtr);
	}
}

// Displays a menu for the program
void displayMenu() {
	
	printf("Choose from the following options:\n\n");
	printf("1) LastName ASC\n2) AccountBalance DESC\n3) AccountBalance ASC\n4) Natural Order\n5) Exit\n\n");
}

// Reads the accounts.dat file and displays in INDEX ORDER
void readUsingIndexFile(char *dataFile, int selection)  {

	FILE *accountsFilePtr;
	FILE *indexFile;
	int readCount;
	int recCount;

	CustomerFile customer = { 0, "", "", 0.0, 0.0 };
	IndexRecord record = { 0 };

	IndexHeader indexHeader;
	IndexRecord *indexRecordsLastName;
	IndexRecord *indexRecordsBalanceASC;
	IndexRecord *indexRecordsBalanceDSC;

	// Open data file for reading
	if ((accountsFilePtr = fopen(dataFile, "rb")) == NULL) {
		printf("ERROR - can not open data file.\n");
		exit(EXIT_FAILURE);
	}

	// Open index file for reading
	if ((indexFile = fopen("accounts.cdx", "rb")) == NULL) {
		printf("ERROR - can not open index file.\n");
		exit(EXIT_FAILURE);
	}

	// Read HEADER record
	readCount = fread(&indexHeader, sizeof(IndexHeader), 1, indexFile);
	recCount = indexHeader.recCount;

	// Allocate space for IndexRecord
	indexRecordsLastName = (IndexRecord *)calloc(recCount, sizeof(IndexRecord));
	indexRecordsBalanceASC = (IndexRecord *)calloc(recCount, sizeof(IndexRecord));
	indexRecordsBalanceDSC = (IndexRecord *)calloc(recCount, sizeof(IndexRecord));

    // Populate indexRecordsLastName from indexFile
	for (int counter = 0; counter < recCount; counter++) {
		readCount = fread(&record, sizeof(IndexRecord), 1, indexFile);
		indexRecordsLastName[counter] = record;
	}

    // DEBUG
	//for (int counter = 0; counter < recCount; counter++) {
	//	printf("%s\n%d\n", indexRecordsLastName[counter].key.lastName, indexRecordsLastName[counter].filePos);
	//}

	// Populate indexRecordsBalanceASC from indexFile
	for (int counter = 0; counter < recCount; counter++) {
		readCount = fread(&record, sizeof(IndexRecord), 1, indexFile);
		indexRecordsBalanceASC[counter] = record;
	}

    // DEBUG
	//for (int counter = 0; counter < recCount; counter++) {
	//	printf("%.2f\n%d\n", indexRecordsBalanceASC[counter].key.accountBalance, indexRecordsBalanceASC[counter].filePos);
	//}

	// Populate indexRecordsBalanceDSC from indexFile
	for (int counter = 0; counter < recCount; counter++) {
		readCount = fread(&record, sizeof(IndexRecord), 1, indexFile);
		indexRecordsBalanceDSC[counter] = record;
	}

    // DEBUG
	//for (int counter = 0; counter < recCount; counter++) {
	//	printf("%.2f\n%d\n", indexRecordsBalanceDSC[counter].key.accountBalance, indexRecordsBalanceDSC[counter].filePos);
	//}

	// Read first index record
	if (selection == 1) { // LastName ASC
		//printf("\nUnable to implement LastName ASC from compound index file.\n");
		//printf("Only natural order is working on this version.\n\n");
	}
	else if (selection == 2) { // AcctBalance DESC
		printf("\nUnable to implement AcctBalance DESC from compound index file.\n");
		printf("Only natural order is working on this version.\n\n");
	}
	else { // AcctBalance ASC
		printf("\nUnable to implement AcctBalance ASC from compound index file.\n");
		printf("Only natural order is working on this version.\n\n");
	}
}