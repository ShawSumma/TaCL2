#include <tach.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        tach_state *state = tach_create_state();
        tach_program *prog = NULL;
        while (true) {
            prog = tach_read_repl(tach_pfopen(stdin), prog);
            tach_program_run(state, prog);
        }
        tach_free_program(prog);
        tach_free_state(state);
    } 
    else {
        tach_file *f = tach_fopen(argv[1], "r");
        if (f != NULL) {
            tach_program *prog = tach_read(f);
            tach_state *state = tach_create_state();
            // for (uint32_t i = 0; i < prog->opcount; i++) {
            //     printf("%d\t:%d\t%d\n", i, prog->opcodes[i].type, prog->opcodes[i].value);
            // }
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