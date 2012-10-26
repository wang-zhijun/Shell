#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *buf = malloc(1024);
	chdir("/etc");
	printf("%s\n",getcwd(buf, 1024));
	return 0;
}


