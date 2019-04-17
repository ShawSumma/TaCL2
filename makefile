CC=clang
LINKS=-lgmp
FILES= vm/*.c tacl/*.c object/*.c lib/*.c files/*.c export/*.c
compile:
	python3 lib/libnames.py
	$(CC) tach.c $(FILES) -I./ $(LINKS) -o tachvm -O3
	rm lib/tach_conv.c lib/tach_lib.h lib/tach_libname.h