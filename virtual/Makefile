all: cpp asm link

cpp:
	g++ -m32 -std=c++20 -pedantic-errors -c virtual.cpp

asm:
	nasm -f elf32 call_f.asm

link:
	g++ -m32 call_f.o virtual.o -o call_virtual.out

clean:
	rm -f *.o *.out

run:
	./call_virtual.out
