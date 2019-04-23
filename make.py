import sys
sys.dont_write_bytecode = True
import subprocess
import os
import time
from lib import libnames

cc = 'clang'
opt = 'z'
strip = False

files = [
    'errors/tach_errors',
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
    cmd = [cc, '-O'+opt, '-c', '-I.', '-o', './out/' + i.replace('/', '_') + '.o', i + '.c']
    subprocess.run(cmd, check=True)

cmd = ['ar', 'rcs', 'libtach.a'] + ['./out/' + i.replace('/', '_') + '.o' for i in files[:-1]]
subprocess.run(cmd, check=True)

cmd = [cc, '-o', files[-1], files[-1] + '.c', '-I.', '-L.', '-ltach', '-lgmp']
subprocess.run(cmd, check=True)

# from ctest import make

cmd = ['strip', files[-1]]

libnames.clear()
