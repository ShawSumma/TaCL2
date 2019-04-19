#include <tach.h>

char tach_getc(tach_file *f) {
    switch (f->type) {
        case tach_file_port:
        case tach_file_pfile: {
            char got = getc(f->file.pfile);
            return got;
            break;
        }
        case tach_file_string: {
            if (f->file.str.place >= f->file.str.str.count) {
                
                return EOF;
            }
            char got = f->file.str.str.str[f->file.str.place];
            f->file.str.place ++;
            return got;
            break;
        }
    }
    return EOF;
}

tach_file *tach_fopen(char *n, char *t) {
    tach_file *ret = malloc(sizeof(tach_file));
    ret->type = tach_file_pfile;
    ret->file.pfile = fopen(n, t);
    if (ret->file.pfile == NULL) {
        return NULL;
    }
    return ret;
}

tach_file *tach_sfopen(tach_string s) {
    tach_file *ret = malloc(sizeof(tach_file));
    ret->type = tach_file_string;
    ret->file.str.str = tach_create_string(s.str);
    ret->file.str.place = 0;
    return ret;
}

tach_file *tach_cfopen(char *s) {
    tach_file *ret = malloc(sizeof(tach_file));
    ret->type = tach_file_string;
    ret->file.str.str = tach_create_string(s);
    ret->file.str.place = 0;
    return ret;
}

tach_file *tach_pfopen(FILE *f) {
    tach_file *ret = malloc(sizeof(tach_file));
    ret->type = tach_file_pfile;
    ret->file.pfile = f;
    return ret;
}

void tach_ungetc(char c, tach_file *f) {
    switch (f->type) {
        case tach_file_port:
        case tach_file_pfile: {
            ungetc(c, f->file.pfile);
            break;
        }
        case tach_file_string: {
            f->file.str.place --;
            f->file.str.str.str[f->file.str.place] = c;
            break;
        }
    }
}

void tach_fclose(tach_file *f) {
    switch (f->type) {
        case tach_file_pfile: {
            fclose(f->file.pfile);
            break;
        }
        case tach_file_string: {
            free(f->file.str.str.str);
            break;
        }
        case tach_file_port: {
            break;
        }
    }
    free(f);
}

void tach_fprintf(tach_file *f, char *format, ...) {
    va_list args;
    va_start(args, format);
    switch (f->type) {
        case tach_file_port:
        case tach_file_pfile: {
            vfprintf(f->file.pfile, format, args);
            break;
        }
        case tach_file_string: {
            char dbuf[8];
            uint32_t need = vsnprintf(dbuf, 0, format, args);
            va_end(args);
            va_start(args, format);
            if (f->file.str.place + f->file.str.str.count > f->file.str.str.alloc) {
                f->file.str.str.alloc += 8;
                f->file.str.str.alloc *= 1.5;
                f->file.str.str.str = realloc(f->file.str.str.str, sizeof(char) * f->file.str.str.alloc);
            }
            char *c = f->file.str.str.str + f->file.str.place;
            vsnprintf(c, need, format, args);
            break;
        }
    }
    va_end(args);
}