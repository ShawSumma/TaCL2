#pragma once
#include "tach.h"

struct tach_state {
    tach_vector *stack;
    tach_table **locals;
    uint32_t *calls;
    uint32_t callalloc;
    uint32_t depth;
    uint32_t uplevels;
    uint32_t place;
};

void tach_create_state_regester(tach_table *, char *, tach_object *);
tach_state *tach_create_state();
void tach_program_run(tach_state *, tach_program *);
tach_object *tach_state_get(tach_state *, tach_object *);
void tach_call(tach_state *, tach_object *, uint32_t, tach_object **);