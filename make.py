import sys
sys.dont_write_bytecode = True
import subprocess
import os
import time
from lib import libnames

cc = "gcc"
opt = '3'
strip = False

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
    cmd = [cc, '-c', '-I.', '-o', './out/' + i.replace('/', '_') + '.o', i + '.c']
    subprocess.run(cmd, check=True)


cmd = ['ar', 'rcs', 'libtach.a'] + ['./out/' + i.replace('/', '_') + '.o' for i in files[:-1]]
subprocess.run(cmd, check=True)

cmd = [cc, '-o', files[-1], files[-1] + '.c', '-I.', '-L.', '-ltach', '-lgmp']
subprocess.run(cmd, check=True)

libnames.clear()
