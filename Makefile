#makefile BASED ON wheelhouse shells

TARGET= thrashcat
CC= gcc
CFLAGS= -Wall   -g 
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


test0: OUTFILE=/mnt/sdcard/to-other/why-no-archive/foo.ogg
test0: ${TARGET} 
	echo "TEST"
	-(cat /mnt/sdcard/to-other/why-no-archive/2*ogg | ./${TARGET} > ${OUTFILE})
	ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}


test1: OUTFILE=/mnt/sdcard/to-other/borkenoggs/foo.ogg
test1: ${TARGET}
	echo "TEST 1"
	-(cat /mnt/sdcard/to-other/borkenoggs/2*ogg | ./${TARGET} > ${OUTFILE})
	ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}

borkinfo::
	(for i in /mnt/sdcard/to-other/borkenoggs/2*ogg; do echo "----$$i"; oggz-info $$i; ogginfo $$i; done)

test2: OUTFILE=/mnt/sdcard/tmp//2014-10-05-foobarbut.ogg
test2: ${TARGET}
	echo "TEST 2"
	-(cat /mnt/sdcard/to-other/borkenoggs/2014-10-05-15_53_55-foobarbut.ogg | ./${TARGET} > ${OUTFILE})
	-ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}

test3: OUTFILE=/mnt/sdcard/tmp//2014-10-11-test_in_the_dark_illegal_chars.ogg
test3: ${TARGET}
	echo "TEST 3"
	-(cat /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_53_53-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_54_59-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_55_39-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_57_18-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_57_57-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_58_09-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_04_59-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_09_12-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_12_56-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_14_06-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_03-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_09-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_15-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_21-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_27-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_18_03-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_18_51-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_23_53-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_25_30-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_27_28-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_32_05-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_33_43-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_46_44-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_50_49-test_in_the_dark_illegal_chars.ogg  | ./${TARGET} > ${OUTFILE})
	ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}


test3-pipe: OUTFILE=/mnt/sdcard/tmp//2014-10-11-test_in_the_dark_illegal_chars.ogg
test3-pipe: ${TARGET}
	echo "TEST 3 pipe"
	-(./${TARGET} < /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_53_53-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_54_59-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_55_39-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_57_18-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_57_57-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_58_09-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_04_59-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_09_12-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_12_56-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_14_06-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_03-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_09-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_15-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_21-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_27-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_18_03-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_18_51-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_23_53-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_25_30-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_27_28-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_32_05-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_33_43-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_46_44-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_50_49-test_in_the_dark_illegal_chars.ogg  > ${OUTFILE})
	ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}


test3-gdb: OUTFILE=/mnt/sdcard/tmp//2014-10-11-test_in_the_dark_illegal_chars.ogg
test3-gdb: ${TARGET}
	-(gdb ${TARGET} < /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_53_53-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_54_59-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_55_39-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_57_18-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_57_57-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-22_58_09-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_04_59-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_09_12-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_12_56-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_14_06-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_03-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_09-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_15-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_21-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_15_27-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_18_03-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_18_51-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_23_53-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_25_30-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_27_28-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_32_05-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_33_43-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_46_44-test_in_the_dark_illegal_chars.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-11-23_50_49-test_in_the_dark_illegal_chars.ogg  > ${OUTFILE})




test4: OUTFILE=/mnt/sdcard/tmp//2014-09-28-testdark.ogg
test4: ${TARGET}
	echo "TEST 4"
	-(cat /mnt/sdcard/to-other/borkenoggs/2014-09-28-01_19_30-testdark.ogg /mnt/sdcard/to-other/borkenoggs/2014-09-28-01_20_24-testdark.ogg /mnt/sdcard/to-other/borkenoggs/2014-09-28-01_20_29-testdark.ogg | ./${TARGET} > ${OUTFILE})
	ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}

test5: OUTFILE=/mnt/sdcard/tmp//2014-10-05-testdarkh.ogg
test5: ${TARGET}
	echo "TEST 5"
	-(cat /mnt/sdcard/to-other/borkenoggs/2014-10-05-15_47_50-testdarkh.ogg /mnt/sdcard/to-other/borkenoggs/2014-10-05-15_47_56-testdarkh.ogg | ./${TARGET} > ${OUTFILE})
	ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}

test6: OUTFILE=/mnt/sdcard/tmp/2014-10-15-esoundc.ogg
test6: ${TARGET}
	echo "TEST 6"
	-(cat /mnt/sdcard/to-other/moreborkens/2014-10-15-11_09_34.ogg /mnt/sdcard/to-other/moreborkens/2014-10-15-11_32_06.ogg /mnt/sdcard/to-other/moreborkens/2014-10-15-19_33_12-.ogg | ./${TARGET} > ${OUTFILE})
	ls -la ${OUTFILE}
	-oggz-validate ${OUTFILE}
	-oggz-info ${OUTFILE}
	-ogginfo ${OUTFILE}


test: test0 test1 test2 test3 test3-pipe test4 test5 test6
