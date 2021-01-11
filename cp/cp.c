#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


int cp(char *inputFile, char *outputFile){ // returneza eroare

	int readFile_ID = open(inputFile, O_RDONLY);
	if(readFile_ID == -1){

		perror(strcat("Eroare la deschiderea fisierului", inputFile));
		return -1;
	}

	int writeFile_ID = open(outputFile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

	if(writeFile_ID == -1){

		perror(strcat("Eroare la deschiderea fisierului", outputFile));
		return -1;
	}

	char *buffer = (char *)calloc(100, sizeof(char));
	int readBytes = 0;
	while((readBytes = read(readFile_ID, buffer, 100)) > 0)
		write(writeFile_ID, buffer, readBytes);

	if(close(readFile_ID) == -1){

		perror(strcat("Eroare la inchiderea fisierului", inputFile));
		return -1;
	}

	if(close(writeFile_ID) == -1){

		perror(strcat("Eroare la inchiderea fisierului", outputFile));
		return -1;
	}

	return 0;
}


int main(int argc, char **argv){

	return cp(argv[1], argv[2]);
}
