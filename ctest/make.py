import sys
sys.dont_write_bytecode = True
import subprocess
import os

cc = "gcc"

cmd = [cc, '-O0', '-g', '-I./', '-L.', './ctest/test.c', '-ltach','-o', './ctest/test', '-lgmp']
print(' '.join(cmd))
subprocess.run(cmd, check=True)