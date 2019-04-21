#include <tach.h>

void tach_errors_type_argc(tach_state *state, char *name, uint32_t count, uint32_t min, uint32_t max) {
    if (max == min) {
        if (count != min) {
            fprintf(stderr, "%s take exatly %d (not %d) args\n", name, count, min);
            exit(1);
        }
    }
    else {
        if (count < min) {
            fprintf(stderr, "%s take no less than %d args\n", name, min);
            exit(1);
        }
        if (count > max) {
            fprintf(stderr, "%s take no more than %d args\n", name, max);
            exit(1);
        }
    }
}

void tach_errors_type_typecheck(tach_state *state, char *name, uint32_t argno, tach_object *obj, uint32_t type) {
    if (obj->type != type) {
        char *bad = tach_clib_type_name(obj->type);
        char *good = tach_clib_type_name(type);
        fprintf(stderr, "%s takes object of type %s (not %s) as arg number %d\n", name, good, bad, argno);
        exit(1);
    }
}

void tach_errors_type_typechecks(tach_state *state, char *name, uint32_t argno, tach_object *obj, uint32_t *types, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        if (obj->type == types[i]) {
            return;
        }
    }
    fprintf(stderr, "%s takes object of any type: ", name);
    for (uint32_t i = 0; i < count; i++) {
        char *good = tach_clib_type_name(types[i]);
        if (i != 0) {
            fprintf(stderr, ", ");
        }
        fprintf(stderr, "%s", good);
    }
    char *bad = tach_clib_type_name(obj->type);
    fprintf(stderr, "(not %s) as arg number %d\n", bad, argno);
}