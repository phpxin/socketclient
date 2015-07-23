all: chatserver

CC = gcc
CFLAGS = -g -Wall -ansi

INCLUDE = -I./header/

OUTPUTF = ./output/

SRCF = ./src/

BINF = ./bin/

chatserver: ${OUTPUTF}main.o ${OUTPUTF}strutil.o ${OUTPUTF}action.o ${OUTPUTF}logicutil.o
	${CC} ${CFLAGS} -o ${BINF}chatclient ${OUTPUTF}main.o ${OUTPUTF}strutil.o ${OUTPUTF}action.o ${OUTPUTF}logicutil.o

#create all c to o
${OUTPUTF}%.o: ${SRCF}%.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

clean:
	rm -rf ${OUTPUTF}*.o ${BINF}chatclient
	
