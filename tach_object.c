#include "tach.h"

tach_object *tach_object_alloc() {
    tach_object *ret = malloc(sizeof(tach_object));
    ret->refc = 1;
    return ret;
}

tach_object *tach_object_make_nil() {
    tach_object *ret = tach_object_alloc();
    ret->type = tach_object_nil;
    return ret;
}

tach_object *tach_object_make_logical(bool b) {
    tach_object *ret = tach_object_alloc();
    ret->type = tach_object_logical;
    ret->value.logical = b;
    return ret;
}

tach_object *tach_object_make_number(double n) {
    tach_object *ret = tach_object_alloc();
    ret->type = tach_object_number;
    ret->value.number = n;
    return ret;
}

tach_object *tach_object_make_point(uint32_t p) {
    tach_object *ret = tach_object_alloc();
    ret->type = tach_object_point;
    ret->value.point.argc = 0;
    ret->value.point.args = NULL;
    ret->value.point.point = p;
    return ret;
}

tach_object *tach_object_make_string(tach_string s) {
    tach_object *ret = tach_object_alloc();
    ret->type = tach_object_string;
    ret->value.string = s;
    return ret;
}

tach_object *tach_object_make_func(tach_func f) {
    tach_object *ret = tach_object_alloc();
    ret->type = tach_object_func;
    ret->value.func = f;
    return ret;
}

tach_object *tach_object_make_vector(tach_vector *v) {
    tach_object *ret = tach_object_alloc();
    ret->type = tach_object_vector;
    ret->value.vector = v;
    return ret;
}

tach_string tach_create_string(char *str) {
    tach_string ret;
    ret.count = strlen(str);
    ret.alloc = ret.count + 1;
    ret.str = malloc(sizeof(char) * ret.alloc);
    for (uint32_t i = 0; i < ret.count; i++) {
        ret.str[i] = str[i];
    }
    ret.str[ret.count] = '\0';
    return ret;
}

tach_vector *tach_create_vector() {
    tach_vector *ret = malloc(sizeof(tach_vector));
    ret->count = 0;
    ret->alloc = 8;
    ret->objects = malloc(sizeof(tach_object *) * ret->alloc);
    return ret;
}

void tach_vector_push(tach_vector *vec, tach_object *obj) {
    obj->refc ++;
    if (vec->count + 4 > vec->alloc) {
        vec->alloc *= 1.5;
        vec->objects = realloc(vec->objects, sizeof(tach_object ) * vec->alloc);
    }
    vec->objects[vec->count] = obj;
    vec->count ++;
}

void tach_vector_reserve(tach_vector *vec, uint32_t size) {
    vec->objects = realloc(vec->objects, sizeof(tach_object *) * vec->alloc);
}

void tach_set_vector(tach_vector *vec, uint32_t place, tach_object *obj) {
    obj->refc ++;
    tach_free_object(vec->objects[place]);
    vec->objects[place] = obj;
}

tach_object *tach_get_vector(tach_vector *vec, uint32_t place) {
    return vec->objects[place];
}

tach_object *tach_vector_last(tach_vector *vec) {
    return vec->objects[vec->count-1];
}

tach_object *tach_vector_pop(tach_vector *vec) {
    tach_object *obj = vec->objects[vec->count-1];
    tach_free_object(obj);
    vec->count --;
    return obj;
}


tach_table *tach_create_table() {
    tach_table *ret = malloc(sizeof(tach_table));
    ret->right = NULL;
    ret->left = NULL;
    ret->key = NULL;
    ret->value = NULL;
    return ret;
}

tach_object *tach_get_table(tach_table *table, tach_object *object) {
    while (true) {
        if (table == NULL) {
            return NULL;
        }
        if (table->key == NULL) {
            return NULL;
        }
        char cmp = tach_clib_compare(object, table->key);
        switch (cmp) {
            case -1: {
                table = table->left;
                break;
            }
            case 0: {
                return table->value;
            }
            case 1: {
                table = table->right;
                break;
            }
        }
    }
    return NULL;
}

void tach_set_table(tach_table *table, tach_object *key, tach_object *value) {
    while (true) {
        if (table->key == NULL) {
            key->refc ++;
            value->refc ++;
            table->key = key;
            table->value = value;
            return;
        }
        char cmp = tach_clib_compare(key, table->key);
        switch (cmp) {
            case -1:
                if (table->left != NULL) {
                    table = table->left;
                }
                else {
                    table->left = tach_create_table();
                    tach_set_table(table->left, key, value);
                    return;
                }
                break;
            case 0: {
                if (table->value != NULL) {
                    tach_free_object(table->value);
                }
                value->refc ++;
                table->value = value;    
                return;            
            }
            case 1: {
                if (table->right != NULL) {
                    table = table->right;
                }
                else {
                    table->right = tach_create_table();
                    tach_set_table(table->right, key, value);
                    return;
                }
                break;
            }
        }
    }
}
