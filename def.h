/* this is a definition file for 
 * prototypes */
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
static char *my_argv[100], *my_envp[100];
static char *search_path[10];
static char current_dir[1024];

void handle_signal(int signo); 
void shell_prompt(void);
void fill_argv(char *tmp_argv); 
void copy_envp(char **envp); 
int attach_path(char *cmd); 
void get_path_string(char **tmp_envp, char *bin_path);
void insert_path_str_to_search(char *path_str); 
void change_dir(void); 
int check_builtin_command(void); 
void call_execve(char *cmd); 
void free_argv(); 

