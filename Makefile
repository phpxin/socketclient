all: chatserver

CC = gcc
CFLAGS = -g -Wall -ansi -D_REENTRANT

INCLUDE = -I.
CLIB = -lpthread

chatserver: ${OUTPUTF}main.o
	${CC} ${CFLAGS} -o chatclient main.o ${CLIB}

#create all c to o
%.o: %.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

clean:
	rm -rf *.o chatclient
	
