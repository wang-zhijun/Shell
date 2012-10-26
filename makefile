CC = gcc
CFlags = -g
mybash : shell.o
	$(CC) $(CFLAGS) -o shell shell.o

# -c says not to run the linker
shell.o : shell.c
	$(CC) $(CFLAGS) -c shell.c
