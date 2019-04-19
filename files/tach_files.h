#pragma once
#include <tach.h>

struct tach_file {
    union {
        FILE *pfile;
        struct {
            tach_string str;
            uint32_t place;
        } str;
    } file;
    enum {
        tach_file_pfile,
        tach_file_port,
        tach_file_string
    } type;
};

char tach_getc(tach_file *);
tach_file *tach_fopen(char *, char *);
tach_file *tach_sfopen(tach_string);
tach_file *tach_cfopen(char *);
tach_file *tach_pfopen(FILE *);
void tach_fclose(tach_file *);
void tach_ungetc(char, tach_file *);
void tach_fprintf(tach_file *, char *, ...);