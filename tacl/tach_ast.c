#include "tach_ast.h"

tach_ast_proc *tach_ast_read_proc(FILE *f) {
    tach_ast_proc *ret = malloc(sizeof(tach_ast_proc));

    uint32_t alloc = 4;
    uint32_t count = 0;

    tach_ast_command **commands = malloc(sizeof(tach_ast_command *) * alloc);

    char got = getc(f);
    if (got == '{') {
        got = getc(f);
    }
    while (true) {
        while (got == '\r' || got == '\n' || got == '\t' || got == ' ') {
            got = getc(f);
        }
        if (got == '}' || got == EOF) {
            break;
        }
        if (count + 4 >= alloc) {
            alloc *= 1.5;
            commands = realloc(commands, sizeof(tach_ast_command *) * alloc);
        }
        ungetc(got, f);
        commands[count] = tach_ast_read_command(f);
        count ++;
        got = getc(f);
    }

    ret->commands = commands;
    ret->count = count;

    return ret;
}

tach_ast_command *tach_ast_read_command(FILE *f) {
    tach_ast_command *ret = malloc(sizeof(tach_ast_command));

    uint32_t alloc = 4;
    uint32_t count = 0;

    tach_ast_single **singles = malloc(sizeof(tach_ast_single *) * alloc);

    char got = getc(f);
    while (got != '}' && got != EOF) {
        if (count + 4 > alloc) {
            alloc *= 1.5;
            singles = realloc(singles, sizeof(tach_ast_single *) * alloc);
        }
        while (got == '\t' || got == ' ') {
            got = getc(f);
        }
        if (got == '\n' || got == '\r' || got == ']') break;
        ungetc(got, f);
        singles[count] = tach_ast_read_single(f);
        count ++;
        got = getc(f);
    }

    ret->singles = singles;
    ret->count = count;

    return ret;
}

tach_ast_single *tach_ast_read_single(FILE *f) {
    char got = getc(f);
    if (got == '$') {
        tach_ast_single *ret = malloc(sizeof(tach_ast_single));
        ret->type = tach_ast_single_name;
        ret->value.name = tach_ast_read_name(f);
        return ret;
    }
    if ((got >= 'a' && got <= 'z') || got == '_' || got == '(') {
        ungetc(got, f);
        tach_ast_single *ret = malloc(sizeof(tach_ast_single));
        ret->type = tach_ast_single_string;
        ret->value.string = tach_ast_read_name(f);
        return ret;
    }
    if (got >= '0' && got <= '9') {
        ungetc(got, f);
        tach_ast_single *ret = malloc(sizeof(tach_ast_single));
        ret->type = tach_ast_single_number;
        ret->value.number = tach_ast_read_number(f);
        return ret;
    }
    if (got == '{') {
        ungetc(got, f);
        tach_ast_single *ret = malloc(sizeof(tach_ast_single));
        ret->type = tach_ast_single_proc;
        ret->value.proc = tach_ast_read_proc(f);
        return ret;
    }
    if (got == '[') {
        tach_ast_single *ret = malloc(sizeof(tach_ast_single));
        ret->type = tach_ast_single_command;
        ret->value.command = tach_ast_read_command(f);
        return ret;
    }
    printf("error reading file\n");
    exit(1);
}

char *tach_ast_read_name(FILE *f) {
    char got = getc(f);
    if (got == '(') {
        uint32_t alloc = 16;
        char *name = malloc(sizeof(char) * alloc);
        uint32_t place = 0;
        uint32_t depth = 1;
        while (true) {
            got = getc(f);
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
        while ((got >= 'a' && got <= 'z') || got == '_') {
            if (place + 4 > alloc) {
                alloc *= 1.5;
                name = realloc(name, sizeof(char) * alloc);
            }
            name[place] = got;
            place ++;
            got = getc(f);
        }
        ungetc(got, f);
        name[place] = '\0';
        return name;
    }
}

char *tach_ast_read_number(FILE *f) {
    char got = getc(f);
    uint32_t alloc = 16;
    char *name = malloc(sizeof(char) * alloc);
    uint32_t place = 0;
    while ((got >= '0' && got <= '9')) {
        if (place + 4 > alloc) {
            alloc *= 1.5;
            name = realloc(name, sizeof(char) * alloc);
        }
        name[place] = got;
        place ++;
        got = getc(f);
    }
    ungetc(got, f);
    name[place] = '\0';
    return name;
}

