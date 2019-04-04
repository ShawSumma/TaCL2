#pragma once
#include "tach.h"

struct tach_string {
    char *str;
    uint32_t count;
    uint32_t alloc;
};

struct tach_vector {
    tach_object **objects;
    uint32_t count;
    uint32_t alloc;
};

struct tach_point {
    uint32_t point;
    uint32_t argc;
    tach_object **args;
};

struct tach_object {
    union {
        bool logical;
        tach_point point;
        double number;
        tach_func func;
        tach_vector *vector;
        tach_string string;
    } value;
    uint32_t refc;
    enum {
        tach_object_nil,
        tach_object_logical,
        tach_object_number,
        tach_object_point,
        tach_object_func,
        tach_object_string,
        tach_object_vector,
    } type;
};

struct tach_table {
    tach_object *key;
    tach_object *value;
    tach_table *left;
    tach_table *right;
};

tach_table *tach_create_table();
tach_string tach_create_string(char *);
void tach_set_table(tach_table *, tach_object * , tach_object *);
tach_object *tach_get_table(tach_table *, tach_object *);
tach_vector *tach_create_vector();
void tach_vector_push(tach_vector *, tach_object *);
tach_object *tach_vector_pop(tach_vector *);
tach_object *tach_vector_last(tach_vector *);

tach_object *tach_object_alloc();
tach_object *tach_object_make_nil();
tach_object *tach_object_make_logical(bool);
tach_object *tach_object_make_number(double);
tach_object *tach_object_make_point(uint32_t);
tach_object *tach_object_make_func(tach_func);
tach_object *tach_object_make_string(tach_string str);
tach_object *tach_object_make_vector(tach_vector *vec);

