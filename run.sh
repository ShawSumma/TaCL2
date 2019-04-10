if clang tach.c tacl/*.c lib/*.c vm/*.c object/*.c object/export/*.c -std=c99 -lgmp -O3 -g -I./ -o tachvm 
then
    time ./tachvm $1
fi