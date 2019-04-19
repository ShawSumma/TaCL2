#pragma once
#include <tach.h>

tach_program *tach_comp_main(tach_ast_proc *);
tach_program *tach_comp_main_empty();
tach_program *tach_comp_main_more(tach_ast_proc *proc, tach_program *ret);
void tach_comp_proc(tach_program *, tach_ast_proc *);
void tach_comp_command(tach_program *, tach_ast_command *);
void tach_comp_single(tach_program *, tach_ast_single *);
void tach_program_realloc(tach_program *);
tach_program *tach_read(tach_file *);
tach_program *tach_read_repl(tach_file *, tach_program *);