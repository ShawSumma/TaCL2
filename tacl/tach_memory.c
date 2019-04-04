#include "tach_memory.h"

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