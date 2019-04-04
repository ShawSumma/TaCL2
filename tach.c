#include "tach.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("give a file to run\n");
        exit(1);
    } 
    FILE *f = fopen(argv[1], "r");
    if (f != NULL) {
        tach_ast_proc *proc = tach_ast_read_proc(f);
        tach_program *prog = tach_comp_main(proc);
        tach_ast_free_proc(proc);
        tach_state *state = tach_create_state();
        tach_program_run(state, prog);
        tach_free_program(prog);
        tach_free_state(state);
    }
    fclose(f);
}