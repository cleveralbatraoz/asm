bin:
	mkdir -p bin

build: bin
	# nasm -f elf32 print.asm -o bin/print.o
	gcc -Wall -std=c17 -m32 print.c -c -o bin/print.o
	gcc -Wall -std=c17 -m32 main.c bin/print.o -o bin/test

run:
	bin/test

clean:
	rm -rf bin

format:
	clang-format -i main.c print.c print.h
	nasmfmt print.asm

