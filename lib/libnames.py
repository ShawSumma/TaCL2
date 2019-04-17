import os
def main():
    names = open('lib/names.txt', 'r').read().split('\n')
    convh = open('lib/tach_lib.h', 'w')
    nameh = open('lib/tach_libname.h', 'w')
    convc = open('lib/tach_conv.c', 'w')

    convh.write('#pragma once\n')
    convh.write('#include "tach.h"\n\n')
    convh.write('char *tach_func_to_name(tach_func);\n')
    convh.write('tach_func tach_name_to_func(char *);\n')

    for i in names:
        convd = i.replace(' ', '_').replace('-', '_')
        convh.write(f'tach_object *tach_lib_{convd}(tach_state *, uint32_t count, tach_object **args);\n')

    convc.write('#include "tach.h"\n\n')
    convc.write('char *tach_func_to_name(tach_func fn) {\n')
    for i in names:
        convd = i.replace(' ', '_').replace('-', '_')
        convc.write('    if (fn == tach_lib_%s) {\n' % convd)
        convc.write('        return "%s";\n' % i)
        convc.write('    }\n')
    convc.write('    fprintf(stderr, "cannot deserialize that");\n')
    convc.write('    exit(1);\n')
    convc.write('}\n')
    convc.write('\n')

    nameh.write('#pragma once\n')
    nameh.write('#include "tach.h"\n\n')
    nameh.write('enum tach_libname {\n')
    convc.write('tach_libname tach_func_to_libname(tach_func fn) {\n')
    for pl, i in enumerate(names):
        convd = i.replace(' ', '_').replace('-', '_')
        nameh.write('    tach_libname_%s,\n' % convd);
        convc.write('    if (fn == tach_lib_%s) {\n' % convd)
        convc.write('        return tach_libname_%s;\n' % convd)
        convc.write('    }\n')
    nameh.write('};\n')
    nameh.write('typedef enum tach_libname tach_libname;\n')
    convc.write('    fprintf(stderr, "cannot deserialize that");\n')
    convc.write('    exit(1);\n')
    convc.write('}\n')
    convc.write('\n')
        
    convc.write('tach_func tach_name_to_func(char *str) {\n')
    for i in names:
        convd = i.replace(' ', '_').replace('-', '_')
        convc.write('    if (!strcmp(str, "%s")) {\n' % i)
        convc.write('        return tach_lib_%s;\n' % convd)
        convc.write('    }\n')
    convc.write('    fprintf(stderr, "cannot serialize that function");\n')
    convc.write('    exit(1);\n')
    convc.write('}\n')
def clear():
    os.remove('lib/tach_lib.h')
    os.remove('lib/tach_conv.c')
    os.remove('lib/tach_libname.h')