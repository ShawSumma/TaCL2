#include <tach.h>

void tach_ast_print_program(tach_file *of, tach_ast_proc *proc) {
    tach_ast_print_proc(of, proc, 0);
}

void tach_ast_print_nspaces(tach_file *of, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        tach_fprintf(of, " ");
    }
}

void tach_ast_print_proc(tach_file *of, tach_ast_proc *proc, uint32_t d) {
    tach_ast_print_nspaces(of, d);
    tach_fprintf(of, "proc: (at %d,%d)\n", proc->lineno, proc->colno);
    for (uint32_t i = 0; i < proc->count; i++) {
        tach_ast_print_command(of, proc->commands[i], d+2);
    }
}

void tach_ast_print_command(tach_file *of, tach_ast_command *cmd, uint32_t d) {
    tach_ast_print_nspaces(of, d);
    tach_fprintf(of, "cmd: (at %d,%d)\n", cmd->lineno, cmd->colno);
    for (uint32_t i = 0; i < cmd->count; i++) {
        tach_ast_print_single(of, cmd->singles[i], d+2);
    }    

}
void tach_ast_print_single(tach_file *of, tach_ast_single *single, uint32_t d) {
    switch(single->type) {
        case tach_ast_single_proc: {
            tach_ast_print_proc(of, single->value.proc, d);
            break;
        }
        case tach_ast_single_command: {
            tach_ast_print_command(of, single->value.command, d);
            break;
        }
        case tach_ast_single_name: {
            tach_ast_print_nspaces(of, d);
            tach_fprintf(of, "name: %s (at %d,%d)\n", single->value.name, single->lineno, single->colno);
            break;
        }
        case tach_ast_single_string: {
            tach_ast_print_nspaces(of, d);
            tach_fprintf(of, "string: %s (at %d,%d)\n", single->value.string, single->lineno, single->colno);
            break;
        }
        case tach_ast_single_number: {
            tach_ast_print_nspaces(of, d);
            tach_fprintf(of, "number: %s (at %d,%d)\n", single->value.number, single->lineno, single->colno);
            break;
        }
    }
}
