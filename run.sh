if clang tach.c tacl/*.c lib/*.c vm/*.c object/*.c export/*.c files/*.c -std=c99 -lgmp -I./ -O3 -g -o tachvm 
then
    valgrind --leak-check=full ./tachvm $1
fi