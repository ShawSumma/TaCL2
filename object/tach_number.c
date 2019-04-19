#include <tach.h>

#if defined tach_use_big_rational
tach_number *tach_create_number(double n) {
    tach_number *ret = malloc(sizeof(tach_number));
    mpq_init(ret->number);
    mpq_set_d(ret->number, n);
    return ret;
}
tach_number *tach_create_number_string(char *str) {
    tach_number *ret = malloc(sizeof(tach_number));
    mpq_init(ret->number);
    mpq_set_str(ret->number, str, 10);
    return ret;
}
tach_number *tach_number_copy(tach_number *n) {
    tach_number *ret = malloc(sizeof(tach_number));
    mpq_init(ret->number);
    mpq_set(ret->number, n->number);
    return ret;
}
char *tach_number_tostring(tach_number *num) {
    char *ret = mpq_get_str(NULL, 10, num->number);
    return ret;
}
bool tach_number_lt(tach_number *a, tach_number *b) {
    return mpq_cmp(a->number, b->number) < 0;
}
bool tach_number_gt(tach_number *a, tach_number *b) {
    return mpq_cmp(a->number, b->number) > 0;
}
void tach_number_add(tach_number *a, tach_number *b) {
    mpq_add(a->number, a->number, b->number);
}
void tach_number_sub(tach_number *a, tach_number *b) {
    mpq_sub(a->number, a->number, b->number);
}
void tach_number_mul(tach_number *a, tach_number *b) {
    mpq_mul(a->number, a->number, b->number);
}
void tach_number_div(tach_number *a, tach_number *b) {
    mpq_div(a->number, a->number, b->number);
}
void tach_number_clear(tach_number *num) {
    mpq_clear(num->number);
}
double tach_number_double(tach_number *num) {
    return mpq_get_d(num->number);
}
#else
tach_number *tach_create_number(double n) {
    tach_number *ret = malloc(sizeof(tach_number));
    ret->number = n;
    return ret;
}
tach_number *tach_create_number_string(char *str) {
    tach_number *ret = malloc(sizeof(tach_number));
    char *end;
    ret->number = strtod(str, &end);
    return ret;
}
tach_number *tach_number_copy(tach_number *n) {
    tach_number *ret = malloc(sizeof(tach_number));
    ret->number = n->number;
    return ret;
}
char *tach_number_tostring(tach_number *num) {
    uint32_t count = snprintf(NULL, 0, "%lf", num->number);
    char *ret = malloc(sizeof(char) * (count + 2));
    snprintf(ret, count + 1, "%lf", num->number);
    return ret;
}
bool tach_number_lt(tach_number *a, tach_number *b) {
    return a->number < b->number;
}
bool tach_number_gt(tach_number *a, tach_number *b) {
    return a->number > b->number;
}
void tach_number_add(tach_number *a, tach_number *b) {
    a->number += b->number;
}
void tach_number_sub(tach_number *a, tach_number *b) {
    a->number -= b->number;
}
void tach_number_mul(tach_number *a, tach_number *b) {
    a->number *= b->number;
}
void tach_number_div(tach_number *a, tach_number *b) {
    a->number /= b->number;
}
void tach_number_clear(tach_number *num) {
}
double tach_number_double(tach_number *num) {
    return num->number;
}
#endif