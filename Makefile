#makefile BASED ON wheelhouse shells

TARGET= decoder_example
CC= gcc
CFLAGS= -g -Wall  
LDFLAGS= -logg -lvorbis
OBJS= ${TARGET}.o 

#
#

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o  ${TARGET} ${OBJS}

clean::
	rm -f ${TARGET} ${OBJS} *core



