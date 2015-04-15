// COMP 2717 Assignment Six - displayFunc.c
// Author: H Lorenzana

#include "displayFunc.h"

// Reads the accounts.dat file and displays in NATURAL ORDER
void readFile(char *dataFile) {
	FILE *accountsFilePtr;
	CustomerFile customer = { 0, "", "", 0.0, 0.0 };

	if ((accountsFilePtr = fopen(dataFile, "rb")) == NULL) {
		printf("\nUnable to open file.\n\n");
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

			printf("%-7d%-15s%-15s%15.2f%10.2f\n"
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

	printf("\nChoose from the following options:\n\n");
	printf("1) LastName ASC\n2) AccountBalance DESC\n3) AccountBalance ASC\n4) Natural Order\n5) Exit\n\n");
}

// Reads the accounts.dat file and displays in INDEX ORDER
void readUsingIndexFile(char *dataFile, int selection)  {

	FILE *accountsFilePtr;
	FILE *indexFile;
	int readCount;
	int recCount;

	CustomerFile customer = { 0, "", "", 0.0, 0.0 };
	IndexRecord indexCecord = { 0 };

	IndexHeader indexHeader;
	IndexRecord *indexRecordsLastName;
	IndexRecord *indexRecordsBalanceASC;
	IndexRecord *indexRecordsBalanceDSC;

	// Open data file for reading
	if ((accountsFilePtr = fopen(dataFile, "rb")) == NULL) {
		printf("\nERROR - can not open data file.\n\n");
		exit(EXIT_FAILURE);
	}

	// Open index file for reading
	if ((indexFile = fopen("accounts.cdx", "rb")) == NULL) {
		printf("\nERROR - can not open index file.\n\n");
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
		readCount = fread(&indexCecord, sizeof(IndexRecord), 1, indexFile);
		indexRecordsLastName[counter] = indexCecord;
	}

	// DEBUG
	//for (int counter = 0; counter < recCount; counter++) {
	//	printf("%s\n%d\n", indexRecordsLastName[counter].key.lastName, indexRecordsLastName[counter].filePos);
	//}

	// Populate indexRecordsBalanceASC from indexFile
	for (int counter = 0; counter < recCount; counter++) {
		readCount = fread(&indexCecord, sizeof(IndexRecord), 1, indexFile);
		indexRecordsBalanceASC[counter] = indexCecord;
	}

	// DEBUG
	//for (int counter = 0; counter < recCount; counter++) {
	//	printf("%.2f\n%d\n", indexRecordsBalanceASC[counter].key.accountBalance, indexRecordsBalanceASC[counter].filePos);
	//}

	// Populate indexRecordsBalanceDSC from indexFile
	for (int counter = 0; counter < recCount; counter++) {
		readCount = fread(&indexCecord, sizeof(IndexRecord), 1, indexFile);
		indexRecordsBalanceDSC[counter] = indexCecord;
	}

	// DEBUG
	//for (int counter = 0; counter < recCount; counter++) {
	//	printf("%.2f\n%d\n", indexRecordsBalanceDSC[counter].key.accountBalance, indexRecordsBalanceDSC[counter].filePos);
	//}

	printf("\n%-7s%-15s%-15s%-15s%-10s\n%s\n"
		, "Acct"
		, "Firstname"
		, "Lastname"
		, "Balance"
		, "Last Payment"
		, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");

	// Read first index record
	if (selection == 1) {
		// LastName ASC
		for (int counter = 0; counter < recCount; counter++) {
			fseek(accountsFilePtr, indexRecordsLastName[counter].filePos, SEEK_SET);
			fread(&customer, sizeof(CustomerFile), 1, accountsFilePtr);
			printf("%-7d%-15s%-15s%15.2f%10.2f\n"
				, customer.accountNumber
				, customer.firstName
				, customer.lastName
				, customer.accountBalance
				, customer.lastPayment);
		}
	}
	else if (selection == 2) {
		// AcctBalance DESC
		for (int counter = 0; counter < recCount; counter++) {
			fseek(accountsFilePtr, indexRecordsBalanceDSC[counter].filePos, SEEK_SET);
			fread(&customer, sizeof(CustomerFile), 1, accountsFilePtr);
			printf("%-7d%-15s%-15s%15.2f%10.2f\n"
				, customer.accountNumber
				, customer.firstName
				, customer.lastName
				, customer.accountBalance
				, customer.lastPayment);
		}
	}
	else {
		// AcctBalance ASC
		for (int counter = 0; counter < recCount; counter++) {
			fseek(accountsFilePtr, indexRecordsBalanceASC[counter].filePos, SEEK_SET);
			fread(&customer, sizeof(CustomerFile), 1, accountsFilePtr);
			printf("%-7d%-15s%-15s%15.2f%10.2f\n"
				, customer.accountNumber
				, customer.firstName
				, customer.lastName
				, customer.accountBalance
				, customer.lastPayment);
		}
	}
}