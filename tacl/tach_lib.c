#include "tach.h"

char *tach_func_to_name(tach_func fn) {
    if (fn == tach_lib_export) {
        return "export";
    }
    if (fn == tach_lib_import) {
        return "import";
    }
    if (fn == tach_lib_len) {
        return "len";
    }
    if (fn == tach_lib_str) {
        return "str";
    }
    if (fn == tach_lib_print) {
        return "print";
    }
    if (fn == tach_lib_add) {
        return "add";
    }
    if (fn == tach_lib_mul) {
        return "mul";
    }
    if (fn == tach_lib_sub) {
        return "sub";
    }
    if (fn == tach_lib_div) {
        return "div";
    }
    if (fn == tach_lib_lt) {
        return "lt";
    }
    if (fn == tach_lib_lte) {
        return "lte";
    }
    if (fn == tach_lib_gt) {
        return "gt";
    }
    if (fn == tach_lib_gte) {
        return "gte";
    }
    if (fn == tach_lib_eq) {
        return "eq";
    }
    if (fn == tach_lib_neq) {
        return "neq";
    }
    if (fn == tach_lib_if) {
        return "if";
    }
    if (fn == tach_lib_copy) {
        return "copy";
    }
    if (fn == tach_lib_call) {
        return "call";
    }
    if (fn == tach_lib_proc) {
        return "proc";
    }
    if (fn == tach_lib_set) {
        return "set";
    }
    if (fn == tach_lib_upset) {
        return "upset";
    }
    if (fn == tach_lib_global) {
        return "global";
    }
    if (fn == tach_lib_get) {
        return "get";
    }
    if (fn == tach_lib_new_table) {
        return "new_table";
    }
    if (fn == tach_lib_new_vector) {
        return "new_vector";
    }
    if (fn == tach_lib_save_state) {
        return "save-state";
    }
    if (fn == tach_lib_load_state) {
        return "load-state";
    }
    return "";
}

tach_func tach_name_to_func(char *str) {
    if (!strcmp(str, "export")) {
        return tach_lib_export;
    }
    if (!strcmp(str, "import")) {
        return tach_lib_import;
    }
    if (!strcmp(str, "len")) {
        return tach_lib_len;
    }
    if (!strcmp(str, "str")) {
        return tach_lib_str;
    }
    if (!strcmp(str, "print")) {
        return tach_lib_print;
    }
    if (!strcmp(str, "add")) {
        return tach_lib_add;
    }
    if (!strcmp(str, "mul")) {
        return tach_lib_mul;
    }
    if (!strcmp(str, "sub")) {
        return tach_lib_sub;
    }
    if (!strcmp(str, "div")) {
        return tach_lib_div;
    }
    if (!strcmp(str, "lt")) {
        return tach_lib_lt;
    }
    if (!strcmp(str, "lte")) {
        return tach_lib_lte;
    }
    if (!strcmp(str, "gt")) {
        return tach_lib_gt;
    }
    if (!strcmp(str, "gte")) {
        return tach_lib_gte;
    }
    if (!strcmp(str, "eq")) {
        return tach_lib_eq;
    }
    if (!strcmp(str, "neq")) {
        return tach_lib_neq;
    }
    if (!strcmp(str, "if")) {
        return tach_lib_if;
    }
    if (!strcmp(str, "copy")) {
        return tach_lib_copy;
    }
    if (!strcmp(str, "call")) {
        return tach_lib_call;
    }
    if (!strcmp(str, "proc")) {
        return tach_lib_proc;
    }
    if (!strcmp(str, "set")) {
        return tach_lib_set;
    }
    if (!strcmp(str, "upset")) {
        return tach_lib_upset;
    }
    if (!strcmp(str, "global")) {
        return tach_lib_global;
    }
    if (!strcmp(str, "get")) {
        return tach_lib_get;
    }
    if (!strcmp(str, "new_table")) {
        return tach_lib_new_table;
    }
    if (!strcmp(str, "new_vector")) {
        return tach_lib_new_vector;
    }
    if (!strcmp(str, "save-state")) {
        return tach_lib_save_state;
    }
    if (!strcmp(str, "load-state")) {
        return tach_lib_load_state;
    }
    return NULL;
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

    tach_create_state_regester(ret->locals[0], "export", tach_object_make_func(tach_lib_export));
    tach_create_state_regester(ret->locals[0], "import", tach_object_make_func(tach_lib_import));
    tach_create_state_regester(ret->locals[0], "save-state", tach_object_make_func(tach_lib_save_state));
    tach_create_state_regester(ret->locals[0], "load-state", tach_object_make_func(tach_lib_load_state));
    
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

    tach_create_state_regester(ret->locals[0], "true", tach_object_make_logical(true));
    tach_create_state_regester(ret->locals[0], "false", tach_object_make_logical(false));

    tach_table *newlocal = tach_create_table();
    tach_create_state_regester(newlocal, "table", tach_object_make_func(tach_lib_new_table));
    tach_create_state_regester(newlocal, "vector", tach_object_make_func(tach_lib_new_vector));

    tach_create_state_regester(ret->locals[0], "new", tach_object_make_table(newlocal));

    return ret;
}

tach_object *tach_lib_save_state(tach_state *state, uint32_t argc, tach_object **args) {
    FILE *f = fopen(args[0]->value.string.str, "wb");
    tach_export_program_to_file(state->program, f);
    fclose(f);
    return tach_object_make_nil();
}

tach_object *tach_lib_load_state(tach_state *state, uint32_t argc, tach_object **args) {
    FILE *f = fopen(args[0]->value.string.str, "rb");
    tach_program *prog = tach_export_file_to_program(f);
    tach_free_program(prog);
    fclose(f);
    return tach_object_make_nil();
}

tach_object *tach_lib_new_vector(tach_state *state, uint32_t argc, tach_object **args) {
    tach_vector *vec = tach_create_vector();
    for (uint32_t i = 0; i < argc; i++) {
        tach_vector_push(vec, args[i]);
    }
    return tach_object_make_vector(vec);
}

tach_object *tach_lib_new_table(tach_state *state, uint32_t argc, tach_object **args) {
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
        return NULL;
    }
    return tach_object_make_number(tach_create_number(args[0]->value.string.count));
}

tach_object *tach_lib_export(tach_state *state, uint32_t argc, tach_object **args) {
    FILE *f = fopen(args[0]->value.string.str, "wb");
    for (uint32_t i = 1; i < argc; i++) {
        tach_export_object_to_file(args[i], f);
    }
    fclose(f);
    return tach_object_make_nil();
}

tach_object *tach_lib_import(tach_state *state, uint32_t argc, tach_object **args) {
    FILE *f = fopen(args[0]->value.string.str, "rb");
    tach_object *ret = tach_export_file_to_object(f);
    fclose(f);
    return ret;
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
        return NULL;
    }
    return tach_object_make_logical(tach_clib_compare(args[0], args[1]) == -1);
}

tach_object *tach_lib_gt(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "gt takes 2 args\n");
        return NULL;
    }
    return tach_object_make_logical(tach_clib_compare(args[0], args[1]) == 1);
}

tach_object *tach_lib_gte(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "gte takes 2 args\n");
        return NULL;
    }
    return tach_object_make_logical(tach_clib_compare(args[0], args[1]) != -1);
}

tach_object *tach_lib_lte(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "lte takes 2 args\n");
        return NULL;
    }
    return tach_object_make_logical(tach_clib_compare(args[0], args[1]) != 1);
}

tach_object *tach_lib_eq(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "gq takes 2 args\n");
        return NULL;
    }
    return tach_object_make_logical(tach_clib_compare(args[0], args[1]) == 0);
}

tach_object *tach_lib_neq(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "neq takes 2 args\n");
        return NULL;
    }
    return tach_object_make_logical(tach_clib_compare(args[0], args[1]) != 0);
}

tach_object *tach_lib_add(tach_state *state, uint32_t argc, tach_object **args) {
    tach_number *ret = tach_create_number(0);
    for (uint32_t i = 0; i < argc; i++) {
        if (args[i]->type == tach_object_number) {
            tach_number_add(ret, args[i]->value.number);
        }
        else {
            fprintf(stderr, "add only takes numbers\n");
            return NULL;
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
            return NULL;
        }
    }
    return tach_object_make_number(ret);
}

tach_object *tach_lib_sub(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc < 1) {
        fprintf(stderr, "sub takes 1 or more args\n");
        return NULL;
    }
    if (args[0]->type != tach_object_number) {
        fprintf(stderr, "sub only takes numbers\n");
        return NULL;
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
        return NULL;
    }
    if (args[0]->type != tach_object_number) {
        fprintf(stderr, "div only takes numbersn");
        return NULL;
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
        return NULL;
    }
    tach_object *obj = tach_state_get(state, args[0]);
    obj->refc ++;
    return obj;
}

tach_object *tach_lib_call(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 1) {
        fprintf(stderr, "call takes 1 arg\n");
        return NULL;
    }
    tach_call(state, args[0], 0, NULL);
    // return tach_object_make_nil();
    return NULL;
}

tach_object *tach_lib_proc(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc < 2) {
        fprintf(stderr, "proc takes 2 or more args\n");
        return NULL;
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
        return NULL;
    }
    tach_set_table(state->locals[state->depth-1], args[0], args[1]);
    return tach_object_make_nil();
}

tach_object *tach_lib_upset(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "upset takes 2 args\n");
        return NULL;
    }
    tach_set_table(state->locals[state->depth-2], args[0], args[1]);
    return tach_object_make_nil();
}

tach_object *tach_lib_global(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2) {
        fprintf(stderr, "global takes 2 args\n");
        return NULL;
    }
    tach_set_table(state->locals[0], args[0], args[1]);
    return tach_object_make_nil();
}

tach_object *tach_lib_if(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "if takes 2 or 3 args\n");
        return NULL;
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
    return NULL;
}

tach_object *tach_lib_copy(tach_state *state, uint32_t argc, tach_object **args) {
    if (argc == 0) {
        fprintf(stderr, "copy takes 1 or more arguments\n");
        return NULL;
    }
    args[argc-1]->refc ++;
    return args[argc-1];
}


