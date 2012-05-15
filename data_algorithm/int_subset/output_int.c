#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		exit(1);
	}
	char *file = argv[1];
	int int_num = atoi(argv[2]);
	
	int fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1) {
		exit(2);
	}
	srandom((unsigned int)time(NULL));
	int i = 0;
	char sint[sizeof("4294967295") + 1] = {0};// max int number + \n + \0
	for (i = 0; i < int_num; i++) {
		snprintf(sint, sizeof(sint), "%ld\n", random());
		if (write(fd, sint, strlen(sint)) == -1) {
			exit(3);
		}
	}

	if (close(fd) == -1)
		exit(4);
	
	return 0;
}
