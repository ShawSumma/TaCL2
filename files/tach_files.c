#include "tach.h"

char tach_getc(tach_file *f) {
    switch (f->type) {
        case tach_file_pfile: {
            return tach_getc(f->file.pfile);
        }
        case tach_file_string: {
            if (f->file.str.place >= f->file.str.str.count) {
                return EOF;
            }
            char got = f->file.str.str.str[f->file.str.place];
            f->file.str.place ++;
            return got;
        }
    }
    return EOF;
}

tach_file *tach_fopen(char *n, char *t) {
    tach_file *ret = malloc(sizeof(tach_file));
    ret->type = tach_file_pfile;
    ret->file.pfile = tach_fopen(n, t);
    return ret;
}

tach_file *tach_sfopen(tach_string s) {
    tach_file *ret = malloc(sizeof(tach_file));
    ret->type = tach_file_string;
    ret->file.str.str = tach_create_string(s.str);
    ret->file.str.place = 0;
    return ret;
}

void tach_ungetc(char c, tach_file *f) {
    switch (f->type) {
        case tach_file_pfile: {
            ungetc(c, f->file.pfile);
        }
        case tach_file_string: {
            f->file.str.place --;
            f->file.str.str.str[f->file.str.place] = c;
        }
    }
    free(f);
}

void tach_fclose(tach_file *f) {
    switch (f->type) {
        case tach_file_pfile: {
            tach_fclose(f->file.pfile);
        }
        case tach_file_string: {
            free(f->file.str.str.str);
        }
    }
    free(f);
}