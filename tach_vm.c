#include "tach.h"

void tach_create_state_regester(tach_table *table, char *str, tach_object *obj) {
    tach_object *key = tach_object_make_string(tach_create_string(str));
    tach_set_table(table, key, obj);
    tach_free_object(key);
    tach_free_object(obj);
}

tach_state *tach_create_state() {
    tach_state *ret = malloc(sizeof(tach_state));
    ret->depth = 1;
    ret->callalloc = 8;
    ret->calls = malloc(sizeof(uint32_t) * ret->callalloc);
    ret->locals = malloc(sizeof(tach_table *) * ret->callalloc);
    ret->stack = tach_create_vector();
    ret->calls[0] = -1;
    ret->locals[0] = tach_create_table();
    tach_object *key;
    tach_object *val;

    tach_create_state_regester(ret->locals[0], "echo", tach_object_make_func(tach_lib_print));
    tach_create_state_regester(ret->locals[0], "print", tach_object_make_func(tach_lib_print));
    tach_create_state_regester(ret->locals[0], "add", tach_object_make_func(tach_lib_add));
    tach_create_state_regester(ret->locals[0], "mul", tach_object_make_func(tach_lib_mul));
    tach_create_state_regester(ret->locals[0], "div", tach_object_make_func(tach_lib_div));
    tach_create_state_regester(ret->locals[0], "sub", tach_object_make_func(tach_lib_sub));
    tach_create_state_regester(ret->locals[0], "get", tach_object_make_func(tach_lib_get));
    tach_create_state_regester(ret->locals[0], "exec", tach_object_make_func(tach_lib_call));
    tach_create_state_regester(ret->locals[0], "proc", tach_object_make_func(tach_lib_proc));
    tach_create_state_regester(ret->locals[0], "set", tach_object_make_func(tach_lib_set));
    tach_create_state_regester(ret->locals[0], "if", tach_object_make_func(tach_lib_if));
    tach_create_state_regester(ret->locals[0], "copy", tach_object_make_func(tach_lib_copy));
    tach_create_state_regester(ret->locals[0], "lt", tach_object_make_func(tach_lib_lt));

    return ret;
}

void tach_call(tach_state *state, tach_object *fn, uint32_t count, tach_object **args) {
    if (fn->type == tach_object_func) {
        tach_object *obj = fn->value.func(state, count, args);
        if (obj != NULL) {
            tach_vector_push(state->stack, obj);
            tach_free_object(obj);
        }
    }
    else if(fn->type == tach_object_point) {
        if (state->depth + 4 > state->callalloc) {
            state->callalloc *= 1.5;
            state->calls = realloc(state->calls, sizeof(uint32_t) * state->callalloc);
            state->locals = realloc(state->locals, sizeof(tach_table *) * state->callalloc);
        }
        state->calls[state->depth] = state->place;
        state->locals[state->depth] = tach_create_table();
        for (uint32_t i = 0; i < fn->value.point.argc; i++) {
            tach_set_table(state->locals[state->depth], fn->value.point.args[i], args[i]);
        }
        state->place = fn->value.point.point;
        state->depth ++;
    }
    else {
        fprintf(stderr, "call error!\n");
        exit(1);
    }
}

tach_object *tach_state_get(tach_state *state, tach_object *obj) {
    tach_object *got = tach_get_table(state->locals[0], obj);
    if (got != NULL) {
        return got;
    }
    for (uint32_t i = state->depth - 1; i > 0; i--) {
        tach_object *got = tach_get_table(state->locals[i], obj);
        if (got != NULL) {
            return got;
        }
    }
    printf("variable not found\n");
    exit(1);
}

void tach_program_run(tach_state *state, tach_program *prog) {
    state->place = 0;
    while (state->place < prog->opcount) {
        tach_opcode op = prog->opcodes[state->place];
        switch (op.type) {
            case tach_opcode_error: {
                fprintf(stderr, "opcode error!");
                exit(1);
                break;
            }
            case tach_opcode_push: {
                tach_vector_push(state->stack, prog->objs[op.value]);
                break;
            }
            case tach_opcode_pop: {
                tach_object *obj = tach_vector_pop(state->stack);
                break;
            }
            case tach_opcode_load: {
                tach_vector_push(state->stack, tach_state_get(state, prog->objs[op.value]));                
                break;
            }
            case tach_opcode_call: {
                uint32_t argc = op.value;
                tach_object **args = malloc(sizeof(tach_object *) * argc);
                for (uint32_t i = 0; i < argc; i++) {
                    tach_object *arg = tach_vector_last(state->stack);
                    arg->refc ++;
                    args[argc-i-1] = arg;
                    tach_vector_pop(state->stack);
                }
                tach_object *fn = tach_vector_pop(state->stack);
                tach_call(state, fn, argc, args);
                for (uint32_t i = 0; i < argc; i++) {
                    tach_free_object(args[i]);
                }
                free(args);
                break;
            }
            case tach_opcode_proc: {
                tach_object *point = tach_object_make_point(state->place);
                tach_vector_push(state->stack, point);
                tach_free_object(point);
                state->place = op.value;
                break;
            }
            case tach_opcode_ret: {
                state->depth --;
                state->place = state->calls[state->depth];
                tach_free_table(state->locals[state->depth]);
                break;
            }
        }
        state->place ++;
    }
}