# Makefile
CC = gcc
FLAGS = -g -W -Wall -ansi -pedantic -O3
LIBS = -lpthread -D_POSIX_C_SOURCE=199309L
TARGET = main
OBJECTS = main.o global.o leitura.o init_pilotos.o sort.o init_pista.o
BACKUPDAY = `date +%y%m%d_%a_%H%M%S`

all : ${TARGET}

${TARGET} : ${OBJECTS}
	${CC} -o $@ $? ${FLAGS} ${LIBS}

%.o : %.c
	${CC} -c -o $@ $? ${FLAGS} ${LIBS} -I.

clean :
	rm -f *~ ${OBJECTS} ${TARGET}

tar:
	tar cvfz ep1-Igor-Antonio.tgz Makefile *.c *.h *.txt *.pdf

backup:
	tar cvfz ep1-Igor-Antonio-Backup-${BACKUPDAY}.tgz Makefile *.c *.h *.txt

