#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int mv(char *source, char *dest){

	int sourceLen = strlen(source);

	int idx = sourceLen - 1;
	while(idx >= 0 && source[idx] != '/') --idx;

	++idx;
	char filename[PATH_MAX];
	int filenameLen = 0;

	for(; idx < sourceLen; ++idx)
		filename[filenameLen++] = source[idx];

	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));

	char auxsource[PATH_MAX];
	memset(auxsource, 0, PATH_MAX);

	// if(source[0] != '/') { // is absolute path
	if(source[0] == '.' && source[1] == '/'){ // cale relativa
		strcpy(auxsource, cwd);
		strcat(auxsource, "/");
		strcat(auxsource, filename);
	} else  // cale absoluta
	{
		strcpy(auxsource, source);
	}
	

	char auxdest[PATH_MAX];
	memset(auxdest, 0, PATH_MAX);

	/*if(strcmp(dest, ".") == 0){
		strcpy(auxdest, cwd);
		strcat(auxdest, "/");
		strcat(auxdest, filename);
	}
	else{

		strcpy(auxdest, cwd);
		strcat(auxdest, "/");
		strcat(auxdest, dest);
		strcat(auxdest, "/");
		strcat(auxdest, filename);
	}*/

	if(dest[0] == '.' && dest[1] == '/') { // cale relativa

		strcpy(auxdest, cwd);
		strcat(auxdest, dest + 1);
	} else 
	{ // cale absoluta
		strcpy(auxdest, dest);
	}

	int readFile_ID = open(auxsource, O_RDONLY);
	if(readFile_ID == -1){

		// perror(strcat("Eroare la deschiderea fisierului", auxsource));
		dprintf(2, "Eroare la deschiderea fisierului %s.\n", auxsource);

		return -1;
	}

	int writeFile_ID = open(auxdest, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

	if(writeFile_ID == -1){
		// perror(strcat("Eroare la deschiderea fisierului", auxdest));

		dprintf(2, "Eroare la deschiderea fisierului %s.\n", auxdest);

		return -1;
	}

	char *buffer = (char *)calloc(100, sizeof(char));
	int readBytes = 0;
	while((readBytes = read(readFile_ID, buffer, 100)) > 0)
		write(writeFile_ID, buffer, readBytes);

	if(close(readFile_ID) == -1){

		// perror(strcat("Eroare la inchiderea fisierului", auxsource));

		dprintf(2, "Eroare la inchiderea fisierului %s.\n", auxsource);

		return -1;
	}

	if(close(writeFile_ID) == -1){

		// perror(strcat("Eroare la inchiderea fisierului", auxdest));

		dprintf(2, "Eroare la inchiderea fisierului %s.\n", auxdest);

		return -1;
	}

	int successRem = remove(auxsource);
	if(successRem){

		perror("Could not remove source");
		return -1;
	}

	return 0;
}

int main(int argc, char **argv){

	// mv(argv[1], argv[2]);

	return mv(argv[1], argv[2]);
}
