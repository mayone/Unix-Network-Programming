CC = gcc
CFLAGS =  -I.
LDFLAGS = -L.

all: client server

client: client.c
	$(CC) $(CFLAGS) client.c -o $@
server: server.c
	$(CC) $(CFLAGS) server.c -o $@

clean:
	rm -rf *~ *.o server client
