bin:
	mkdir -p bin

build32: bin
	nasm -f elf32 add_32.asm -o bin/add_32.o
	gcc -std=c17 -g -O0 -m32 call_add.c bin/add_32.o -o bin/call_add_32

run32:
	bin/call_add_32

build64: bin
	nasm -f elf64 add_64.asm -o bin/add_64.o
	gcc -std=c17 -g -O0 -m64 call_add.c bin/add_64.o -o bin/call_add_64

build-cpp64: bin
	nasm -f elf64 add_64.asm -o bin/add_64.o
	g++ -Wl,--no-demangle -std=c++20 -g -O0 -m64 call_add.cpp bin/add_64.o -o bin/call_add_64

run64:
	bin/call_add_64

format:
	nasmfmt add_32.asm add_64.asm
	clang-format -i call_add.c call_add.cpp

clean:
	rm -rf bin
