#pragma once

#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

struct tach_object;
typedef struct tach_object tach_object;
struct tach_pair;
typedef struct tach_pair tach_pair;
struct tach_vector;
typedef struct tach_vector tach_vector;
struct tach_point;
typedef struct tach_point tach_point;

struct tach_opcode;
typedef struct tach_opcode tach_opcode;
struct tach_program;
typedef struct tach_program tach_program;
struct tach_table;
typedef struct tach_table tach_table;
struct tach_string;
typedef struct tach_string tach_string;

struct tach_ast_proc;
typedef struct tach_ast_proc tach_ast_proc;
struct tach_ast_command;
typedef struct tach_ast_command tach_ast_command;
struct tach_ast_single;
typedef struct tach_ast_single tach_ast_single;

struct tach_state;
typedef struct tach_state tach_state;

typedef tach_object *(*tach_func)(tach_state *, uint32_t, tach_object **);

#include "tach_memory.h"
#include "tach_object.h"
#include "tach_comp.h"
#include "tach_ast.h"
#include "tach_vm.h"
#include "tach_clib.h"
#include "tach_lib.h"

