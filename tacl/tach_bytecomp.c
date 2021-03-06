#include <tach.h>

tach_program *tach_read(tach_file *f) {
    tach_ast_state *state = tach_create_ast_state();
    tach_ast_proc *proc = tach_ast_read_proc(state, f, false);
    free(state);
    tach_program *prog = tach_bytecomp_main(proc);
    tach_ast_free_proc(proc);
    return prog;
}

tach_program *tach_read_repl(tach_ast_state *state, tach_file *f, tach_program *p) {
    if (p == NULL) {
        p = tach_bytecomp_main_empty();
    }
    tach_ast_proc *proc = tach_ast_read_proc(state, f, true);
    tach_program *prog = tach_bytecomp_main_more(proc, p);
    tach_ast_free_proc(proc);    
    return prog;
}

tach_program *tach_bytecomp_main_empty() {
    tach_program *ret = malloc(sizeof(tach_program));
    ret->objcount = 0;
    ret->objalloc = 8;
    ret->objs = malloc(sizeof(tach_object *) * ret->objalloc);
    ret->opcount = 0;
    ret->opalloc = 8;
    ret->opcodes = malloc(sizeof(tach_opcode) * ret->opalloc);
    ret->linenos = malloc(sizeof(uint32_t) * ret->opalloc);
    ret->colnos = malloc(sizeof(uint32_t) * ret->opalloc);
    return ret;
}

tach_program *tach_bytecomp_main(tach_ast_proc *proc) {
    tach_program *ret = tach_bytecomp_main_empty();
    for (uint32_t i = 0; i < proc->count; i++) {
        if (i != 0) {
            tach_program_realloc(ret);
            ret->opcodes[ret->opcount].type = tach_opcode_pop;
            ret->opcodes[ret->opcount].value = 0;
            ret->linenos[ret->opcount] = proc->lineno;
            ret->colnos[ret->opcount] = proc->colno;
            ret->opcount ++;
        }
        tach_bytecomp_command(ret, proc->commands[i]);
    }
    return ret;
}

tach_program *tach_bytecomp_main_more(tach_ast_proc *proc, tach_program *ret) {
    for (uint32_t i = 0; i < proc->count; i++) {
        tach_bytecomp_command(ret, proc->commands[i]);
    }
    return ret;
}

void tach_bytecomp_proc(tach_program *prog, tach_ast_proc *proc) {
    tach_program_realloc(prog);   
    uint32_t begin = prog->opcount;
    prog->opcodes[prog->opcount].type = tach_opcode_proc;
    prog->opcodes[prog->opcount].value = 0;
    prog->linenos[prog->opcount] = proc->lineno;
    prog->colnos[prog->opcount] = proc->colno;
    prog->opcount ++;
    for (uint32_t i = 0; i < proc->count; i++) {
        if (i != 0) {
            tach_program_realloc(prog);
            prog->opcodes[prog->opcount].type = tach_opcode_pop;
            prog->opcodes[prog->opcount].value = 0;
            prog->linenos[prog->opcount] = proc->lineno;
            prog->colnos[prog->opcount] = proc->colno;
            prog->opcount ++;
        }
        tach_bytecomp_command(prog, proc->commands[i]);
    }
    if (proc->count == 0) {
        tach_program_realloc(prog);
        prog->opcodes[prog->opcount].type = tach_opcode_push;
        prog->opcodes[prog->opcount].value = prog->objcount;
        prog->linenos[prog->opcount] = proc->lineno;
        prog->colnos[prog->opcount] = proc->colno;
        prog->objs[prog->objcount] = tach_object_make_nil();
        prog->opcount ++;
        prog->objcount ++;
    }
    tach_program_realloc(prog);
    prog->opcodes[begin].value = prog->opcount;    
    prog->opcodes[prog->opcount].type = tach_opcode_ret;
    prog->opcodes[prog->opcount].value = 0;
    prog->linenos[prog->opcount] = proc->lineno;
    prog->colnos[prog->opcount] = proc->colno;
    prog->opcount ++;
}

void tach_bytecomp_command(tach_program *prog, tach_ast_command *cmd) {
    if (cmd->count != 0) {
        if (cmd->singles[0]->type == tach_ast_single_string) {
            cmd->singles[0]->type = tach_ast_single_name;
        }
        for (uint32_t i = 0; i < cmd->count; i++) {
            tach_bytecomp_single(prog, cmd->singles[i]);
        }
        tach_program_realloc(prog);
        prog->opcodes[prog->opcount].type = tach_opcode_call;
        prog->opcodes[prog->opcount].value = cmd->count - 1;
        prog->linenos[prog->opcount] = cmd->lineno;
        prog->colnos[prog->opcount] = cmd->colno;
        prog->opcount ++;
    }
}

void tach_bytecomp_single(tach_program *prog, tach_ast_single *single) {
    tach_program_realloc(prog);
    switch (single->type) {
        case tach_ast_single_name:  {
            prog->opcodes[prog->opcount].type = tach_opcode_load;
            prog->opcodes[prog->opcount].value = prog->objcount;
            prog->linenos[prog->opcount] = single->lineno;
            prog->colnos[prog->opcount] = single->colno;
            prog->objs[prog->objcount] = tach_object_make_string(tach_create_string(single->value.name));
            prog->opcount ++;
            prog->objcount ++;
            break;
        }
        case tach_ast_single_number: {
            prog->opcodes[prog->opcount].type = tach_opcode_push;
            prog->opcodes[prog->opcount].value = prog->objcount;
            prog->linenos[prog->opcount] = single->lineno;
            prog->colnos[prog->opcount] = single->colno;
            prog->objs[prog->objcount] = tach_object_make_number(tach_create_number_string(single->value.number));
            prog->opcount ++;
            prog->objcount ++;
            break;
        }
        case tach_ast_single_string: {
            prog->opcodes[prog->opcount].type = tach_opcode_push;
            prog->opcodes[prog->opcount].value = prog->objcount;
            prog->linenos[prog->opcount] = single->lineno;
            prog->colnos[prog->opcount] = single->colno;
            prog->objs[prog->objcount] = tach_object_make_string(tach_create_string(single->value.name));
            prog->opcount ++;
            prog->objcount ++;
            break;
        }
        case tach_ast_single_proc: {
            tach_bytecomp_proc(prog, single->value.proc);
            break;
        }
        case tach_ast_single_command: {
            tach_bytecomp_command(prog, single->value.command);
            break;
        }
    }
}

void tach_program_realloc(tach_program *prog) {
    if (prog->opcount + 4 > prog->opalloc) {
        prog->opalloc *= 1.5;
        prog->opcodes = realloc(prog->opcodes, sizeof(tach_opcode) * prog->opalloc);
        prog->linenos = realloc(prog->linenos, sizeof(uint32_t) * prog->opalloc);
        prog->colnos = realloc(prog->colnos, sizeof(uint32_t) * prog->opalloc);
    }
    if (prog->objcount + 4 > prog->objalloc) {
        prog->objalloc *= 1.5;
        prog->objs = realloc(prog->objs, sizeof(tach_object *) * prog->objalloc);
    }
}

