#!/usr/bin/python3
import subprocess
import sys

output = subprocess.check_output(['java', '-jar', 'casmParser/Main.jar', 'example.casm']).decode('utf-8')

f = open('vars.cpp',mode='r')
lines = f.readlines()
for i in range(0,len(lines)):
    if "define program_bytes" in lines[i]:
        lines[i] = f"#define program_bytes {output}"
        print(lines[i])
f.close()
f = open('vars.cpp',mode='w')
for i in range(0,len(lines)):
    f.write(lines[i])
f.close()