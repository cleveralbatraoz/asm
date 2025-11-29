bin:
	mkdir -p bin

build: bin
	nasm -f elf64 simd_strlen.asm -o bin/simd_strlen.o
	gcc -std=c17 --pedantic-errors -Wall -g -O0 -m64 main.c bin/simd_strlen.o -o bin/strlen

run:
	bin/strlen

clean:
	rm -rf bin

format:
	clang-format -i main.c
	nasmfmt simd_strlen.asm
