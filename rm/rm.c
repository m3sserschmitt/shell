#include <stdio.h>

int rm(char *file){

	int status = remove(file);

	if (status){

		perror("Error: Couldn't remove file");
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[]){

	return rm(argv[1]);
}
