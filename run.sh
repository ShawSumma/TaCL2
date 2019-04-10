if clang tach.c tacl/*.c lib/*.c vm/*.c object/*.c -std=c99 -lgmp -O3 -I./ -o tachvm 
then
    time ./tachvm $1
fi