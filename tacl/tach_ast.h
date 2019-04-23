#pragma once
#include <tach.h>

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
    uint32_t lineno;
    uint32_t colno;
};

struct tach_ast_command {
    tach_ast_single **singles;
    uint32_t count;
    uint32_t lineno;
    uint32_t colno;
};

struct tach_ast_proc {
    tach_ast_command **commands;
    uint32_t count;
    uint32_t lineno;
    uint32_t colno;
};

struct tach_ast_state {
    uint32_t lineno;
    uint32_t colno;
    uint32_t ld;
};

tach_ast_state *tach_create_ast_state();
char tach_ast_state_getc(tach_ast_state *, tach_file *);
void tach_ast_state_ungetc(tach_ast_state *, char, tach_file *);
tach_ast_proc *tach_ast_read_proc(tach_ast_state *, tach_file *, bool);
tach_ast_command *tach_ast_read_command(tach_ast_state *, tach_file *, bool);
tach_ast_single *tach_ast_read_single(tach_ast_state *, tach_file *);
char *tach_ast_read_name(tach_ast_state *, tach_file *);
char *tach_ast_read_number(tach_ast_state *, tach_file *);

