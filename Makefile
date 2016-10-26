all: chatclient

CC = gcc
CFLAGS = -g -Wall -ansi -D_REENTRANT

EVENTDIR = /usr/local/libevent-2.0.22/

INCLUDE = -I./header/ -I${EVENTDIR}include/

OUTPUTF = ./output/

SRCF = ./src/

BINF = ./bin/

CLIB = -L${EVENTDIR}lib -lpthread  -levent

chatclient: ${OUTPUTF}main.o ${OUTPUTF}strutil.o ${OUTPUTF}action.o ${OUTPUTF}logicutil.o
	${CC} ${CFLAGS} -o ${BINF}chatclient ${OUTPUTF}main.o ${OUTPUTF}strutil.o ${OUTPUTF}action.o ${OUTPUTF}logicutil.o ${CLIB}

#create all c to o
${OUTPUTF}%.o: ${SRCF}%.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

clean:
	rm -rf ${OUTPUTF}*.o ${BINF}chatclient
	
