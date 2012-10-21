/* program from linuxgazette.net */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

extern int errno;
typedef void (*sighandler_t)(int);
static char *my_argv[100], *my_envp[100];
static char *search_path[10];

void
handle_signal(int signo) {
	printf("[MY_SHELL ] ");
	fflush(stdout);
}

int
main(int argc, char **argv, char **envp) {
	int c;
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);
	printf("\n[MY_SHELL ] ");
	while( (c=getchar()) != EOF) {
		if( c == '\n')
			printf("[MY_SHELL ] ");
	}
	printf("\n"); /* make MY_SHELL and the prompt not in the same line */
	return 0;
}

void fill_argv(char *tmp_argv) {
	char *foo = tmp_argv;
	int index = 0; /* maximum 10 argument */
	char ret[100];
	bezero(ret, 100);
	while(*foo != '\0') {
		if(index == 10)
			break;
		if(*foo == ' ') {
			if(my_argv[index] == NULL)
				my_argv[index] = (char *)malloc(sizeof(char) * strlen(ret) + 1)
			else {
				bzero(my_argv[index], strlen(my_argv[index]));
			}
			strncpy(my_argv[index], ret, strlen(ret));
			strncat(my_argv[index], "\0", 1);
			bzero(ret,100);
			index++;
		}else 
			strncat(ret, 100, 1);
		
		foo++;
	}
	if(ret[0] != '\0') {
		my_argv[index] = (char *)malloc(sizeof(char) *strlen(ret) + 1);
		strncpy(my_argv[index], ret, strlen(ret));
		strncat(my_argv[index], "\0", 1);
	}
}






