if tcc tach.c tacl/*.c lib/*.c vm/*.c object/*.c -std=c99 -O3 -lgmp -ldl -ltcc -I./ -o tachvm
then
    ./tachvm tacl/tacl.so $1
fi