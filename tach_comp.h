#pragma once
#include "tach.h"

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

tach_program *tach_comp_main(tach_ast_proc *);
void tach_comp_proc(tach_program *, tach_ast_proc *);
void tach_comp_command(tach_program *, tach_ast_command *);
void tach_comp_single(tach_program *, tach_ast_single *);
void tach_program_realloc(tach_program *);
