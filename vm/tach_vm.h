#pragma once
#include "tach.h"

struct tach_state {
    tach_program *program;
    tach_vector *stack;
    tach_table **locals;
    uint32_t *calls;
    uint32_t callalloc;
    uint32_t depth;
    uint32_t uplevels;
    uint32_t place;
};

struct tach_opcode {
    uint32_t value;
    enum {
        tach_opcode_error,
        tach_opcode_push,
        tach_opcode_pop,
        tach_opcode_proc,
        tach_opcode_ret,
        tach_opcode_load,
        tach_opcode_call,
    } type;
};

struct tach_program {
    tach_object **objs;
    tach_opcode *opcodes;
    uint32_t objcount;
    uint32_t objalloc;
    uint32_t opalloc;
    uint32_t opcount;
};

tach_state *tach_create_state();
void tach_create_state_regester(tach_table *, char *, tach_object *);
void tach_program_run(tach_state *, tach_program *);
tach_object *tach_state_get(tach_state *, tach_object *);
void tach_call(tach_state *, tach_object *, uint32_t, tach_object **);