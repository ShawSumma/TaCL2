if clang tach.c tacl/*.c lib/*.c vm/*.c object/*.c -std=c99 -O3 -lgmp -ldl -I./ -o tachvm 
then
    time ./tachvm $1
fi