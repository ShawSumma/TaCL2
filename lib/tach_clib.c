#include <tach.h>

char *tach_clib_type_name(uint32_t id) {
    switch (id) {
        case tach_object_logic: return "logic";
        case tach_object_nil: return "nil";
        case tach_object_number: return "number";
        case tach_object_point: return "point";
        case tach_object_func: return "func";
        case tach_object_string: return "string";
        case tach_object_vector: return "vector";
        case tach_object_table: return "table";
    }
    return "unknown";
}

char tach_clib_compare(tach_object *a, tach_object *b) {
    if (a->type < b->type) {
        return -1;
    }
    if (a->type > b->type) {
        return 1;
    }
    switch (a->type) {
        case tach_object_logic: {
            if (a->value.logic < b->value.logic) {
                return -1;
            }
            if (a->value.logic > b->value.logic) {
                return 1;
            }
            return 0;
        }
        case tach_object_number: {
            if (tach_number_lt(a->value.number, b->value.number)) {
                return -1;
            }
            if (tach_number_gt(a->value.number, b->value.number)) {
                return 1;
            }
            return 0;
        }
        case tach_object_string: {
            tach_string sa = a->value.string;
            tach_string sb = b->value.string;
            if (sa.count < sb.count) {
                return -1;
            }
            if (sa.count > sb.count) {
                return 1;
            }
            for (uint32_t i = 0; i < sa.count; i++) {
                if (sa.str[i] < sb.str[i]) {
                    return -1;
                }
                if (sa.str[i] > sb.str[i]) {
                    return 1;
                }
            }
            return 0;
        }
        case tach_object_point: {
            if (a->value.point.point < b->value.point.point) {
                return -1;
            }
            if (a->value.point.point > b->value.point.point) {
                return 1;
            }
            return 0;
        }
        case tach_object_func: {
            void *sa = a->value.func.func;
            void *sb = b->value.func.func;
            if (sa < sb) {
                return -1;
            }
            if (sa > sb) {
                return 1;
            }
            return 0;
        }
        case tach_object_vector: {
            tach_vector *va = a->value.vector;
            tach_vector *vb = b->value.vector;
            if (va->count < vb->count) {
                return -1;
            }
            if (va->count > vb->count) {
                return 1;
            }
            for (uint32_t i = 0; i < va->count; i++) {
                char got = tach_clib_compare(va->objects[i], vb->objects[i]);
                if (got != 0) {
                    return got;
                }
            }
            return 0;
        }
        case tach_object_table: {
            fprintf(stderr, "cannot compare tables\n");
            exit(1);
        }
        case tach_object_nil: {
            return 0;
        }
    }
    return 0;
}

tach_string tach_clib_tostring(tach_object *obj) {
    switch (obj->type) {
        case tach_object_func:
            return tach_create_string("(func)");
        case tach_object_nil:
            return tach_create_string("(nil)");
        case tach_object_point:
            return tach_create_string("(proc)");
        case tach_object_vector: {
            tach_vector *vec = obj->value.vector;
            tach_string *strings = malloc(sizeof(tach_string) * vec->count);
            uint32_t size = 3;
            for (uint32_t i = 0; i < vec->count; i++) {
                strings[i] = tach_clib_tostring(vec->objects[i]);
                size += strings[i].count + 1;
            }
            char *str = malloc(sizeof(char) * size);
            str[0] = '<';
            uint32_t pl = 1;
            for (uint32_t i = 0; i < vec->count; i++) {
                if (i != 0) {
                    str[pl] = ' ';
                    pl ++;
                }
                for (uint32_t j = 0; j < strings[i].count; j++, pl++) {
                    str[pl] = strings[i].str[j];
                }
                free(strings[i].str);
            }
            free(strings);
            str[pl] = '>';
            str[pl+1] = '\0';
            tach_string ret = tach_create_string(str);
            free(str);
            return ret;
        }
        case tach_object_table:
            return tach_create_string("(table)");
        case tach_object_logic:
            return tach_create_string(obj->value.logic ? "true" : "false");
        case tach_object_string:
            return tach_create_string(obj->value.string.str);
        case tach_object_number: {
            char *str = tach_number_tostring(obj->value.number);
            tach_string tstr = tach_create_string(str);
            free(str);
            return tstr;
        }
    }
}