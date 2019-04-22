#include <tach.h>

void dostring(char *str) {
    tach_file *f = tach_cfopen(str);
    tach_ast_proc *process = tach_ast_read_proc(f, false);
    tach_ast_print_program(tach_pfopen(stderr), process);
    tach_program *program = tach_bytecomp_main(process);
    tach_state *state = tach_create_state();
    tach_ast_free_proc(process);    
    tach_program_run(state, program);
    tach_free_program(program);
    tach_free_state(state);
    tach_fclose(f);
}

int main() {
    dostring("print (hello world)");
}