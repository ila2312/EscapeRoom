CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99

all: server

server: server.c GameData.h GameInstance.h
	$(CC) $(CFLAGS) $^ -o server

client: client.c
	$(CC) $(CFLAGS) client.c -o client

# pulizia dei file della compilazione (eseguito con ‘make clean’ da terminale)
clean:
	rm *.o client server