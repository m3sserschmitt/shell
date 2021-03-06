#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>


int ls(char *input[], int argc){

	char cwd[PATH_MAX]; // PATH_MAX is a constant that defines the maximum length of a path in this OS
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		perror("getcwd() error");
		return -1;
	}

	DIR * dp = opendir(cwd);
	if(!dp){
		perror("Error: Couldn't open the current directory for scanning");
		return -1;
	}

	if(argc == 1){

		struct dirent *ep;
		
		while((ep = readdir(dp)))
			if(strncmp(ep->d_name, ".", 1)) // check for hidden files
				printf("%s\n", ep->d_name);
	}
	else{

		struct dirent *ep;

		while((ep = readdir(dp)))
			for (int i = 1; i < argc; ++i)
				if(strcmp(ep->d_name, input[i]) == 0){

					printf("%s\n", ep->d_name);
					break;
				}

	}

	return 0;
}


int main(int argc, char **argv){

	return ls(argv, argc);
}
