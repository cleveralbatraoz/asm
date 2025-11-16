bin:
	mkdir -p bin

build: bin
	nasm -f elf32 lzw32.asm -o bin/lzw32.o
	gcc -std=c17 -Wall -g -O0 -m32 common.c decode.c main.c reader.c decode_table.c bin/lzw32.o -o bin/lzw

run:
	bin/lzw

clean:
	rm -rf bin

format:
	clang-format -i common.c decode.c decode.h reader.c reader.h decode_table.c decode_table.h
	nasmfmt lzw32.asm
