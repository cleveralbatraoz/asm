bin:
	mkdir -p bin

build: bin
	gcc -std=c17 -Wall -g -O0 -m32 common.c decode.c encode.c main.c reader.c writer.c decode_table.c byte_writer.c -o bin/lzw

run:
	bin/lzw

clean:
	rm -rf bin

format:
	clang-format -i common.c decode.c decode.h encode.c encode.h reader.c reader.h writer.c writer.h decode_table.c decode_table.h byte_writer.c byte_writer.h
