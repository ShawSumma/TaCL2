#include "tach.h"

tach_object *tach_lib_print(tach_state *state, uint32_t argc, tach_object **args) {
    for (uint32_t i = 0; i < argc; i++) {
        if (i != 0) {
            printf(" ");
        }
        tach_string str = tach_clib_tostring(args[i]);
        printf("%s", str.str);
        free(str.str);
    }
    printf("\n");
    return tach_object_make_nil();
}

tach_object *tach_lib_lt(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "lt takes 2 args\n");
        exit(1);
    }
    return tach_object_make_logical(tach_clib_compare(args[0], args[1]) == -1);
}

tach_object *tach_lib_add(tach_state *state, uint32_t argc, tach_object **args) {
    double ret = 0;
    for (uint32_t i = 0; i < argc; i++) {
        if (args[i]->type == tach_object_number) {
            ret += args[i]->value.number;
        }
        else {
            fprintf(stderr, "add only takes numbers");
            exit(1);
        }
    }
    return tach_object_make_number(ret);
}

tach_object *tach_lib_mul(tach_state *state, uint32_t argc, tach_object **args) {
    double ret = 1;
    for (uint32_t i = 0; i < argc; i++) {
        if (args[i]->type == tach_object_number) {
            ret *= args[i]->value.number;
        }
        else {
            fprintf(stderr, "mul only takes numbers");
            exit(1);
        }
    }
    return tach_object_make_number(ret);
}

tach_object *tach_lib_sub(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc < 1) {
        fprintf(stderr, "sub takes 1 or more args");
        exit(1);
    }
    if (args[0]->type != tach_object_number) {
        fprintf(stderr, "sub only takes numbers");
        exit(1);
    }
    double ret = args[0]->value.number;
    for (uint32_t i = 1; i < argc; i++) {
        if (args[i]->type == tach_object_number) {
            ret -= args[i]->value.number;
        }
    }
    return tach_object_make_number(ret);
}

tach_object *tach_lib_div(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc < 1) {
        fprintf(stderr, "div takes 1 or more args");
        exit(1);
    }
    if (args[0]->type != tach_object_number) {
        fprintf(stderr, "div only takes numbers");
        exit(1);
    }
    double ret = args[0]->value.number;
    for (uint32_t i = 1; i < argc; i++) {
        if (args[i]->type == tach_object_number) {
            ret /= args[i]->value.number;
        }
    }
    return tach_object_make_number(ret);
}

tach_object *tach_lib_get(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 1) {
        fprintf(stderr, "get takes 1 arg");
        exit(1);
    }
    tach_object *obj = tach_state_get(state, args[0]);
    obj->refc ++;
    return obj;
}

tach_object *tach_lib_call(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 1) {
        fprintf(stderr, "call takes 1 arg");
        exit(1);
    }
    tach_call(state, args[0], 0, NULL);
    // return tach_object_make_nil();
    return NULL;
}

tach_object *tach_lib_proc(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc < 2) {
        fprintf(stderr, "proc takes 2 or more args");
        exit(1);
    }
    tach_object *obj = args[argc-1];
    obj->value.point.argc = argc-2;
    obj->value.point.args = malloc(sizeof(tach_object *) * obj->value.point.argc);
    for (uint32_t i = 1; i < argc-1; i++) {
        args[i]->refc ++;
        obj->value.point.args[i-1] = args[i];
    }
    tach_set_table(state->locals[state->depth-1], args[0], obj);
    return tach_object_make_nil();
}

tach_object *tach_lib_set(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "set takes 2 or args");
        exit(1);
    }
    tach_set_table(state->locals[state->depth-1], args[0], args[1]);
    return tach_object_make_nil();
}

tach_object *tach_lib_if(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "if takes 2 or 3 args\n");
        exit(1);
    }
    bool first = true;
    if (args[0]->type == tach_object_number && args[0]->value.number == 0) {
        first = false;
    }
    else if (args[0]->type == tach_object_logical && !args[0]->value.logical) {
        first = false;
    }
    else if (args[0]->type == tach_object_nil) {
        first = false;
    }
    if (argc == 2) {
        if (first) {
            tach_call(state, args[1], 0, NULL);
        }
    }
    else if (argc == 3) {
        if (first) {
            tach_call(state, args[1], 0, NULL);
        }
        else {
            tach_call(state, args[2], 0, NULL);
        }
    }
    // return tach_object_make_nil();
    return NULL;
}

tach_object *tach_lib_copy(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc == 0) {
        fprintf(stderr, "copy takes 1 or more arguments");
        exit(1);
    }
    args[argc-1]->refc ++;
    return args[argc-1];
}

