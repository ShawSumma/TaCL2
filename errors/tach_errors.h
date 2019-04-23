#include <tach.h>

void tach_errors_lineout(tach_state *);
void tach_errors_index(tach_state *, int32_t, int32_t, int32_t);
void tach_errors_builtin_table_index(tach_state *, tach_object *);
void tach_errors_type_typecheck_anon(tach_state *, tach_object *, uint32_t);
void tach_errors_lineout(tach_state *);
void tach_errors_type_argc(tach_state *, char *, uint32_t, uint32_t, uint32_t);
void tach_errors_type_typecheck(tach_state *, char *, uint32_t, tach_object *, uint32_t);
void tach_errors_type_typechecks(tach_state *, char *, uint32_t, tach_object *, uint32_t *, uint32_t);