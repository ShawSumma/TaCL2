#include "tach.h"

void tach_free_object(tach_object *obj) {
    // printf("%p\n", obj);
    obj->refc --;
    if (obj->refc != 0) {
        return;
    }
    switch (obj->type) {
        case tach_object_string: {
            free(obj->value.string.str);
            break;
        }
        case tach_object_vector: {
            tach_free_vector(obj->value.vector);
            break;
        }
        case tach_object_point: {
            for (uint32_t i = 0; i < obj->value.point.argc; i++) {
                tach_free_object(obj->value.point.args[i]);
            }
            if (obj->value.point.args != NULL) {
                free(obj->value.point.args);
            }
            break;
        }
        case tach_object_number: {
            tach_free_number(obj->value.number);
            break;
        }
        default: {
            break;
        }
    }
    free(obj);
}

void tach_free_number(tach_number *num) {
    tach_number_clear(num);
    free(num);
}

void tach_free_vector(tach_vector *vec) {
    for (uint32_t i = 0; i < vec->count; i++) {
        tach_free_object(vec->objects[i]);
    }
    free(vec->objects);
    free(vec);
}

void tach_free_state(tach_state *state) {
    for (uint32_t i = 0; i < state->depth; i++) {
        tach_free_table(state->locals[i]);
    }
    tach_free_vector(state->stack);
    free(state->locals);
    free(state->calls);
    free(state);
}

void tach_free_program(tach_program *prog) {
    free(prog->opcodes);
    for (uint32_t i = 0; i < prog->objcount; i++) {
        tach_free_object(prog->objs[i]);
    }
    free(prog->objs);
    free(prog);
}

void tach_free_table(tach_table *table) {
    if (table->key != NULL) {
        tach_free_object(table->key);
        tach_free_object(table->value);
    }
    if (table->left != NULL) {
        tach_free_table(table->left);
    }
    if (table->right != NULL) {
        tach_free_table(table->right);
    }
    free(table);
}

