#include <tach.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        tach_ast_state *astc = tach_create_ast_state();
        tach_state *state = tach_create_state();
        tach_program *prog = NULL;
        while (true) {
            prog = tach_read_repl(astc, tach_pfopen(stdin), prog);
            tach_program_run(state, prog);
        }
        free(astc);
        tach_free_program(prog);
        tach_free_state(state);
    }
    else {
        tach_file *f = tach_fopen(argv[1], "r");
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
        tach_fclose(f);
    }
}