#pragma once

#include "tach.h"

void tach_free_object(tach_object *);
void tach_free_vector(tach_vector *);
void tach_free_program(tach_program *);
void tach_free_state(tach_state *);
void tach_free_table(tach_table *);
void tach_free_number(tach_number *);