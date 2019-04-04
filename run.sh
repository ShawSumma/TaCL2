clang tach.c lib/*.c vm/*.c object/*.c -std=c99 -O3 -lgmp -ldl -I./ -o tachvm
cd tacl
clang *.c ../object/*.c ../vm/*.c ../lib/*.c -shared -fPIC -lgmp -O3 -I.. -o tacl.so
cd ..
./tachvm tacl/tacl.so tach/tcl.tach