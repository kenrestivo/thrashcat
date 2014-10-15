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



test: OUTFILE=/mnt/sdcard/to-other/why-no-archive/foo.ogg
test: ${TARGET} 
	-(cat /mnt/sdcard/to-other/why-no-archive/2*ogg | ./decoder_example > ${OUTFILE})
	ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}


test1: OUTFILE=/mnt/sdcard/to-other/borkenoggs/foo.ogg
test1: ${TARGET}
	-(cat /mnt/sdcard/to-other/borkenoggs/2*ogg | ./decoder_example > ${OUTFILE})
	ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}

borkinfo::
	(for i in /mnt/sdcard/to-other/borkenoggs/2*ogg; do echo "----$$i"; oggz-info $$i; ogginfo $$i; done)
