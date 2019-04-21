import sys
sys.dont_write_bytecode = True
import subprocess
import os
import time
from lib import libnames

cc = "clang"
opt = 'g'
strip = True

files = [
    'errors/tach_errors',
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
    'tacl/tach_bytecomp',
    'tacl/tach_memory',
    'vm/tach_vm',
    'tach'
]

libnames.main()

for i in files:
    cmd = [cc, '-c', '-I./', '-O' + opt, '-std=c99', '-o', 'out/' + '_'.join(i.split('/')) + '.o', i + '.c']
    subprocess.run(cmd, check=True)
    print(' '.join(cmd))
lis = [cc, '-o', './tachvm', '-lgmp', '-O3']
for i in files:
    lis.append('out/' + '_'.join(i.split('/')) + '.o')
print(' '.join(lis[:5]), '\\\n  ' + ' \\\n  '.join(lis[5:]))
subprocess.run(lis, check=True)
for i in files:
    cmd = ['rm', 'out/' + '_'.join(i.split('/')) + '.o']
    print(' '.join(cmd))

libnames.clear()
if strip:
    cmd = ['strip', './tachvm']
    subprocess.run(cmd, check=True)
    print(' '.join(cmd))
