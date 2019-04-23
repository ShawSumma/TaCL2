#pragma once
#include <tach.h>

tach_program *tach_bytecomp_main(tach_ast_proc *);
tach_program *tach_bytecomp_main_empty();
tach_program *tach_bytecomp_main_more(tach_ast_proc *proc, tach_program *ret);
void tach_bytecomp_proc(tach_program *, tach_ast_proc *);
void tach_bytecomp_command(tach_program *, tach_ast_command *);
void tach_bytecomp_single(tach_program *, tach_ast_single *);
void tach_program_realloc(tach_program *);
tach_program *tach_read(tach_file *);
tach_program *tach_read_repl(tach_ast_state *state, tach_file *, tach_program *);