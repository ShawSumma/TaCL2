#include <tach.h>

tach_ast_state *tach_create_ast_state() {
    tach_ast_state *state = malloc(sizeof(tach_ast_state));
    state->lineno = 1;
    state->colno = 1;
    return state;
}

char tach_ast_state_getc(tach_ast_state *s, tach_file *f) {
    char c = tach_getc(f);
    if (c == '\n') {
        s->lineno ++;
        s->ld = s->colno;
        s->colno = 1;
    }
    else {
        s->colno ++;
    }
    return c;
}

void tach_ast_state_ungetc(tach_ast_state *s, char c, tach_file *f) {
    if (c == '\n') {
        s->lineno --;
        s->colno = s->ld;
    }
    else if (s->colno == 1) {
        s->lineno --;
        s->colno = s->ld;
    }
    else {
        s->colno --;
    }
    tach_ungetc(c, f);
}

tach_ast_proc *tach_ast_read_proc(tach_ast_state *state, tach_file *f, bool isonce) {
    tach_ast_proc *ret = malloc(sizeof(tach_ast_proc));
    ret->lineno = state->lineno;
    ret->colno = state->colno;

    uint32_t alloc = 4;
    uint32_t count = 0;

    tach_ast_command **commands = malloc(sizeof(tach_ast_command *) * alloc);

    char got = tach_ast_state_getc(state, f);
    if (got != '{') {
        tach_ast_state_ungetc(state, got, f);
    }
    while (true) {
        got = tach_ast_state_getc(state, f);
        while (got == '\r' || got == '\n' || got == ';' || got == '\t' || got == ' ' || got == ']') {
            got = tach_ast_state_getc(state, f);
        }
        if (got == '}' || got == EOF) break;
        if (count + 4 >= alloc) {
            alloc *= 1.5;
            commands = realloc(commands, sizeof(tach_ast_command *) * alloc);
        }
        tach_ast_state_ungetc(state, got, f);
        tach_ast_command *cmd = tach_ast_read_command(state, f, false);
        if (cmd != NULL) {
            commands[count] = cmd;
            count ++;
        }
        if (isonce) break;
    }

    ret->commands = commands;
    ret->count = count;

    return ret;
}

tach_ast_command *tach_ast_read_command(tach_ast_state *state,tach_file *f, bool issub) {
    tach_ast_command *ret = malloc(sizeof(tach_ast_command));
    ret->lineno = state->lineno;
    ret->colno = state->colno;

    uint32_t alloc = 4;
    uint32_t count = 0;

    tach_ast_single **singles = malloc(sizeof(tach_ast_single *) * alloc);
    char got;
    bool discard = false;
    while (true) {
        got = tach_ast_state_getc(state, f);
        if (count + 4 > alloc) {
            alloc *= 1.5;
            singles = realloc(singles, sizeof(tach_ast_single *) * alloc);
        }
        if (issub) {
            while (got == '\t' || got == ' ' || got == '\n' || got == '\r') {
                got = tach_ast_state_getc(state, f);
            }
            if (got == ']') {
                break;
            }
        }
        else {
            while (got == '\t' || got == ' ') {
                got = tach_ast_state_getc(state, f);
            }
            if (got == '\n' || got == '\r' || got == ';' || got == EOF) {
                break;
            }
        }
        if (got == '}') {
            break;
        }
        if (got == '#') {
            got = tach_ast_state_getc(state, f);
            discard = true;
            while (got == '\t' || got == ' ') {
                got = tach_ast_state_getc(state, f);
            }
        }
        tach_ast_state_ungetc(state, got, f);
        singles[count] = tach_ast_read_single(state, f);
        count ++;
    }
    if (got == '}') {
        tach_ast_state_ungetc(state, got, f);
    }
    ret->singles = singles;
    ret->count = count;

    if (discard) {
        tach_ast_free_command(ret);
        return NULL;
    }
    return ret;
}

tach_ast_single *tach_ast_read_single(tach_ast_state *state, tach_file *f) {
    tach_ast_single *ret = malloc(sizeof(tach_ast_single));
    ret->lineno = state->lineno;
    ret->colno = state->colno;
    char got = tach_ast_state_getc(state, f);
    if (got == '$') {
        ret->type = tach_ast_single_name;
        ret->value.name = tach_ast_read_name(state, f);
        return ret;
    }
    if ((got >= 'A' && got <= 'Z') || (got >= 'a' && got <= 'z') || got == '_' || got == '(') {
        tach_ast_state_ungetc(state, got, f);
        ret->type = tach_ast_single_string;
        ret->value.string = tach_ast_read_name(state, f);
        return ret;
    }
    if ((got >= '0' && got <= '9') || got == '-') {
        tach_ast_state_ungetc(state, got, f);
        ret->type = tach_ast_single_number;
        ret->value.number = tach_ast_read_number(state, f);
        return ret;
    }
    if (got == '{') {
        tach_ast_state_ungetc(state, got, f);
        ret->type = tach_ast_single_proc;
        ret->value.proc = tach_ast_read_proc(state, f, false);
        return ret;
    }
    if (got == '[') {
        ret->type = tach_ast_single_command;
        ret->value.command = tach_ast_read_command(state, f, true);
        return ret;
    }
    fprintf(stderr, "error reading file %d\n", got);
    exit(1);
}

char *tach_ast_read_name(tach_ast_state *state, tach_file *f) {
    char got = tach_ast_state_getc(state, f);
    if (got == '(') {
        uint32_t alloc = 16;
        char *name = malloc(sizeof(char) * alloc);
        uint32_t place = 0;
        uint32_t depth = 1;
        while (true) {
            got = tach_ast_state_getc(state, f);
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
        while ((got >= 'A' && got <= 'Z') || (got >= 'a' && got <= 'z') || got == '_' || got == '-' || got == '.') {
            if (place + 4 > alloc) {
                alloc *= 1.5;
                name = realloc(name, sizeof(char) * alloc);
            }
            name[place] = got;
            place ++;
            got = tach_ast_state_getc(state, f);
        }
        tach_ast_state_ungetc(state, got, f);
        name[place] = '\0';
        return name;
    }
}

char *tach_ast_read_number(tach_ast_state *state, tach_file *f) {
    char got = tach_ast_state_getc(state, f);
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
        got = tach_ast_state_getc(state, f);
    }
    tach_ast_state_ungetc(state, got, f);
    name[place] = '\0';
    return name;
}

