/* program from linuxgazette.net */
/* 'cd' command is supported */
/* Ctrl-d doesn't send a signal, but indicates an EOF on input*/
#include "def.h"
typedef void (*sighandler_t)(int);


void
handle_signal(int signo) {
	putchar('\n');
	shell_prompt();
	fflush(stdout);
}
void 
shell_prompt(void){
    printf("%s :> ", getcwd(current_dir,1024));
}



void 
fill_argv(char *tmp_argv) {
	char *foo = tmp_argv;
	int index = 0; /* maximum 10 argument */
	char ret[100];
	bzero(ret, 100);
	while(*foo != '\0') {
		if(index == 10)
			break;
		if(*foo == ' ') {
			if(my_argv[index] == NULL)
				my_argv[index] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
			else {
				bzero(my_argv[index], strlen(my_argv[index]));
			}
			strncpy(my_argv[index], ret, strlen(ret));
			strncat(my_argv[index], "\0", 1);
			bzero(ret,100);
			index++;
		}else 
			strncat(ret, foo, 1);
		
		foo++;
	}
	/* Prepare the last parameter */
	my_argv[index] = (char *)malloc(sizeof(char) *strlen(ret) + 1);
	strncpy(my_argv[index], ret, strlen(ret));
	strncat(my_argv[index], "\0", 1);

}

void 
copy_envp(char **envp) {
	int index = 0;
	for ( ; envp[index] != NULL; index++) {
		my_envp[index] = (char *)malloc(sizeof(char)*(strlen(envp[index])+1));
		memcpy(my_envp[index], envp[index], strlen(envp[index]));
	}
}

int 
attach_path(char *cmd) {
	char ret[100];
	int index;
	int fd;
	bzero(ret,100);
	for(index = 0; search_path[index] != NULL; index++) {
		strcpy(ret,search_path[index]);
		strncat(ret, cmd, strlen(cmd));
		if((fd = open(ret, O_RDONLY)) > 0) {
			/* Copy the full path to 'cmd' */
			strncpy(cmd, ret, strlen(ret));
			close(fd);
			return 0;
		}
	}
	return 0;
}

/* Copy 'PATH' variable to bin_path after searching */
void 
get_path_string(char **tmp_envp, char *bin_path) {
	int count = 0;
	char *tmp;
	while(1) {
		tmp = strstr(tmp_envp[count], "PATH");
		if(tmp == NULL)
			count++;
		else
			break;
	}
	strncpy(bin_path, tmp, strlen(tmp));
}

void
insert_path_str_to_search(char *path_str) {
	int index = 0;
	char *tmp = path_str;
	char ret[100];

	/* PATH=.....,try to find the '='*/
	while(*tmp != '=')
		tmp++;
	tmp++;

	/* see 122th line first */
	while(*tmp != '\0') {
		if(*tmp == ':') {
			strncat(ret, "/", 1);

			/* Allocating space for copying */
			search_path[index] = (char *)malloc(sizeof(char) * (strlen(ret)+1));
			strncat(search_path[index], ret, strlen(ret));
			strncat(search_path[index], "\0", 1);
			index++;
			/* initialize ret to '\0's*/
			bzero(ret, 100);
		}
		else 
			strncat(ret, tmp, 1);

		tmp++;
	}
}

void 
change_dir(void) {
    if(strcmp("cd",my_argv[0])== 0) {
        if(my_argv[1] == NULL) {
            chdir(getenv("HOME"));
        }   
        else
            if(chdir(my_argv[1]) == -1) 
                printf(" %s: no such directory\n",my_argv[1]);
    }
} 


int 
check_builtin_command(void) {
    if(strcmp("exit",my_argv[0]) == 0)
        exit(EXIT_SUCCESS);
    if(strcmp("cd", my_argv[0]) == 0) {
        change_dir();
        return 1;
    }   
}




void
call_execve(char *cmd) {
	int i;
//	printf("cmd is %s\n", cmd);
    
    check_builtin_command();
   
    if(fork() == 0) {
        i = execve(cmd, my_argv, my_envp);
        printf("errno is %d\n", errno);
        if( i < 0 ) {
            printf("%s: %s\n", cmd, "Command not found");
            exit(1);
        }
	}
	else {
		wait(NULL);
	}
}

void
free_argv() {
	int index;
	for(index = 0; my_argv[index] != NULL; index++) {
		bzero(my_argv[index], strlen(my_argv[index])+1);
		my_argv[index] = NULL;
		free(my_argv[index]);
	}
}


int
main(int argc, char *argv[], char *envp[]) {
	int c;
	int i, fd;
    

	/* Allocate 100 bytes */
	char *tmp = (char *)malloc(sizeof(char) * 100);

	/* Allocate 256 bytes */
	char *path_str = (char *)malloc(sizeof(char) * 256);

	char *cmd = (char *)malloc(sizeof(char) * 100);


	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);
    
    shell_prompt();
    

	/* Copy envp to global variable my_envp 
	 * my_envp is a 100 element array */
	copy_envp(envp);

	/* Find the line with 'PATH' from 'envp'  
	 * and copy the line
	 * to path_str ( maximum 256 bytes) */
	get_path_string(my_envp, path_str);

	/* Dissect path_str, put every part to search_path[] */
	insert_path_str_to_search(path_str);

	if(fork() == 0)  {
		execve("/usr/bin/clear", argv, my_envp);
		exit(1);
	}
	else 
		wait(NULL);

	fflush(stdout);
	while(c != EOF) {
		c = getchar();
		switch(c) {
			case '\n':
				if ( tmp[0] == '\0')
					shell_prompt();
				else {
					/* fill my_argv[] */
					fill_argv(tmp);

					/* cmd has maximum 100 chars*/
					strncpy(cmd, my_argv[0], strlen(my_argv[0]));
					strncat(cmd, "\0", 1);
					/* if just the relative path is given
					 * not the full path  */
					if(index(cmd, '/') == NULL) {
						/* find the cmd in the search_path[] and 
						 * make cmd full path using strncpy()*/
						if(attach_path(cmd) == 0) {
							/* use fork to execute the cmd
							 * and the parent is wait(ing)!*/
							call_execve(cmd);
						}
						else {
							printf("%s: command not found\n", cmd);
						}
					}
					/* if the full path is given */
					else {
						if((fd = open(cmd, O_RDONLY)) > 0) {
							close(fd);
							call_execve(cmd);
						}
						else {
							printf("%s: command not found\n",cmd);
						}
					}
					free_argv();
					shell_prompt();
					bzero(cmd, 100);
				}
				bzero(tmp, 100);
				break;
			default:
				strncat(tmp, &c, 1);
				break;
		}
	}
	free(tmp);
	free(path_str);
	for(i = 0; my_envp[i] != NULL; i++) 
		free(my_envp[i]);
	for(i = 0; i < 10; i++)
		free(search_path[i]);
	printf("\n");
	return 0;
}


