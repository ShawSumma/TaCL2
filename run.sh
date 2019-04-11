if clang tach.c tacl/*.c lib/*.c vm/*.c object/*.c object/export/*.c -std=c99 -lgmp -O3 -g -I./ -o tachvm 
then
    tcc tach.c tacl/*.c lib/*.c vm/*.c object/*.c object/export/*.c -std=c99 -lgmp -O3 -g -I./ -o tachvm 
    # time  valgrind --leak-check=full --show-leak-kinds=all ./tachvm $1
    time ./tachvm $1
fi