#include <tach.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("2 or more args\n");
    }
    else {
        for (uint32_t i = 1; i < argc; i++) {
            tach_file *f = tach_fopen(argv[i], "r");
            if (f != NULL) {
                tach_program *prog = tach_read(f);
                tach_state *state = tach_create_state(prog);
                tach_object *obj = tach_program_run(state);
                tach_free_state(state);
                tach_free_object(obj);
            }
            else {
                fprintf(stderr, "no such file\n");
                exit(1);
            }
            tach_fclose(f);
        }
    }
}