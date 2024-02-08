CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99

all: server client

server: server_prova.c
	$(CC) $(CFLAGS) server_prova.c -o server

client: client_prova.c
	$(CC) $(CFLAGS) client_prova.c -o client

# pulizia dei file della compilazione (eseguito con ‘make clean’ da terminale)
clean:
	rm *o client server