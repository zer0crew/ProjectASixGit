// COMP 2717 Assignment Six - display.c
// Author: H Lorenzana

#include "displayFunc.h"

int main(int argc, char *argv[]) {

	char *dataFile = argv[1];

	if (argc != 2) {
		printf("\nIncorrect parameters.\nEx: index.exe datafile.\n\n");
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