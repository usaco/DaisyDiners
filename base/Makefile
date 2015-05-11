CC=gcc
CFLAGS=-g -Wall
LIBS=-lm
GL=-lglut -lGL -lGLU
TERM=

all: driver-null driver-term driver-opengl

driver-visual.h: driver-base.h

driver-null.o: driver-null.c driver-base.h driver-visual.h
driver-term.o: driver-term.c driver-base.h driver-visual.h
driver-opengl.o: driver-opengl.c driver-base.h driver-visual.h

driver.o: driver.c driver-base.h driver-visual.h

driver-null: driver.o driver-null.o
	${CC} ${CFLAGS} -o $@ $^ ${LIBS}

driver-term: driver.o driver-term.o
	${CC} ${CFLAGS} -o $@ $^ ${LIBS} ${TERM}

driver-opengl: driver.o driver-opengl.o
	${CC} ${CFLAGS} -o $@ $^ ${LIBS} ${GL}

clean:
	rm -f *.o

fresh: clean all

