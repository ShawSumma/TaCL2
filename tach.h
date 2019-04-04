#pragma once

// #define tach_number_use_double
#define tach_number_use_gmp

#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#ifdef tach_number_use_gmp
#include <gmp.h>
#endif

#include <dlfcn.h>

#include "object/tach_types.h"
#include "vm/tach_types.h"
#include "lib/tach_types.h"
// #include "tacl/tach_types.h"

#include "object/tach_all.h"
#include "vm/tach_all.h"
#include "lib/tach_all.h"
// #include "tacl/tach_all.h"