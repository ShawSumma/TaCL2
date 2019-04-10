#include "tach.h"

void tach_export_object_to_file(tach_object *obj, FILE *f) {
    switch (obj->type) {
        case tach_object_func: {
            fprintf(stderr, "cannot export a func\n");
            exit(1);
            break;
        }
        case tach_object_point: {
            fprintf(stderr, "cannot export a point\n");
            exit(1);
            break;
        }
        case tach_object_other: {
            fprintf(stderr, "cannot export a %s\n", obj->value.other.type);
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
        case tach_object_vector: {
            tach_export_vector_to_file(obj->value.vector, f);
            break;
        }
        case tach_object_table: {
            tach_export_table_to_file(obj->value.table, f);
            break;
        }
    }
}

void tach_export_vector_to_file(tach_vector *v, FILE *f) {
    fprintf(f, "V%d:", v->count);
    for (uint32_t i = 0; i < v->count; i++) {
        tach_export_object_to_file(v->objects[i], f);
    }
}

void tach_export_table_to_file(tach_table *t, FILE *f) {
    fprintf(f, "T");
    tach_export_object_to_file(t->key, f);
    tach_export_object_to_file(t->value, f);
    if (t->left != NULL) {
        tach_export_table_to_file(t->left, f);
    }
    else {
        fprintf(f, "#N");
    }
    if (t->right != NULL) {
        tach_export_table_to_file(t->right, f);
    }
    else {
        fprintf(f, "#N");
    }
}