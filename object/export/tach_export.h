#include "tach.h"

void tach_export_object_to_file(tach_object *, FILE *);
void tach_export_vector_to_file(tach_vector *, FILE *);
void tach_export_table_to_file(tach_table *, FILE *);
void tach_export_func_to_file(tach_func, FILE *);
void tach_export_point_to_file(tach_point, FILE *);


tach_object *tach_export_file_to_object(FILE *);
tach_vector *tach_export_file_to_vector(FILE *);
tach_table *tach_export_file_to_table(FILE *);
tach_func tach_export_file_to_func(FILE *);
tach_point tach_export_file_to_point(FILE *);

void tach_export_program_to_file(tach_program *prog, FILE *);


