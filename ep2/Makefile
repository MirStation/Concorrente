# Makefile
CC = gcc
FLAGS = -g -Wall -ansi -pedantic -O3
LIBS = -lpthread -D_POSIX_C_SOURCE=199309L -lgmp -lrt -lm
TARGET = ep2
OBJECTS = ep2.o
BACKUPDAY = `date +%y%m%d_%a_%H%M%S`

all : ${TARGET}

${TARGET} : ${OBJECTS}
	${CC} -o $@ $? ${FLAGS} ${LIBS}

%.o : %.c
	${CC} -c -o $@ $? ${FLAGS} ${LIBS} -I.

clean :
	rm -f *~ ${OBJECTS} ${TARGET}

tar:
	tar cvfz ${TARGET}-Igor-Antonio.tgz Makefile *.c *.h *.txt *.pdf

backup:
	tar cvfz ${TARGET}-Igor-Antonio-Backup-${BACKUPDAY}.tgz Makefile *.c *.h *.txt

