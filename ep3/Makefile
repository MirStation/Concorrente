# Makefile
CC = gcc
FLAGS = -g -Wall -ansi -pedantic -O3
LIBS = -lpthread -D_POSIX_C_SOURCE=199309L
TARGET = ep3
OBJECTS = ep3.o pqueue.o monitor.o uniformDistribution.o
BACKUPDAY = `date +%y%m%d_%a_%H%M%S`

all : ${TARGET}

${TARGET} : ${OBJECTS}
	${CC} -o $@ $? ${FLAGS} ${LIBS}

%.o : %.c
	${CC} $? -c ${FLAGS} ${LIBS} -I.

clean :
	rm -f *~ ${OBJECTS} ${TARGET}

tar:
	tar cvfz ${TARGET}-Igor-Antonio.tgz Makefile *.c *.h *.txt *.pdf

backup:
	tar cvfz ${TARGET}-Igor-Antonio-Backup-${BACKUPDAY}.tgz Makefile *.c *.h *.txt
