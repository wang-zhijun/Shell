/* program from linuxgazette.net */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef void (*sighandler_t)(int);

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
