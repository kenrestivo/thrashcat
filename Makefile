#makefile BASED ON wheelhouse shells

TARGET= thrashcat
CC= gcc
CFLAGS= -Wall   # -g
LDFLAGS= -logg -lvorbis
OBJS= ${TARGET}.o 


#
#

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o  ${TARGET} ${OBJS}

clean::
	rm -f ${TARGET} ${OBJS} *core


install: ${TARGET}
	sudo cp ${TARGET} /usr/local/bin/
	sudo chown root:root /usr/local/bin/${TARGET}


test: OUTFILE=/mnt/sdcard/to-other/why-no-archive/foo.ogg
test: ${TARGET} 
	-(cat /mnt/sdcard/to-other/why-no-archive/2*ogg | ./thrashcat > ${OUTFILE})
	ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}


test1: OUTFILE=/mnt/sdcard/to-other/borkenoggs/foo.ogg
test1: ${TARGET}
	-(cat /mnt/sdcard/to-other/borkenoggs/2*ogg | ./thrashcat > ${OUTFILE})
	ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}

borkinfo::
	(for i in /mnt/sdcard/to-other/borkenoggs/2*ogg; do echo "----$$i"; oggz-info $$i; ogginfo $$i; done)
