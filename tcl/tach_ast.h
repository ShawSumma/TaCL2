#pragma once
#include "tach.h"

struct tach_ast_single {
    enum {
        tach_ast_single_name,
        tach_ast_single_string,
        tach_ast_single_number,
        tach_ast_single_command,
        tach_ast_single_proc,
    } type;
    union {
        tach_ast_command *command;
        tach_ast_proc *proc;
        char *name;
        char *string;
        char *number;
    } value;
};


struct tach_ast_command {
    tach_ast_single **singles;
    uint32_t count;
};

struct tach_ast_proc {
    tach_ast_command **commands;
    uint32_t count;
};

tach_ast_proc *tach_ast_read_proc(FILE *f);
tach_ast_command *tach_ast_read_command(FILE *f);
tach_ast_single *tach_ast_read_single(FILE *f);
char *tach_ast_read_name(FILE *f);
char *tach_ast_read_number(FILE *f);

