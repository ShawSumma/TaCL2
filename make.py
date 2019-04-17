import sys
sys.dont_write_bytecode = True
import subprocess
import os
import time
from lib import libnames
cc = "clang"
files = [
    'export/tach_export',
    'files/tach_files',
    'lib/tach_conv',
    'lib/tach_clib',
    'lib/tach_lib',
    'object/tach_memory',
    'object/tach_number',
    'object/tach_object',
    'tacl/tach_ast_print',
    'tacl/tach_ast',
    'tacl/tach_comp',
    'tacl/tach_memory',
    'vm/tach_vm',
    'tach'
]

libnames.main()

for i in files:
    cmd = [cc, '-c', '-I./', '-o', 'out/' + '_'.join(i.split('/')) + '.o', i + '.c', '-O3']
    subprocess.run(cmd, check=True)
    print(' '.join(cmd))
lis = [cc, '-o', './tachvm', '-lgmp', '-O3']
for i in files:
    lis.append('out/' + '_'.join(i.split('/')) + '.o')
print(' '.join(lis[:5]), '\\\n  ' + ' \\\n  '.join(lis[5:]))
subprocess.run(lis, check=True)
for i in files:
    cmd = ['rm', 'out/' + '_'.join(i.split('/')) + '.o']
    subprocess.run(cmd, check=True)
    print(' '.join(cmd))

libnames.clear()