#include <tach.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("give an interpreter and a file file to run\n");
        exit(1);
    } 
    FILE *f = fopen(argv[2], "r");
    if (f != NULL) {
        tach_program *prog = tach_read(f);
        tach_state *state = tach_create_state();
        tach_program_run(state, prog);
        tach_free_program(prog);
        tach_free_state(state);
    }
    fclose(f);
}