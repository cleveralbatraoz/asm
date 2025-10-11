bin:
	mkdir -p bin

build32: bin
	nasm -f elf32 call_printf_32.asm -o bin/call_printf_32.o
	gcc -Wall -g -O0 -std=c17 -m32 bin/call_printf_32.o -o bin/call_printf_32

build64: bin
	nasm -f elf64 call_printf_64.asm -o bin/call_printf_64.o
	gcc -no-pie -Wall -g -O0 -std=c17 -m64 bin/call_printf_64.o -o bin/call_printf_64

run32:
	bin/call_printf_32

run64:
	bin/call_printf_64

clean:
	rm -rf bin

format:
	nasmfmt call_printf_32.asm call_printf_64.asm
