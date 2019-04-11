#pragma once
#include "tach.h"

char *tach_func_to_name(tach_func);
tach_func tach_name_to_func(char *);

tach_object *tach_lib_print(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_get(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_call(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_proc(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_set(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_get(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_upset(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_global(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_if(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_copy(tach_state *, uint32_t count, tach_object **args);

tach_object *tach_lib_add(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_mul(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_div(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_sub(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_lt(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_gt(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_lte(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_gte(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_eq(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_neq(tach_state *, uint32_t count, tach_object **args);

tach_object *tach_lib_str(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_len(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_cat(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_ind(tach_state *, uint32_t count, tach_object **args);

tach_object *tach_lib_new_table(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_new_vector(tach_state *, uint32_t count, tach_object **args);

tach_object *tach_lib_export(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_import(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_save_state(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_save_state_die(tach_state *, uint32_t count, tach_object **args);
tach_object *tach_lib_run_state(tach_state *, uint32_t count, tach_object **args);

