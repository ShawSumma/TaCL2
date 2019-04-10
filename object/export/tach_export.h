#include "tach.h"

void tach_export_object_to_file(tach_object *, FILE *);
void tach_export_vector_to_file(tach_vector *, FILE *);
void tach_export_table_to_file(tach_table *, FILE *);

tach_object *tach_export_file_to_object(FILE *F);
tach_vector *tach_export_file_to_vector(FILE *F);
tach_table *tach_export_file_to_table(FILE *F);

