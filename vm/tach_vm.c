#include <tach.h>

void tach_free_vector(tach_vector *vec) {
    for (uint32_t i = 0; i < vec->count; i++) {
        tach_free_object(vec->objects[i]);
    }
    free(vec->objects);
    free(vec);
}

void tach_free_state(tach_state *state) {
    for (uint32_t i = 0; i < state->depth; i++) {
        tach_free_table(state->locals[i]);
    }
    tach_free_program(state->program);
    tach_free_vector(state->stack);
    free(state->locals);
    free(state->calls);
    free(state);
}

void tach_free_program(tach_program *prog) {
    free(prog->opcodes);
    for (uint32_t i = 0; i < prog->objcount; i++) {
        tach_free_object(prog->objs[i]);
    }
    free(prog->objs);
    free(prog->linenos);
    free(prog->colnos);
    free(prog);
}

tach_state *tach_create_state_empty(tach_program *program) {
    tach_state *ret = malloc(sizeof(tach_state));
    ret->uplevels = 0;
    ret->depth = 1;
    ret->callalloc = 8;
    ret->locals = malloc(sizeof(tach_table *) * ret->callalloc);
    ret->calls = malloc(sizeof(uint32_t) * ret->callalloc);
    ret->stack = tach_create_vector();
    ret->locals[0] = tach_create_table();
    ret->place = 0;
    ret->program = program;
    return ret;
}

void tach_create_state_regester(tach_table *table, char *str, tach_object *obj) {
    tach_object *key = tach_object_make_string(tach_create_string(str));
    tach_set_table(table, key, obj);
    tach_free_object(key);
    tach_free_object(obj);
}

tach_object *tach_call(tach_state *state, tach_object *fn, uint32_t count, tach_object **args) {
    if (fn->type == tach_object_func) {
        tach_object *got = fn->value.func.func(state, count, args);
        return got;
    }
    else if(fn->type == tach_object_point) {
        if (state->depth + 4 > state->callalloc) {
            state->callalloc *= 1.5;
            state->calls = realloc(state->calls, sizeof(uint32_t) * state->callalloc);
            state->locals = realloc(state->locals, sizeof(tach_table *) * state->callalloc);
        }
        state->calls[state->depth] = state->place;
        state->locals[state->depth] = tach_create_table();
        tach_object *keyargc = tach_object_make_string(tach_create_string("argc"));
        tach_object *valargc = tach_object_make_number(tach_create_number(count));
        tach_set_table(
            state->locals[state->depth],
            keyargc,
            valargc
        );
        tach_free_object(keyargc);
        tach_free_object(valargc);
        for (uint32_t i = 0; i < count; i++) {
            char name[8];
            snprintf(name, 7, "%d", i);
            tach_object *r = tach_object_make_string(tach_create_string(name));
            tach_set_table(state->locals[state->depth], r, args[i]);
            tach_free_object(r);
        }
        for (uint32_t i = 0; i < fn->value.point.argc; i++) {
            tach_set_table(state->locals[state->depth], fn->value.point.args[i], args[i]);
        }
        state->place = fn->value.point.point + 1;
        state->depth ++;
        tach_object *got = tach_program_run(state);
        state->depth --;
        state->place = state->calls[state->depth];
        tach_free_table(state->locals[state->depth]);
        return got;
    }
    else if (fn->type == tach_object_table || fn->type == tach_object_vector) {
        while ((fn->type == tach_object_table || fn->type == tach_object_vector) && count > 0) {
            tach_object *old = fn;
            if (fn->type == tach_object_table) {
                fn = tach_get_table(fn->value.table, args[0]);
                if (fn == NULL) {
                    tach_errors_builtin_table_index(state, old);
                }
            }
            else if (fn->type == tach_object_vector) {
                tach_errors_type_typecheck_anon(state, args[0], tach_object_number);
                int32_t pl = tach_number_double(args[0]->value.number);
                int32_t size = fn->value.vector->count;
                tach_errors_index(state, pl, -(size-1), size-1);
                if (pl < 0) {
                    pl += size;
                }
                fn = fn->value.vector->objects[pl];
            }
            else {
                tach_errors_type_typecheck_anon(state, args[0], tach_object_vector);
            }
            count --;
            args ++;
        }
        fn->refc ++;
        return fn;
    }
    else {
        char *bad = tach_clib_type_name(fn->type);
        tach_errors_lineout(state);
        fprintf(stderr, "cannot call object of type %s\n", bad);
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
    char *bad = tach_clib_tostring(obj).str;
    tach_errors_lineout(state);
    printf("variable %s not found\n", bad);
    exit(1);
}

tach_object *tach_program_run(tach_state *state) {
    while (state->place < state->program->opcount) {
        tach_opcode op = state->program->opcodes[state->place];
        switch (op.type) {
            case tach_opcode_error: {
                fprintf(stderr, "opcode error!");
                exit(1);
                break;
            }
            case tach_opcode_push: {
                tach_vector_push(state->stack, state->program->objs[op.value]);
                break;
            }
            case tach_opcode_pop: {
                tach_vector_pop(state->stack);
                break;
            }
            case tach_opcode_load: {
                tach_vector_push(state->stack, tach_state_get(state, state->program->objs[op.value]));
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
                tach_object *fn = tach_vector_last(state->stack);
                fn->refc ++;
                tach_vector_pop(state->stack);
                tach_object *got = tach_call(state, fn, argc, args);
                tach_vector_push(state->stack, got);
                tach_free_object(got);
                for (uint32_t i = 0; i < argc; i++) {
                    tach_free_object(args[i]);
                }
                tach_free_object(fn);
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
                tach_object *obj = tach_vector_last(state->stack);
                obj->refc ++;
                tach_vector_pop(state->stack);
                return obj;
                break;
            }
        }
        state->place ++;
    }
    if (state->stack->count == 0) {
        return tach_object_make_nil();
    }
    tach_object *obj = tach_vector_last(state->stack);
    obj->refc ++;
    tach_vector_pop(state->stack);
    return obj;
}