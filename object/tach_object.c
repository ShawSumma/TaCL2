#include <tach.h>

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

tach_object *tach_object_make_logic(bool b) {
    tach_object *ret = tach_object_alloc();
    ret->type = tach_object_logic;
    ret->value.logic = b;
    return ret;
}

tach_object *tach_object_make_number(tach_number *n) {
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

tach_object *tach_object_make_table(tach_table *t) {
    tach_object *ret = tach_object_alloc();
    ret->type = tach_object_table;
    ret->value.table = t;
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
        vec->objects = realloc(vec->objects, sizeof(tach_object *) * vec->alloc);
    }
    vec->objects[vec->count] = obj;
    vec->count ++;
}

void tach_vector_reserve(tach_vector *vec, uint32_t size) {
    vec->objects = realloc(vec->objects, sizeof(tach_object *) * vec->alloc);
}

void tach_set_vector(tach_vector *vec, uint32_t place, tach_object *obj) {
    obj->refc ++;
    tach_object *old = vec->objects[place];
    vec->objects[place] = obj;
    tach_free_object(old);
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

tach_object *tach_object_copy(tach_object *obj) {
    switch (obj->type) {
        case tach_object_nil: {
            return tach_object_make_nil();
        }
        case tach_object_logic: {
            return tach_object_make_logic(obj->value.logic);
        }
        case tach_object_number: {
            return tach_object_make_number(tach_number_copy(obj->value.number));
        }
        case tach_object_point: {
            tach_object *ret = tach_object_alloc();
            ret->type = tach_object_point;
            ret->value.point.argc = obj->value.point.argc;
            ret->value.point.point = obj->value.point.point;
            ret->value.point.args = malloc(sizeof(tach_object *) * ret->value.point.argc);
            for (uint32_t i = 0; i < ret->value.point.argc; i++) {
                ret->value.point.args[i] = tach_object_copy(obj->value.point.args[i]);
            }
            return ret;
        }
        case tach_object_func: {
            return tach_object_make_func(obj->value.func);
        }
        case tach_object_string: {
            return tach_object_make_string(tach_string_copy(obj->value.string));
        }
        case tach_object_vector: {
            return tach_object_make_vector(tach_vector_copy(obj->value.vector));
        }
        case tach_object_table: {
            return tach_object_make_table(tach_table_copy(obj->value.table));
        }
    }
}

tach_vector *tach_vector_copy(tach_vector *vec) {
    tach_vector *ret = tach_create_vector();
    ret->alloc = vec->alloc;
    ret->count = vec->count;
    ret->objects = malloc(sizeof(tach_object *) * ret->alloc);
    for (uint32_t i = 0; i < vec->count; i++) {
        ret->objects[i] = tach_object_copy(vec->objects[i]);
    }
    return ret;
}

tach_table *tach_table_copy(tach_table *tab) {
    if (tab == NULL) {
        return NULL;
    }
    tach_table *ret = tach_create_table();
    if (tab->key == NULL) {
        ret->key = NULL;
    }
    else {
        ret->key = tach_object_copy(tab->key);
    }
    if (tab->value == NULL) {
        ret->value = NULL;
    }
    else {
        ret->value = tach_object_copy(tab->value);
    }
    ret->left = tach_table_copy(tab->left);
    ret->right = tach_table_copy(tab->right);
    return ret;
}

tach_string tach_string_copy(tach_string str) {
    tach_string ret = tach_create_string(str.str);
    return ret;
}

void tach_table_merge_into(tach_table *to, tach_table *from) {
    if (from == NULL) {
        return;
    }
    tach_set_table(to, from->key, from->value);
    tach_table_merge_into(to, from->left);
    tach_table_merge_into(to, from->right);
}