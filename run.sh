if tcc tach.c tacl/*.c lib/*.c vm/*.c object/*.c object/export/*.c -std=c99 -lgmp -I./ -o tachvm 
then
    ./tachvm $1
fi