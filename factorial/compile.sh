#!/bin/bash

nasm -f elf32 factorial.asm
gcc -m32 -no-pie -Wl,-znoexecstack -o factorial factorial.o

