bin:
	mkdir -p bin

build-asm: bin
	nasm -f elf32 lzw32.asm -o bin/lzw32.o
	gcc -std=c17 -Wall -g -O0 -m32 common.c decode.c main.c bin/lzw32.o -o bin/lzw-asm

build-c: bin
	gcc -std=c17 -Wall -g -O0 -m32 common.c common_c.c decode.c decode_table.c reader.c main.c -o bin/lzw-c

run:
	bin/lzw-asm

clean:
	rm -rf bin

format:
	clang-format -i common.c decode.c decode.h reader.h decode_table.h reader.c decode_table.c
	nasmfmt lzw32.asm
