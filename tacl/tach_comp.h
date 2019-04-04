#pragma once
#include "tach_all.h"

tach_program *tach_comp_main(tach_ast_proc *);
void tach_comp_proc(tach_program *, tach_ast_proc *);
void tach_comp_command(tach_program *, tach_ast_command *);
void tach_comp_single(tach_program *, tach_ast_single *);
void tach_program_realloc(tach_program *);
tach_program *tach_read(FILE *);