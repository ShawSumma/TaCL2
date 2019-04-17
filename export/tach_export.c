#include "tach.h"

long tach_readnum(tach_file *f) {
    long n = 0;
    char got = tach_getc(f);
    while (got != ':') {
        n *= 10;
        n += got - '0';
        got = tach_getc(f);
    }
    return n;
}

void tach_export_object_to_file(tach_object *obj, tach_file *f) {
    switch (obj->type) {
        case tach_object_other: {
            fprintf(stderr, "cannot export an object of type %s\n", obj->value.other.type->name);
            exit(1);
            break;
        }
        case tach_object_nil: {
            tach_fprintf(f, "#Z");
            break;
        }   
        case tach_object_logic: {
            tach_fprintf(f, obj->value.logic ? "#T" : "#F");
            break;
        }
        case tach_object_number: {
            char *num = tach_number_tostring(obj->value.number);
            tach_fprintf(f, "N%lu:%s", strlen(num), num);
            free(num);
            break;
        }
        case tach_object_string: {
            tach_fprintf(f, "S%u:%s", obj->value.string.count,  obj->value.string.str);
            break;
        }
        case tach_object_func: {
            tach_export_func_to_file(obj->value.func, f);
            break;
        }
        case tach_object_vector: {
            tach_export_vector_to_file(obj->value.vector, f);
            break;
        }
        case tach_object_table: {
            tach_export_table_to_file(obj->value.table, f);
            break;
        }
        case tach_object_point: {
            tach_export_point_to_file(obj->value.point, f);            
            break;
        }
    }
    tach_fprintf(f, "%d:", obj->refc);
}

tach_object *tach_export_file_to_object(tach_file *f) {
    char command = tach_getc(f);
    tach_object *ret;
    switch (command) {
        case '#': {
            switch (tach_getc(f)) {
                case 'Z': {
                    ret = tach_object_make_nil();
                    break;
                }
                case 'T': {
                    ret = tach_object_make_logic(true);
                    break;
                }
                case 'F': {
                    ret = tach_object_make_logic(false);
                    break;
                }
                case 'N': {
                    return NULL;
                }
                default: {
                    fprintf(stderr, "unknown constant in export to object\n");
                    exit(1);
                }
            }
            break;
        }
        case 'V': {
            ret = tach_object_make_vector(tach_export_file_to_vector(f));
            break;
        }
        case 'T': {
            tach_ungetc('T', f);
            ret = tach_object_make_table(tach_export_file_to_table(f));
            break;
        }
        case 'F': {
            ret = tach_object_make_func(tach_export_file_to_func(f));
            break;
        }
        case 'P': {
            ret = tach_object_alloc();
            ret->type = tach_object_point;
            ret->value.point = tach_export_file_to_point(f);
            break;
        }
        case 'N': {
            long lenl = tach_readnum(f);
            char *str = malloc(sizeof(char) * (lenl + 1));
            char got = tach_getc(f);
            uint32_t strpl = 0;
            while (strpl < lenl) {
                str[strpl] = got;
                strpl ++;
                got = tach_getc(f);
            }
            tach_ungetc(got, f);
            str[strpl] = '\0';
            tach_number *num = tach_create_number_string(str);
            free(str);
            ret = tach_object_make_number(num);
            break;
        }
        case 'S': {
            char len[32];
            uint32_t pl = 0;
            char got = tach_getc(f);
            while (got != ':') {
                len[pl] = got;
                pl ++;
                got = tach_getc(f);
            }
            len[pl] = '\0';
            long lenl = atol(len);
            char *str = malloc(sizeof(char) * (lenl + 1));
            char strgot = tach_getc(f);
            char strpl = 0;
            while (strpl < lenl) {
                str[strpl] = strgot;
                strpl ++;
                strgot = tach_getc(f);
            }
            tach_ungetc(strgot, f);
            str[strpl] = '\0';
            tach_string s = tach_create_string(str);
            free(str);
            ret = tach_object_make_string(s);
            break;
        }
        default: {
            fprintf(stderr, "unknown type in export to object (%d)\n", command);
            exit(1);
        }
    }
    ret->refc = tach_readnum(f) + 1000;
    return ret;
}

void tach_export_vector_to_file(tach_vector *v, tach_file *f) {
    tach_fprintf(f, "V%d:", v->count);
    for (uint32_t i = 0; i < v->count; i++) {
        tach_export_object_to_file(v->objects[i], f);
    }
}

tach_vector *tach_export_file_to_vector(tach_file *f) {
    tach_getc(f);
    long lenl = tach_readnum(f);
    tach_vector *vec = tach_create_vector();
    for (uint32_t i = 0; i < lenl; i++) {
        tach_vector_push(vec, tach_export_file_to_object(f));
    }
    return vec;
}



void tach_export_table_to_file(tach_table *t, tach_file *f) {
    tach_fprintf(f, "T");
    if (t->key != NULL && t->value != NULL) {
        tach_export_object_to_file(t->key, f);
        tach_export_object_to_file(t->value, f);
    }
    else {
        tach_fprintf(f, "#N");
        tach_fprintf(f, "#N");
    }
    if (t->left != NULL) {
        tach_export_table_to_file(t->left, f);
    }
    else {
        tach_fprintf(f, "N");
    }
    if (t->right != NULL) {
        tach_export_table_to_file(t->right, f);
    }
    else {
        tach_fprintf(f, "N");
    }
}

tach_table *tach_export_file_to_table(tach_file *f) {
    if (tach_getc(f) == 'N') {
        return NULL;
    }
    tach_table *tab = tach_create_table();
    tab->key = tach_export_file_to_object(f);
    tab->value = tach_export_file_to_object(f);
    tab->left = tach_export_file_to_table(f);
    tab->right = tach_export_file_to_table(f);
    return tab;
}

void tach_export_program_to_file(tach_program *prog, tach_file *f) {
    tach_fprintf(f, "%d:", prog->opcount);
    for (uint32_t i = 0; i < prog->opcount; i++) {
        tach_fprintf(f, "%d:%d:", prog->opcodes[i].type, prog->opcodes[i].value);
    }
    tach_fprintf(f, "%d:", prog->objcount);
    for (uint32_t i = 0; i < prog->objcount; i++) {
        tach_export_object_to_file(prog->objs[i], f);
    }
}

tach_program *tach_export_file_to_program(tach_file *f) {
    tach_program *ret = malloc(sizeof(tach_program));
    ret->opcount = tach_readnum(f);
    ret->opalloc = ret->opcount;
    ret->opcodes = malloc(sizeof(tach_opcode) * ret->opalloc);
    for (uint32_t i = 0; i < ret->opcount; i++) {
        ret->opcodes[i].type = tach_readnum(f);
        ret->opcodes[i].value = tach_readnum(f);
    }
    ret->objcount = tach_readnum(f);
    ret->objalloc = ret->objcount;
    ret->objs = malloc(sizeof(tach_object *) * ret->objcount);
    char got = tach_getc(f);
    tach_ungetc(got, f);
    for (uint32_t i = 0; i < ret->objcount; i++) {
        ret->objs[i] = tach_export_file_to_object(f);
    }
    return ret;
}

void tach_export_func_to_file(tach_func func, tach_file *f) {
    char *name = tach_func_to_name(func);
    if (strlen(name) == 0) {
        fprintf(stderr, "unknown function\n");
        exit(1);
    }
    tach_fprintf(f, "F%lu:%s", strlen(name), name);
}

tach_func tach_export_file_to_func(tach_file *f) {
    long lenl = tach_readnum(f);
    char *str = malloc(sizeof(char) * (lenl + 1));
    char strgot = tach_getc(f);
    char strpl = 0;
    while (strpl < lenl) {
        str[strpl] = strgot;
        strpl ++;
        strgot = tach_getc(f);
    }
    tach_ungetc(strgot, f);
    str[strpl] = '\0';
    tach_func ret = tach_name_to_func(str);
    free(str);
    return ret;
}



void tach_export_point_to_file(tach_point point, tach_file *f) {
    tach_fprintf(f, "P");
    tach_fprintf(f, "%d:", point.point);
    tach_fprintf(f, "%d:", point.argc);
    for (uint32_t i = 0; i < point.argc; i++) {
        tach_export_object_to_file(point.args[i], f);
    }
}

tach_point tach_export_file_to_point(tach_file *f) {
    tach_point ret;
    ret.point = tach_readnum(f);
    ret.argc = tach_readnum(f);
    ret.args = malloc(sizeof(tach_object *) * ret.argc);
    for (uint32_t i = 0; i < ret.argc; i++) {
        ret.args[i] = tach_export_file_to_object(f);
    }
    return ret;
}

void tach_export_state_to_file(tach_state *prog, tach_file *f) {
    tach_fprintf(f, "%d:", prog->place);
    tach_fprintf(f, "%d:", prog->depth);
    for (uint32_t i = 0; i < prog->depth; i++) {
        tach_fprintf(f, "%d:", prog->calls[i]);
    }
    for (uint32_t i = 0; i < prog->depth; i++) {
        tach_export_table_to_file(prog->locals[i], f);
    }
    tach_export_vector_to_file(prog->stack, f);
}

tach_state *tach_export_file_to_state(tach_file *f) {
    tach_state *ret = malloc(sizeof(tach_state));
    ret->place = tach_readnum(f);
    ret->depth = tach_readnum(f);
    ret->callalloc = ret->depth + 8;
    ret->calls = malloc(sizeof(uint32_t) * ret->callalloc);
    for (uint32_t i = 0; i < ret->depth; i++) {
        ret->calls[i] = tach_readnum(f);
    }
    ret->locals = malloc(sizeof(tach_table *) * ret->callalloc);
    for (uint32_t i = 0; i < ret->depth; i++) {
        ret->locals[i] = tach_export_file_to_table(f);
    }
    ret->stack = tach_export_file_to_vector(f);
    return ret;
}