#pragma once
#include "tach.h"

struct tach_number {
    mpq_t number;
};

tach_number *tach_create_number(double);
tach_number *tach_create_number_string(char *);
tach_number *tach_number_copy(tach_number *);
char *tach_number_tostring(tach_number *);
bool tach_number_lt(tach_number *, tach_number *);
bool tach_number_gt(tach_number *, tach_number *);
void tach_number_add(tach_number *, tach_number *);
void tach_number_sub(tach_number *, tach_number *);
void tach_number_mul(tach_number *, tach_number *);
void tach_number_div(tach_number *, tach_number *);
void tach_number_clear(tach_number *);
double tach_number_double(tach_number *);