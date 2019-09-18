#include "common.h"
#include <syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>


void copyDir(const char* sourceDir, const char* destDir);
void copyFile(const char* sourceFilePath, const char* destFilePath);
/* make sure to use syserror() when a system call fails. see common.h */

void
usage()
{
	fprintf(stderr, "Usage: cpr srcdir dstdir\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	if (argc != 3) {
		usage();
	}

	copyDir(argv[1], argv[2]);	

	return 0;
}


void copyDir(const char* sourceDir, const char* destDir){

	DIR* pDir = opendir(sourceDir);
	
	struct dirent* dir;
	struct stat buf;

	int ret = stat(sourceDir, &buf);
	if(ret == -1){
		syserror(stat, sourceDir);
	}
	// Set to IWUSR for now to copy sub files and directories into it 
	ret = mkdir(destDir, S_IRWXU);
	mode_t sourceDirMode = buf.st_mode;

	if(ret == -1){
		syserror(mkdir, destDir);
	}
	
	if(pDir != NULL){
		// Read all directory entries (dirent)
		while( (dir = readdir(pDir)) != NULL){

			if(strcmp(dir->d_name, ".") == 0||strcmp(dir->d_name, "..") == 0){
				continue;
			}

			char* subSource = (char*)malloc(sizeof(char)*1000);
			char* subDest = (char*)malloc(sizeof(char)*1000);

			strcpy(subSource, sourceDir);
			strcat(subSource, "/");
			strcat(subSource, dir->d_name);

			strcpy(subDest, destDir);
			strcat(subDest, "/");
			strcat(subDest, dir->d_name);

			// 1. Get file information using stat
			int ret = stat(subSource, &buf);

			if(S_ISDIR(buf.st_mode)){ // Directory
				// 2. Recursively call this function with the newly created dir
				copyDir(subSource, subDest);
			}
			else if(S_ISREG(buf.st_mode)){ // Regular file

				// 2. Create a file with the same name inside
				// destination folder w/ wrtie only permission
				ret = creat(subDest, S_IRWXU);
				if(ret < 0){
					syserror(creat, subDest);
				}

				// 3. Copy the file
				copyFile(subSource, subDest);

				// 4. Use chmod to set the same permission
				ret = chmod(subDest, buf.st_mode);
				if(ret < 0){
					syserror(chmod, subDest);
				}

			}

			free(subSource);
			free(subDest);
		}

	}
	
	// Chnage back to permission level of source dir
	chmod(destDir, sourceDirMode);
	
	closedir(pDir);
	
}


/* Copy file 1 to file 2*/ 
void copyFile(const char* sourceFilePath, const char* destFilePath){

	int fd1, fd2;
	ssize_t bytes;

	fd1 = open(sourceFilePath, O_RDONLY);
	fd2 = creat(destFilePath, S_IWUSR);

	char buffer[100];

	while( (bytes = read(fd1, buffer, 100)) ){
		if(bytes == -1) syserror(read, sourceFilePath);
		write(fd2, buffer, bytes);
	}

	close(fd1);
	close(fd2);

}


