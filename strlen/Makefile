all:
	nasm -f macho64 -o strlen_dummy_impl.o src/strlen_dummy_impl.asm
	nasm -f macho64 -o strlen_vector_impl.o src/strlen_vector_impl.asm
	clang -std=c17 --pedantic-errors -O3 -o main.o -c src/main.c
	clang strlen_dummy_impl.o strlen_vector_impl.o main.o -o strlen.out

clean:
	rm -f *.o *.out
