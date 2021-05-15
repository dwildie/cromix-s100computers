% Build idecf.068
%

% S100 IDE/CF hard disk controller board

rawconsole.o68:    rawconsole.c
	cc -s rawconsole

idecf.o68:      idecf.c idecf.h
	cc -s idecf

systimer.o68:   systimer.c
	cc -s systimer

stty.o68:       stty.c sio.h
	cc -s stty
        
sio.o68:        sio.c sio.h
	cc -s sio

s100lib.o68:    rawconsole.o68
s100lib.o68:    idecf.o68
s100lib.o68:    systimer.o68
s100lib.o68:    stty.o68
s100lib.o68:    sio.o68
	ty rawconsole.o68 > s100lib.o68
	ty idecf.o68 >> s100lib.o68
	ty systimer.o68 >> s100lib.o68
	ty stty.o68 >> s100lib.o68
	ty sio.o68 >> s100lib.o68
	echo "s100lib.o68 has been built"        
