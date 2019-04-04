#pragma once
#include "tach_all.h"


tach_object *tach_lib_print(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_add(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_mul(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_div(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_sub(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_get(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_call(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_proc(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_set(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_if(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_copy(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_lt(tach_state *, uint32_t count, tach_object **args);
