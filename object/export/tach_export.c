#include "tach.h"

long tach_readnum(FILE *f) {
    char len[32];
    uint32_t pl = 0;
    char got = getc(f);
    while (got != ':') {
        len[pl] = got;
        pl ++;
        got = getc(f);
    }
    return atol(len);
}

void tach_export_object_to_file(tach_object *obj, FILE *f) {
    switch (obj->type) {
        case tach_object_other: {
            fprintf(stderr, "cannot export a %s\n", obj->value.other.type->name);
            exit(1);
            break;
        }
        case tach_object_nil: {
            fprintf(f, "#Z");
            break;
        }   
        case tach_object_logical: {
            fprintf(f, obj->value.logical ? "#T" : "#F");
            break;
        }
        case tach_object_number: {
            char *num = tach_number_tostring(obj->value.number);
            fprintf(f, "N%lu:%s", strlen(num), num);
            free(num);
            break;
        }
        case tach_object_string: {
            fprintf(f, "S%u:%s", obj->value.string.count,  obj->value.string.str);
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
}

tach_object *tach_export_file_to_object(FILE *f) {
    char command = getc(f);
    switch (command) {
        case '#': {
            switch (getc(f)) {
                case 'Z': {
                    return tach_object_make_nil();
                }
                case 'T': {
                    return tach_object_make_logical(true);
                }
                case 'F': {
                    return tach_object_make_logical(false);
                }
                case 'N': {
                    return NULL;
                }
                default: {
                    fprintf(stderr, "unknown constant in export to object\n");
                    exit(1);
                }
            }
        }
        case 'V': {
            return tach_object_make_vector(tach_export_file_to_vector(f));
        }
        case 'T': {
            ungetc('T', f);
            return tach_object_make_table(tach_export_file_to_table(f));
        }
        case 'F': {
            return tach_object_make_func(tach_export_file_to_func(f));
        }
        case 'P': {
            tach_object *ret = tach_object_alloc();
            ret->type = tach_object_point;
            ret->value.point = tach_export_file_to_point(f);
            return ret;
        }
        case 'N': {
            char len[32];
            uint32_t pl = 0;
            char got = getc(f);
            while (got != ':') {
                len[pl] = got;
                pl ++;
                got = getc(f);
            }
            len[pl] = '\0';
            long lenl = atol(len);
            char *str = malloc(sizeof(char) * (lenl + 1));
            char strgot = getc(f);
            char strpl = 0;
            while (strpl < lenl) {
                str[strpl] = strgot;
                strpl ++;
                strgot = getc(f);
            }
            ungetc(strgot, f);
            str[strpl] = '\0';
            tach_number *num = tach_create_number_string(str);
            free(str);
            return tach_object_make_number(num);
        }
        case 'S': {
            char len[32];
            uint32_t pl = 0;
            char got = getc(f);
            while (got != ':') {
                len[pl] = got;
                pl ++;
                got = getc(f);
            }
            len[pl] = '\0';
            long lenl = atol(len);
            char *str = malloc(sizeof(char) * (lenl + 1));
            char strgot = getc(f);
            char strpl = 0;
            while (strpl < lenl) {
                str[strpl] = strgot;
                strpl ++;
                strgot = getc(f);
            }
            ungetc(strgot, f);
            str[strpl] = '\0';
            tach_string s = tach_create_string(str);
            free(str);
            return tach_object_make_string(s);
        }
        default: {
            fprintf(stderr, "unknown type in export to object (%d)\n", command);
            exit(1);
        }
    }
}

void tach_export_vector_to_file(tach_vector *v, FILE *f) {
    fprintf(f, "V%d:", v->count);
    for (uint32_t i = 0; i < v->count; i++) {
        tach_export_object_to_file(v->objects[i], f);
    }
}

tach_vector *tach_export_file_to_vector(FILE *f) {
    char len[32];
    uint32_t pl = 0;
    char got = getc(f);
    while (got != ':') {
        len[pl] = got;
        pl ++;
        got = getc(f);
    }
    len[pl] = '\0';
    long lenl = atol(len);
    tach_vector *vec = tach_create_vector();
    for (uint32_t i = 0; i < lenl; i++) {
        tach_vector_push(vec, tach_export_file_to_object(f));
    }
    return vec;
}

void tach_export_table_to_file(tach_table *t, FILE *f) {
    fprintf(f, "T");
    tach_export_object_to_file(t->key, f);
    tach_export_object_to_file(t->value, f);
    if (t->left != NULL) {
        tach_export_table_to_file(t->left, f);
    }
    else {
        fprintf(f, "N");
    }
    if (t->right != NULL) {
        tach_export_table_to_file(t->right, f);
    }
    else {
        fprintf(f, "N");
    }
}

tach_table *tach_export_file_to_table(FILE *f) {
    if (getc(f) == 'N') {
        return NULL;
    }
    tach_table *tab = tach_create_table();
    tab->key = tach_export_file_to_object(f);
    tab->value = tach_export_file_to_object(f);
    tab->left = tach_export_file_to_table(f);
    tab->right = tach_export_file_to_table(f);
    return tab;
}

void tach_export_program_to_file(tach_program *prog, FILE *f) {
    fprintf(f, "%d:", prog->opcount);
    for (uint32_t i = 0; i < prog->opcount; i++) {
        fprintf(f, "%d:%d:", prog->opcodes[i].type ,prog->opcodes[i].value);
    }
    fprintf(f, "%d:", prog->objcount);
    for (uint32_t i = 0; i < prog->objcount; i++) {
        tach_export_object_to_file(prog->objs[i], f);
    }
}

tach_program *tach_export_file_to_program(FILE *f) {
    tach_program *ret = malloc(sizeof(tach_program));
    ret->opcount = tach_readnum(f);
    ret->opalloc = ret->opalloc;
    ret->opcodes = malloc(sizeof(tach_opcode) * ret->opalloc);
    for (uint32_t i = 0; i < ret->opcount; i++) {
        ret->opcodes[i].type = tach_readnum(f);
        ret->opcodes[i].value = tach_readnum(f);
    }
    ret->objcount = tach_readnum(f);
    ret->objalloc = ret->objcount;
    ret->objs = malloc(sizeof(tach_object *) * ret->objcount);
    for (uint32_t i = 0; i < ret->objcount; i++) {
        ret->objs[i] = tach_export_file_to_object(f);
    }
    return ret;
}

void tach_export_func_to_file(tach_func func, FILE *f) {
    char *name = tach_func_to_name(func);
    if (strlen(name) == 0) {
        fprintf(stderr, "unknown function");
        exit(1);
    }
    fprintf(f, "F%lu:%s", strlen(name), name);
}

tach_func tach_export_file_to_func(FILE *f) {
    long lenl = tach_readnum(f);
    char *str = malloc(sizeof(char) * (lenl + 1));
    char strgot = getc(f);
    char strpl = 0;
    while (strpl < lenl) {
        str[strpl] = strgot;
        strpl ++;
        strgot = getc(f);
    }
    ungetc(strgot, f);
    str[strpl] = '\0';
    tach_func ret = tach_name_to_func(str);
    free(str);
    return ret;
}

void tach_export_point_to_file(tach_point point, FILE *f) {
    fprintf(f, "P");
    fprintf(f, "%d:", point.point);
    fprintf(f, "%d:", point.argc);
    for (uint32_t i = 0; i < point.argc; i++) {
        tach_export_object_to_file(point.args[i], f);
    }
}

tach_point tach_export_file_to_point(FILE *f) {
    char pt[32];
    char argc[32];
    char got = getc(f);
    uint32_t pl = 0;
    while (got != ':') {
        pt[pl] = got;
        pl ++;
        got = getc(f);
    }
    pt[got] = '\0';
    got = getc(f);
    pl = 0;
    while (got != ':') {
        argc[pl] = got;
        pl ++;
        got = getc(f);
    }
    argc[got] = '\0';
    tach_point ret;
    ret.argc = atol(argc);
    ret.point = atol(pt);
    ret.args = malloc(sizeof(tach_object *) * ret.argc);
    for (uint32_t i = 0; i < ret.argc; i++) {
        ret.args[i] = tach_export_file_to_object(f);
    }
    return ret;
}