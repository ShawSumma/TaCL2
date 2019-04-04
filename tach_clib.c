#include "tach.h"

char tach_clib_compare(tach_object *a, tach_object *b) {
    if (a->type < b->type) {
        return -1;
    }
    if (a->type > b->type) {
        return 0;
    }
    switch (a->type) {
        case tach_object_logical: {
            if (a->value.logical < b->value.logical) {
                return -1;
            }
            if (a->value.logical > b->value.logical) {
                return 1;
            }
            return 0;
        }
        case tach_object_number: {
            if (a->value.number < b->value.number) {
                return -1;
            }
            if (a->value.number > b->value.number) {
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
        default: {
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
        case tach_object_vector:
            return tach_create_string("(vector)");
        case tach_object_logical:
            return tach_create_string(obj->value.logical ? "true" : "false");
        case tach_object_string:
            return tach_create_string(obj->value.string.str);
        case tach_object_number: {
            char *str;
            uint32_t count = snprintf(str, 0, "%lf", obj->value.number);
            str = malloc(sizeof(char) * count);
            snprintf(str, count, "%lf", obj->value.number);
            tach_string tstr = tach_create_string(str);
            free(str);
            return tstr;
        }
    }
}