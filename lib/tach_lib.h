#pragma once
#include <tach.h>

tach_state *tach_create_state(tach_program *prog);

tach_object *tach_lib_uplevel(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_print(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_echo(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_newline(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_add(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_mul(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_div(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_sub(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_mod(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_call(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_apply(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_proc(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_set(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_upset(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_global(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_get(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_if(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_copy(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_pass(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_lt(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_gt(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_lte(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_gte(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_eq(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_neq(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_exit(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_incr(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_count(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_table_new(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_table_set(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_table_has(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_table_concat(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_table_unpack(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vector_new(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vector_set(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vector_push(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vector_concat(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vector_slice(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vector_pop(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vector_last(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vector_split(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vector_len(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vector_map(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vars_global(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_vars_local(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_string_join(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_string_slice(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_string_split(tach_state *, uint32_t, tach_object **);
tach_object *tach_lib_string_index(tach_state *, uint32_t, tach_object **);