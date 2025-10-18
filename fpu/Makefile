bin:
	mkdir -p bin

build: bin
	nasm -f elf32 fpuatan.asm -o bin/fpuatan.o
	gcc -Wall -g -O0 -std=c17 -m32 main.c bin/fpuatan.o -o bin/fpuatan

run:
	bin/fpuatan

clean:
	rm -rf bin

format:
	nasmfmt fpuatan.asm
	clang-format -i main.c
