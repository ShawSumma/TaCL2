#pragma once
#include "tach.h"

void tach_ast_free_proc(tach_ast_proc *);
void tach_ast_free_command(tach_ast_command *);
void tach_ast_free_single(tach_ast_single *);
void tach_ast_free_name(char *);