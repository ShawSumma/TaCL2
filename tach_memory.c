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
        default: {
            break;
        }
    }
    free(obj);
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

void tach_ast_free_proc(tach_ast_proc *p) {
    for (uint32_t i = 0; i < p->count; i++) {
        tach_ast_free_command(p->commands[i]);
    }
    free(p->commands);
    free(p);
}

void tach_ast_free_command(tach_ast_command *p) {
    for (uint32_t i = 0; i < p->count; i++) {
        tach_ast_free_single(p->singles[i]);
    }
    free(p->singles);
    free(p);
}

void tach_ast_free_single(tach_ast_single *p) {
    switch (p->type) {
        case tach_ast_single_command: {
            tach_ast_free_command(p->value.command);
            break;
        }
        case tach_ast_single_proc: {
            tach_ast_free_proc(p->value.proc);
            break;
        }
        case tach_ast_single_name: {
            tach_ast_free_name(p->value.name);
            break;
        }
        case tach_ast_single_string: {
            tach_ast_free_name(p->value.string);
            break;
        }
        default: {
            break;
        }
    }
    free(p);
}

void tach_ast_free_name(char *p) {
    free(p);
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

