// COMP 2717 Assignment Six - index.c
// Author: H Lorenzana

#include "indexFunc.h"

int main(int argc, char *argv[]) {

	char *dataFile = argv[1];

	if (argc != 2) {
		printf("Incorrect parameters.\nEx: index.exe datafile.\n\n");
		exit(EXIT_FAILURE);
	}

	// Read file
	indexDataFile(dataFile);

} // end main()
