#include <tach.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        // tach_state *state = tach_create_state();
        // while (1) {
        //     tach_program *prog = tach_read_repl(stdin);
        //     tach_program_run(state, prog);
        //     tach_free_program(prog);
        // }
        // tach_free_state(state);
    } 
    else {
        FILE *f = fopen(argv[1], "r");
        if (f != NULL) {
            tach_program *prog = tach_read(f);
            tach_state *state = tach_create_state();
            tach_program_run(state, prog);
            tach_free_program(prog);
            tach_free_state(state);
        }
        else {
            fprintf(stderr, "no such file\n");
            exit(1);
        }
        fclose(f);
    }
    
}