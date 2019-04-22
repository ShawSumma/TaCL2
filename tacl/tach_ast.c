#include <tach.h>

tach_ast_proc *tach_ast_read_proc(tach_file *f, bool isonce) {
    tach_ast_proc *ret = malloc(sizeof(tach_ast_proc));

    uint32_t alloc = 4;
    uint32_t count = 0;

    tach_ast_command **commands = malloc(sizeof(tach_ast_command *) * alloc);

    char got = tach_getc(f);
    if (got != '{') {
        tach_ungetc(got, f);
    }
    while (true) {
        got = tach_getc(f);
        while (got == '\r' || got == '\n' || got == ';' || got == '\t' || got == ' ' || got == ']') {
            got = tach_getc(f);
        }
        if (got == '}' || got == EOF) break;
        if (count + 4 >= alloc) {
            alloc *= 1.5;
            commands = realloc(commands, sizeof(tach_ast_command *) * alloc);
        }
        tach_ungetc(got, f);
        commands[count] = tach_ast_read_command(f, false);
        count ++;
        if (isonce) break;
    }

    ret->commands = commands;
    ret->count = count;

    return ret;
}

tach_ast_command *tach_ast_read_command(tach_file *f, bool issub) {
    tach_ast_command *ret = malloc(sizeof(tach_ast_command));

    uint32_t alloc = 4;
    uint32_t count = 0;

    tach_ast_single **singles = malloc(sizeof(tach_ast_single *) * alloc);
    char got;
    while (true) {
        got = tach_getc(f);
        if (count + 4 > alloc) {
            alloc *= 1.5;
            singles = realloc(singles, sizeof(tach_ast_single *) * alloc);
        }
        if (got == '}' || got == EOF) {
            break;
        }
        if (issub) {
            while (got == '\t' || got == ' ' || got == '\n' || got == '\r') {
                got = tach_getc(f);
            }
            if (got == ']') {
                break;
            }
        }
        else {
            while (got == '\t' || got == ' ') {
                got = tach_getc(f);
            }
            if (got == '\n' || got == '\r' || got == ';') {
                break;
            }
        }
        if (got == '}') {
            break;
        }
        tach_ungetc(got, f);
        singles[count] = tach_ast_read_single(f);
        count ++;
    }
    tach_ungetc(got, f);
    ret->singles = singles;
    ret->count = count;

    return ret;
}

tach_ast_single *tach_ast_read_single(tach_file *f) {
    char got = tach_getc(f);
    if (got == '$') {
        tach_ast_single *ret = malloc(sizeof(tach_ast_single));
        ret->type = tach_ast_single_name;
        ret->value.name = tach_ast_read_name(f);
        return ret;
    }
    if ((got >= 'A' && got <= 'Z') || (got >= 'a' && got <= 'z') || got == '_' || got == '(') {
        tach_ungetc(got, f);
        tach_ast_single *ret = malloc(sizeof(tach_ast_single));
        ret->type = tach_ast_single_string;
        ret->value.string = tach_ast_read_name(f);
        return ret;
    }
    if ((got >= '0' && got <= '9') || got == '-') {
        tach_ungetc(got, f);
        tach_ast_single *ret = malloc(sizeof(tach_ast_single));
        ret->type = tach_ast_single_number;
        ret->value.number = tach_ast_read_number(f);
        return ret;
    }
    if (got == '{') {
        tach_ungetc(got, f);
        tach_ast_single *ret = malloc(sizeof(tach_ast_single));
        ret->type = tach_ast_single_proc;
        ret->value.proc = tach_ast_read_proc(f, false);
        return ret;
    }
    if (got == '[') {
        tach_ast_single *ret = malloc(sizeof(tach_ast_single));
        ret->type = tach_ast_single_command;
        ret->value.command = tach_ast_read_command(f, true);
        return ret;
    }
    fprintf(stderr, "error reading file %c\n", got);
    exit(1);
}

char *tach_ast_read_name(tach_file *f) {
    char got = tach_getc(f);
    if (got == '(') {
        uint32_t alloc = 16;
        char *name = malloc(sizeof(char) * alloc);
        uint32_t place = 0;
        uint32_t depth = 1;
        while (true) {
            got = tach_getc(f);
            if (got == '(') {
                depth ++;
            }
            if (got == ')') {
                depth --;
            }
            if (depth == 0) {
                break;
            }
            if (place + 4 > alloc) {
                alloc *= 1.5;
                name = realloc(name, sizeof(char) * alloc);
            }
            name[place] = got;
            place ++;
        }
        name[place] = '\0';
        return name;
    }
    else {
        uint32_t alloc = 16;
        char *name = malloc(sizeof(char) * alloc);
        uint32_t place = 0;
        while ((got >= 'A' && got <= 'Z') || (got >= 'a' && got <= 'z') || got == '_' || got == '-') {
            if (place + 4 > alloc) {
                alloc *= 1.5;
                name = realloc(name, sizeof(char) * alloc);
            }
            name[place] = got;
            place ++;
            got = tach_getc(f);
        }
        tach_ungetc(got, f);
        name[place] = '\0';
        return name;
    }
}

char *tach_ast_read_number(tach_file *f) {
    char got = tach_getc(f);
    uint32_t alloc = 16;
    char *name = malloc(sizeof(char) * alloc);
    uint32_t place = 0;
    while ((got >= '0' && got <= '9') || got == '/' || got == '-') {
        if (place + 4 > alloc) {
            alloc *= 1.5;
            name = realloc(name, sizeof(char) * alloc);
        }
        name[place] = got;
        place ++;
        got = tach_getc(f);
    }
    tach_ungetc(got, f);
    name[place] = '\0';
    return name;
}

