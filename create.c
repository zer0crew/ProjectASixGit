// COMP 2717 Assignment Five - create.c
// Author: H Lorenzana

// Preprocessor Directives
#define _CRT_SECURE_NO_WARNINGS
#define CUST_ARRAY_SIZE 20
#define STR_BUFFER 100
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct customerFile {
	int accountNumber;
	char firstName[CUST_ARRAY_SIZE];
	char lastName[CUST_ARRAY_SIZE];
	double accountBalance;
	double lastPayment;
} CustomerFile;

// Function prototypes
CustomerFile parse(char *stringInput);
void writeToFile(CustomerFile customers[], char *destinationFile);
int fileExists(char *filename);

int main(int argc, char *argv[] ) {

	char *sourceFile = argv[1];
	char *destinationFile = argv[2];

	if (argc != 3) {
		printf("Not enough parameters.\nEx: create.exe source destination.\n\n");
		exit(EXIT_FAILURE);
	}

	// Variables
	FILE *custFilePtr;
	char stringInput[STR_BUFFER] = "";
	CustomerFile customers[CUST_ARRAY_SIZE];
	CustomerFile customer;
	
	// Open file
	if ((custFilePtr = fopen(sourceFile, "r")) == NULL) {
		printf("File could not be openned.\n");
		exit(EXIT_FAILURE);
	}
	else {
		fscanf(custFilePtr, "%s", stringInput);

		int index = 0;
		while (!feof(custFilePtr)) {
			customer = parse(stringInput);
			customers[index].accountNumber = customer.accountNumber;
			strcpy(customers[index].firstName, customer.firstName);
			strcpy(customers[index].lastName, customer.lastName);
			customers[index].accountBalance = customer.accountBalance;
			customers[index].lastPayment = customer.lastPayment;
			index++;
			fscanf(custFilePtr, "%s", stringInput);
		}

		// Write array to BIN
		writeToFile(customers, destinationFile);
		
		// Close file
		fclose(custFilePtr);
	}

} // end main()

// Parses string and populates a CustomerFile
CustomerFile parse(char *stringInput) {
	
	char *stringToken;
	CustomerFile customer = { 0, "", "", 0.0, 0.0 };

	stringToken = strtok(stringInput, ",");
	customer.accountNumber = atoi(stringToken);
	stringToken = strtok(NULL, ",");
	strcpy(customer.firstName, stringToken);
	stringToken = strtok(NULL, ",");
	strcpy(customer.lastName, stringToken);
	stringToken = strtok(NULL, ",");
	customer.accountBalance = atol(stringToken);
	stringToken = strtok(NULL, ",");
	customer.lastPayment = atol(stringToken);

	return customer;
}

// Writes CustomerFile to binary file accounts.dat
void writeToFile(CustomerFile customers[], char *destinationFile) {
	FILE *accountsFilePtr;
	char response;

	// Check if file exist
	if (fileExists(destinationFile)) {
		// File exists
		printf("File exists. Press 'Y' to overwrite: ");
		scanf("%c", &response);
		if (response == 'y' || response == 'Y') {
			printf("\nOK. Overwriting file.\n");
		}
		else {
			printf("\nSkipping write.\n\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// Write CustomerFile array to binary file accounts.bin
	if ((accountsFilePtr = fopen("accounts.dat", "wb")) == NULL) {
		printf("Unable to open file.\n");
	}
	else {
		for (int index = 0; index < CUST_ARRAY_SIZE; index++) {
			fwrite(&customers[index], sizeof(CustomerFile), 1, accountsFilePtr);
		}

		printf("Write to file complete.\n\n");

		fclose(accountsFilePtr);
	}
	
	// Test Array
	/*for (int index = 0; index < CUST_ARRAY_SIZE; index++) {
		printf("Acct: %d\nFName: %s\nLName: %s\nBalance: %.2f\nLastPay: %.2f"
		, customers[index].accountNumber
		, customers[index].firstName
		, customers[index].lastName
		, customers[index].accountBalance
		, customers[index].lastPayment);
		printf("\n\n");
	}*/
}

// Checks if a filename exists
int fileExists(char *filename) {
	FILE *fileToCheck;
	if (fileToCheck = fopen(filename, "r")) {
		fclose(fileToCheck);
		return 1;
	}
	return 0;
}