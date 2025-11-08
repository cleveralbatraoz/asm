bin:
	mkdir -p bin

build: bin
	nasm -f elf32 fpu_atan.asm -o bin/fpu_atan.o
	nasm -f elf32 scalar_atan.asm -o bin/scalar_atan.o
	nasm -f elf32 simd_atan.asm -o bin/simd_atan.o
	gcc -Wall -g -O0 -std=c17 -m32 main.c bin/fpu_atan.o bin/scalar_atan.o bin/simd_atan.o -o bin/atan

build-bench: bin
	nasm -f elf32 fpu_atan.asm -o bin/fpu_atan.o
	nasm -f elf32 scalar_atan.asm -o bin/scalar_atan.o
	nasm -f elf32 simd_atan.asm -o bin/simd_atan.o
	gcc -Wall -O2 -std=c17 -m32 benchmark.c bin/fpu_atan.o bin/scalar_atan.o bin/simd_atan.o -o bin/bench -lrt

run:
	bin/atan

run-bench:
	bin/bench

clean:
	rm -rf bin

format:
	nasmfmt fpu_atan.asm scalar_atan.asm simd_atan.asm
	clang-format -i main.c benchmark.c
