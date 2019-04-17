#pragma once
#include "tach.h"

void tach_ast_print_program(tach_file *, tach_ast_proc *);
void tach_ast_print_nspaces(tach_file *, uint32_t);
void tach_ast_print_proc(tach_file *, tach_ast_proc *, uint32_t);
void tach_ast_print_command(tach_file *, tach_ast_command *, uint32_t);
void tach_ast_print_single(tach_file *, tach_ast_single *, uint32_t);