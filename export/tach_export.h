#include "tach.h"

void tach_export_object_to_file(tach_object *, tach_file *);
void tach_export_vector_to_file(tach_vector *, tach_file *);
void tach_export_table_to_file(tach_table *, tach_file *);
void tach_export_func_to_file(tach_func, tach_file *);
void tach_export_point_to_file(tach_point, tach_file *);

tach_object *tach_export_file_to_object(tach_file *);
tach_vector *tach_export_file_to_vector(tach_file *);
tach_table *tach_export_file_to_table(tach_file *);
tach_func tach_export_file_to_func(tach_file *);
tach_point tach_export_file_to_point(tach_file *);

void tach_export_program_to_file(tach_program *, tach_file *);
tach_program *tach_export_file_to_program(tach_file *);

void tach_export_state_to_file(tach_state *, tach_file *);
tach_state *tach_export_file_to_state(tach_file *);


