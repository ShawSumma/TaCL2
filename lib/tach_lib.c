#include "tach.h"

tach_state *tach_create_state() {
    tach_state *ret = malloc(sizeof(tach_state));
    ret->depth = 1;
    ret->callalloc = 8;
    ret->calls = malloc(sizeof(uint32_t) * ret->callalloc);
    ret->locals = malloc(sizeof(tach_table *) * ret->callalloc);
    ret->stack = tach_create_vector();
    ret->calls[0] = -1;
    ret->locals[0] = tach_create_table();
    ret->place = 0;

    tach_create_state_regester(ret->locals[0], "save-state", tach_object_make_func(tach_lib_save_state));
    tach_create_state_regester(ret->locals[0], "save-state-die", tach_object_make_func(tach_lib_save_state_die));
    tach_create_state_regester(ret->locals[0], "run-state", tach_object_make_func(tach_lib_run_state));
    
    tach_create_state_regester(ret->locals[0], "len", tach_object_make_func(tach_lib_len));
    tach_create_state_regester(ret->locals[0], "str", tach_object_make_func(tach_lib_str));

    tach_create_state_regester(ret->locals[0], "print", tach_object_make_func(tach_lib_print));
    
    tach_create_state_regester(ret->locals[0], "add", tach_object_make_func(tach_lib_add));
    tach_create_state_regester(ret->locals[0], "mul", tach_object_make_func(tach_lib_mul));
    tach_create_state_regester(ret->locals[0], "div", tach_object_make_func(tach_lib_div));
    tach_create_state_regester(ret->locals[0], "sub", tach_object_make_func(tach_lib_sub));

    tach_create_state_regester(ret->locals[0], "call", tach_object_make_func(tach_lib_call));
    tach_create_state_regester(ret->locals[0], "proc", tach_object_make_func(tach_lib_proc));

    tach_create_state_regester(ret->locals[0], "set", tach_object_make_func(tach_lib_set));
    tach_create_state_regester(ret->locals[0], "upset", tach_object_make_func(tach_lib_upset));
    tach_create_state_regester(ret->locals[0], "global", tach_object_make_func(tach_lib_global));
    tach_create_state_regester(ret->locals[0], "get", tach_object_make_func(tach_lib_get));

    tach_create_state_regester(ret->locals[0], "if", tach_object_make_func(tach_lib_if));

    tach_create_state_regester(ret->locals[0], "copy", tach_object_make_func(tach_lib_copy));
    
    tach_create_state_regester(ret->locals[0], "lt", tach_object_make_func(tach_lib_lt));
    tach_create_state_regester(ret->locals[0], "gt", tach_object_make_func(tach_lib_gt));
    tach_create_state_regester(ret->locals[0], "lte", tach_object_make_func(tach_lib_lte));
    tach_create_state_regester(ret->locals[0], "gte", tach_object_make_func(tach_lib_gte));
    tach_create_state_regester(ret->locals[0], "eq", tach_object_make_func(tach_lib_eq));
    tach_create_state_regester(ret->locals[0], "neq", tach_object_make_func(tach_lib_neq));

    tach_create_state_regester(ret->locals[0], "true", tach_object_make_logic(true));
    tach_create_state_regester(ret->locals[0], "false", tach_object_make_logic(false));

    tach_table *table = tach_create_table();
    tach_create_state_regester(table, "new", tach_object_make_func(tach_lib_table_new));
    tach_create_state_regester(table, "set", tach_object_make_func(tach_lib_table_set));
    tach_create_state_regester(table, "has", tach_object_make_func(tach_lib_table_has));
    tach_create_state_regester(ret->locals[0], "table", tach_object_make_table(table));

    tach_table *vector = tach_create_table();
    tach_create_state_regester(vector, "new", tach_object_make_func(tach_lib_vector_new));
    tach_create_state_regester(vector, "set", tach_object_make_func(tach_lib_vector_set));
    tach_create_state_regester(vector, "push", tach_object_make_func(tach_lib_vector_push));
    tach_create_state_regester(ret->locals[0], "vector", tach_object_make_table(vector));

    return ret;
}

tach_object *tach_lib_save_state(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 1) {
        fprintf(stderr, "save-state takes 1 arg\n");
    }
    tach_file *f = tach_fopen(args[0]->value.string.str, "wb");
    tach_export_program_to_file(state->program, f);
    tach_export_state_to_file(state, f);
    tach_fclose(f);
    return tach_object_make_nil();
}

tach_object *tach_lib_save_state_die(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 1) {
        fprintf(stderr, "save-state-die takes 1 arg\n");
    }
    tach_file *f = tach_fopen(args[0]->value.string.str, "wb");
    tach_export_program_to_file(state->program, f);
    tach_export_state_to_file(state, f);
    tach_fclose(f);
    exit(0);
}

tach_object *tach_lib_run_state(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 1) {
        fprintf(stderr, "run-state takes 1 arg\n");
    }
    tach_file *f = tach_fopen(args[0]->value.string.str, "rb");
    tach_program *prog = tach_export_file_to_program(f);
    tach_state *new_state = tach_export_file_to_state(f);
    tach_fclose(f);
    new_state->place ++;
    tach_object *obj = tach_object_make_nil();
    tach_vector_push(new_state->stack, obj);
    tach_free_object(obj);
    
    
    
    tach_program_run(new_state, prog);
    tach_free_state(new_state);
    tach_free_program(prog);
    return tach_object_make_nil();
}

tach_object *tach_lib_vector_new(tach_state *state, uint32_t argc, tach_object **args) {
    tach_vector *vec = tach_create_vector();
    for (uint32_t i = 0; i < argc; i++) {
        tach_vector_push(vec, args[i]);
    }
    return tach_object_make_vector(vec);
}

tach_object *tach_lib_vector_set(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 3) {
        fprintf(stderr, "vector set takes 3 args\n");
        exit(1);
    }
    printf("%s\n", tach_clib_tostring(args[0]).str);
    if (args[0]->type != tach_object_vector && args[1]->type != tach_object_number) {
        fprintf(stderr, "vector set takes vector number any\n");
        exit(1);
    }
    tach_set_vector(args[0]->value.vector, tach_number_double(args[1]->value.number), args[2]);
    return tach_object_make_nil();
}

tach_object *tach_lib_table_set(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 3) {
        fprintf(stderr, "table set takes 3 args\n");
        exit(1);
    }
    if (args[0]->type != tach_object_table) {
        fprintf(stderr, "table set takes table any any\n");
        exit(1);
    }
    tach_set_table(args[0]->value.table, args[1], args[2]);
    return tach_object_make_nil();
}

tach_object *tach_lib_table_has(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "table has takes 2 args\n");
        exit(1);
    }
    if (args[0]->type != tach_object_table) {
        fprintf(stderr, "table has takes table any\n");
    }
    return tach_object_make_logic(tach_get_table(args[0]->value.table, args[1])  != NULL);
}

tach_object *tach_lib_vector_push(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc < 1) {
        fprintf(stderr, "vector push takes 1 or more args\n");
        exit(1);
    }
    if (args[0]->type != tach_object_vector) {
        fprintf(stderr, "vector push needs a vector as the first argument\n");
        exit(1);
    }
    for (uint32_t i = 1; i < argc; i++) {
        tach_vector_push(args[0]->value.vector, args[i]);
    }
    return tach_object_make_nil();
}

tach_object *tach_lib_table_new(tach_state *state, uint32_t argc, tach_object **args) {
    tach_table *vec = tach_create_table();
    for (uint32_t i = 0; i < argc; i+=2) {
        tach_set_table(vec, args[i], args[i+1]);
    }
    return tach_object_make_table(vec);
}

tach_object *tach_lib_str(tach_state *state, uint32_t argc, tach_object **args) {
    tach_string *strs = malloc(sizeof(tach_string) * argc);
    uint32_t len = 0;
    for (uint32_t i = 0; i < argc; i++) {
        strs[i] = tach_clib_tostring(args[i]);
        len += strs[i].count;
    }
    char *c = malloc(sizeof(char) * (len+1));
    for (uint32_t i = 0, pl = 0; i < argc; i++) {
        for (uint32_t si = 0; si < strs[i].count; si++, pl++){
            c[pl] = strs[i].str[si];
        }
        free(strs[i].str);
    }
    free(strs);
    c[len] = '\0';
    tach_string str;
    str.str = c;
    str.alloc = len + 1;
    str.count = len;
    return tach_object_make_string(str);
}

tach_object *tach_lib_len(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 1) {
        fprintf(stderr, "len takes 1 arg\n");
        exit(1);
    }
    return tach_object_make_number(tach_create_number(args[0]->value.string.count));
}

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
    return tach_object_make_logic(tach_clib_compare(args[0], args[1]) == -1);
}

tach_object *tach_lib_gt(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "gt takes 2 args\n");
        exit(1);
    }
    return tach_object_make_logic(tach_clib_compare(args[0], args[1]) == 1);
}

tach_object *tach_lib_gte(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "gte takes 2 args\n");
        exit(1);
    }
    return tach_object_make_logic(tach_clib_compare(args[0], args[1]) != -1);
}

tach_object *tach_lib_lte(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "lte takes 2 args\n");
        exit(1);
    }
    return tach_object_make_logic(tach_clib_compare(args[0], args[1]) != 1);
}

tach_object *tach_lib_eq(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "gq takes 2 args\n");
        exit(1);
    }
    return tach_object_make_logic(tach_clib_compare(args[0], args[1]) == 0);
}

tach_object *tach_lib_neq(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "neq takes 2 args\n");
        exit(1);
    }
    return tach_object_make_logic(tach_clib_compare(args[0], args[1]) != 0);
}

tach_object *tach_lib_add(tach_state *state, uint32_t argc, tach_object **args) {
    tach_number *ret = tach_create_number(0);
    for (uint32_t i = 0; i < argc; i++) {
        if (args[i]->type == tach_object_number) {
            tach_number_add(ret, args[i]->value.number);
        }
        else {
            fprintf(stderr, "add only takes numbers\n");
            exit(1);
        }
    }
    return tach_object_make_number(ret);
}

tach_object *tach_lib_mul(tach_state *state, uint32_t argc, tach_object **args) {
    tach_number *ret = tach_create_number(1);
    for (uint32_t i = 0; i < argc; i++) {
        if (args[i]->type == tach_object_number) {
            tach_number_mul(ret, args[i]->value.number);
        }
        else {
            fprintf(stderr, "mul only takes numbers\n");
            exit(1);
        }
    }
    return tach_object_make_number(ret);
}

tach_object *tach_lib_sub(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc < 1) {
        fprintf(stderr, "sub takes 1 or more args\n");
        exit(1);
    }
    if (args[0]->type != tach_object_number) {
        fprintf(stderr, "sub only takes numbers\n");
        exit(1);
    }
    tach_number *ret = tach_number_copy(args[0]->value.number);
    for (uint32_t i = 1; i < argc; i++) {
        if (args[i]->type == tach_object_number) {
            tach_number_sub(ret, args[i]->value.number);
        }
    }
    return tach_object_make_number(ret);
}

tach_object *tach_lib_div(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc < 1) {
        fprintf(stderr, "div takes 1 or more args\n");
        exit(1);
    }
    if (args[0]->type != tach_object_number) {
        fprintf(stderr, "div only takes numbersn");
        exit(1);
    }
    tach_number *ret = tach_number_copy(args[0]->value.number);
    for (uint32_t i = 1; i < argc; i++) {
        if (args[i]->type == tach_object_number) {
            tach_number_div(ret, args[i]->value.number);
        }
    }
    return tach_object_make_number(ret);
}

tach_object *tach_lib_get(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 1) {
        fprintf(stderr, "get takes 1 arg\n");
        exit(1);
    }
    tach_object *obj = tach_state_get(state, args[0]);
    obj->refc ++;
    return obj;
}

tach_object *tach_lib_call(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 1) {
        fprintf(stderr, "call takes 1 arg\n");
        exit(1);
    }
    tach_call(state, args[0], 0, NULL);
    return NULL;
}

tach_object *tach_lib_proc(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc < 2) {
        fprintf(stderr, "proc takes 2 or more args\n");
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
        fprintf(stderr, "set takes 2 args\n");
        exit(1);
    }
    tach_set_table(state->locals[state->depth-1], args[0], args[1]);
    return tach_object_make_nil();
}

tach_object *tach_lib_upset(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "upset takes 2 args\n");
        exit(1);
    }
    tach_set_table(state->locals[state->depth-2], args[0], args[1]);
    return tach_object_make_nil();
}

tach_object *tach_lib_global(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "global takes 2 args\n");
        exit(1);
    }
    tach_set_table(state->locals[0], args[0], args[1]);
    return tach_object_make_nil();
}

tach_object *tach_lib_if(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "if takes 2 or 3 args\n");
        exit(1);
    }
    bool first = true;
    if (args[0]->type == tach_object_logic && !args[0]->value.logic) {
        first = false;
    }
    else if (args[0]->type == tach_object_nil) {
        first = false;
    }
    if (argc == 2) {
        if (first) {
            tach_call(state, args[1], 0, NULL);
        }
        else {
            return tach_object_make_nil();
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
    return NULL;
}

tach_object *tach_lib_copy(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc == 0) {
        fprintf(stderr, "copy takes 1 or more arguments\n");
        exit(1);
    }
    args[argc-1]->refc ++;
    return args[argc-1];
}