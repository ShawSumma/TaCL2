#include "tach.h"

tach_program *tach_read(FILE *f) {
    tach_ast_proc *proc = tach_ast_read_proc(f, false);
    tach_program *prog = tach_comp_main(proc);
    tach_ast_free_proc(proc);    
    return prog;
}

tach_program *tach_read_repl(FILE *f) {
    tach_ast_proc *proc = tach_ast_read_proc(f, true);
    tach_program *prog = tach_comp_main(proc);
    tach_ast_free_proc(proc);    
    return prog;
}


tach_program *tach_comp_main(tach_ast_proc *proc) {
    tach_program *ret = malloc(sizeof(tach_program));
    ret->objcount = 0;
    ret->objalloc = 8;
    ret->objs = malloc(sizeof(tach_object *) * ret->objalloc);
    ret->opcount = 0;
    ret->opalloc = 8;
    ret->opcodes = malloc(sizeof(tach_opcode) * ret->opalloc);
    for (uint32_t i = 0; i < proc->count; i++) {
        tach_comp_command(ret, proc->commands[i]);
    }
    return ret;
}

void tach_comp_proc(tach_program *prog, tach_ast_proc *proc) {
    tach_program_realloc(prog);   
    uint32_t begin = prog->opcount;
    prog->opcodes[prog->opcount].type = tach_opcode_proc;
    prog->opcount ++;
    for (uint32_t i = 0; i < proc->count; i++) {
        if (i != 0) {
            tach_program_realloc(prog);
            prog->opcodes[prog->opcount].type = tach_opcode_pop;
            prog->opcodes[prog->opcount].value = 0;
            prog->opcount ++;
        }
        tach_comp_command(prog, proc->commands[i]);
    }
    tach_program_realloc(prog);
    prog->opcodes[begin].value = prog->opcount;    
    prog->opcodes[prog->opcount].type = tach_opcode_ret;
    prog->opcodes[prog->opcount].value = 0;
    prog->opcount ++;
}

void tach_comp_command(tach_program *prog, tach_ast_command *cmd) {
    if (cmd->count != 0) {
        cmd->singles[0]->type = tach_ast_single_name;
        for (uint32_t i = 0; i < cmd->count; i++) {
            tach_comp_single(prog, cmd->singles[i]);
        }
        tach_program_realloc(prog);
        prog->opcodes[prog->opcount].type = tach_opcode_call;
        prog->opcodes[prog->opcount].value = cmd->count - 1;
        prog->opcount ++;
    }
}

void tach_comp_single(tach_program *prog, tach_ast_single *single) {
    tach_program_realloc(prog);
    switch (single->type) {
        case tach_ast_single_name:  {
            prog->opcodes[prog->opcount].type = tach_opcode_load;
            prog->opcodes[prog->opcount].value = prog->objcount;
            prog->objs[prog->objcount] = tach_object_make_string(tach_create_string(single->value.name));
            prog->opcount ++;
            prog->objcount ++;
            break;
        }
        case tach_ast_single_number: {
            prog->opcodes[prog->opcount].type = tach_opcode_push;
            prog->opcodes[prog->opcount].value = prog->objcount;
            prog->objs[prog->objcount] = tach_object_make_number(tach_create_number_string(single->value.number));
            prog->opcount ++;
            prog->objcount ++;
            free(single->value.number);
            break;
        }
        case tach_ast_single_string: {
            prog->opcodes[prog->opcount].type = tach_opcode_push;
            prog->opcodes[prog->opcount].value = prog->objcount;
            prog->objs[prog->objcount] = tach_object_make_string(tach_create_string(single->value.name));
            prog->opcount ++;
            prog->objcount ++;
            break;
        }
        case tach_ast_single_proc: {
            tach_comp_proc(prog, single->value.proc);
            break;
        }
        case tach_ast_single_command: {
            tach_comp_command(prog, single->value.command);
            break;
        }
    }
}

void tach_program_realloc(tach_program *prog) {
    if (prog->opcount + 4 > prog->opalloc) {
        prog->opalloc *= 1.5;
        prog->opcodes = realloc(prog->opcodes, sizeof(tach_opcode *) * prog->opalloc);
    }
    if (prog->objcount + 4 > prog->objalloc) {
        prog->objalloc *= 1.5;
        prog->objs = realloc(prog->objs, sizeof(tach_object *) * prog->objalloc);
    }
}

