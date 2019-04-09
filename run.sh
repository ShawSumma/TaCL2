if clang tach.c tacl/*.c lib/*.c vm/*.c object/*.c -std=c99 -lgmp -O3 -ldl -I./ -o tachvm 
then
    sudo perf record ./tachvm $1
    sudo perf report
fi