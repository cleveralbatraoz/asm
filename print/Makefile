bin:
	mkdir -p bin

build: bin
	nasm -f elf32 print.asm -o bin/print.o
	gcc -Wall -std=c17 -m32 -g -I.. main.c bin/print.o -o bin/test

build-c: bin
	gcc -Wall -std=c17 -m32 -g -I.. format.c print.c main.c -o bin/test

run:
	bin/test

clean:
	rm -rf bin

format:
	clang-format -i print.h flags.h format.h format.c print.c main.c
	nasmfmt print.asm
