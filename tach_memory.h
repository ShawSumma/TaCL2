#pragma once
#include "tach.h"

void tach_ast_free_proc(tach_ast_proc *);
void tach_ast_free_command(tach_ast_command *);
void tach_ast_free_single(tach_ast_single *);
void tach_ast_free_name(char *);
void tach_free_object(tach_object *);
void tach_free_vector(tach_vector *);
void tach_free_program(tach_program *);
void tach_free_state(tach_state *);
void tach_free_table(tach_table *);

