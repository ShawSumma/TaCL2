#include <tach.h>

void tach_errors_lineout(tach_state *state) {
    if (state->depth > 12) {
        for (uint32_t i = 1; i < 6; i ++) {
            fprintf(
                stderr,
                "    from line %d col %d\n",
                state->program->linenos[state->calls[i]],
                state->program->colnos[state->calls[i]]
            );
        }
        fprintf(stderr, "    ...\n");
        for (uint32_t i = state->depth-5; i < state->depth; i ++) {
            fprintf(
                stderr,
                "    from line %d col %d\n",
                state->program->linenos[state->calls[i]],
                state->program->colnos[state->calls[i]]
            );
        }
    }
    else {
        for (uint32_t i = 1; i < state->depth; i ++) {
            fprintf(
                stderr,
                "    from line %d col %d\n",
                state->program->linenos[state->calls[i]],
                state->program->colnos[state->calls[i]]
            );
        }
    }
    fprintf(
        stderr,
        ">   error on line %d at col %d!\n",
        state->program->linenos[state->place],
        state->program->colnos[state->place]
    );
}

void tach_errors_builtin_table_index(tach_state *state, tach_object *obj) {
    tach_errors_lineout(state);
    fprintf(stderr, "%s not in table\n", tach_clib_tostring(obj).str);
    exit(1);
}

void tach_errors_type_typecheck_anon(tach_state *state, tach_object *obj, uint32_t type) {
    if (obj->type != type) {
        char *bad = tach_clib_type_name(obj->type);
        char *good = tach_clib_type_name(type);
        tach_errors_lineout(state);
        fprintf(stderr, "object of type %s (not %s) needed\n", good, bad);
        exit(1);
    }
}

void tach_errors_index(tach_state *state, int32_t ind, int32_t min, int32_t max) {
    if (ind < min) {
        tach_errors_lineout(state);
        fprintf(stderr, "index out of range %d is less than %d", ind, min);
        exit(1);
    }
    if (ind > max) {
        tach_errors_lineout(state);
        fprintf(stderr, "index out of range %d is greater than %d", ind, max);
        exit(1);
    }
}

void tach_errors_type_argc(tach_state *state, char *name, uint32_t count, uint32_t min, uint32_t max) {
    if (max == min) {
        if (count != min) {
            tach_errors_lineout(state);
            fprintf(stderr, "%s take exatly %d (not %d) args\n", name, min, count);
            exit(1);
        }
    }
    else {
        if (count < min) {
            tach_errors_lineout(state);
            fprintf(stderr, "%s take no less than %d args (not %d)\n", name, min, count);
            exit(1);
        }
        if (count > max) {
            tach_errors_lineout(state);
            fprintf(stderr, "%s take no more than %d args (not %d)\n", name, max, count);
            exit(1);
        }
    }
}

void tach_errors_type_typecheck(tach_state *state, char *name, uint32_t argno, tach_object *obj, uint32_t type) {
    if (obj->type != type) {
        char *bad = tach_clib_type_name(obj->type);
        char *good = tach_clib_type_name(type);
        tach_errors_lineout(state);
        fprintf(stderr, "%s takes object of type %s (not %s) as arg number %d\n", name, good, bad, argno+1);
        exit(1);
    }
}

void tach_errors_type_typechecks(tach_state *state, char *name, uint32_t argno, tach_object *obj, uint32_t *types, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        if (obj->type == types[i]) {
            return;
        }
    }
    tach_errors_lineout(state);
    fprintf(stderr, "%s takes object of any type: ", name);
    for (uint32_t i = 0; i < count; i++) {
        char *good = tach_clib_type_name(types[i]);
        if (i != 0) {
            fprintf(stderr, ", ");
        }
        fprintf(stderr, "%s", good);
    }
    char *bad = tach_clib_type_name(obj->type);
    fprintf(stderr, "(not %s) as arg number %d\n", bad, argno+1);
}