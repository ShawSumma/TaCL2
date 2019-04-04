#include <tach.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("give an interpreter and a file file to run\n");
        exit(1);
    } 
    void *tcllib = dlopen(argv[1], RTLD_LAZY);
    FILE *f = fopen(argv[2], "r");
    if (f != NULL && tcllib != NULL) {
        // tach_ast_proc *proc = tach_ast_read_proc(f);
        // tach_program *prog = tach_comp_main(proc);
        tach_program *(*tach_read)(FILE *f) = dlsym(tcllib, "tach_read");
        tach_program *prog = tach_read(f);
        // tach_ast_free_proc(proc);
        tach_state *(*tach_create_state)() = dlsym(tcllib, "tach_create_state");
        tach_state *state = tach_create_state();
        tach_program_run(state, prog);
        tach_free_program(prog);
        tach_free_state(state);
    }
    fclose(f);
}