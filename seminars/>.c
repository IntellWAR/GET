#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

int main (int argc, char** argv)
{
	int fd_out = open(argv[1], O_WRONLY|O_CREAT, 0666);
	dup2(fd_out, STDOUT_FILENO);
	//fork();
	execvp (argv[2], argv + 2);
}