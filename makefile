CC=gcc
LINKS= -lgmp
FILES= vm/*.c tacl/*.c object/*.c lib/*.c files/*.c export/*.c
compile:
	python3 lib/libnames.py
	$(CC) tach.c $(FILES) -I./ $(LINKS) -o tachvm
	rm lib/tach_conv.c
	rm lib/tach_lib.h