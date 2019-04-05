#include "object/tach_all.h"

double fib(double n) {
    if (n < 2) {
        return n;
    }
    else {
        return fib(n-1) + fib(n-2);
    }
}

tach_object *ffi_fib(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 1 || args[0]->type != tach_object_number) {
        fprintf(stderr, "ffi_fib takes 1 arg, a number");
        exit(1);
    }
    double arg = tach_number_double(args[0]->value.number);
    double result = fib(arg);
    return tach_object_make_number(tach_create_number(result));
}