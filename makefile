CC = gcc
CFlags = -g
mybash : listing1.o
	$(CC) $(CFLAGS) -o mybash listing1.o

# -c says not to run the linker
listing1.o : listing1.c
	$(CC) $(CFLAGS) -c listing1.c
