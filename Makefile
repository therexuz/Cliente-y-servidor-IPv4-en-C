CC	= gcc
CFLAGS	= -O2 -Wall

all: client server

client: client.o
server: server.o

clean:
	rm -f client server \
              client.o server.o

.PHONY: all clean

