#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

size_t strlen_dummy_impl(const char *);
size_t strlen_vector_impl(const char *);

static clock_t measure(const char *restrict str, size_t (*strlen_impl)(const char *), size_t *restrict result)
{
    clock_t start = clock();
    *result = strlen_impl(str);
    clock_t end = clock();
    return end - start;
}

static void test(size_t size)
{
    char *str = malloc(size + 16);
    memset(str, 1, size + 15);
    str[size + 15] = '\0';
    size_t std_impl, dummy_impl, vector_impl;
    clock_t std_time = measure(str, strlen, &std_impl);
    clock_t dummy_time = measure(str, strlen_dummy_impl, &dummy_impl);
    clock_t vector_time = measure(str, strlen_vector_impl, &vector_impl);
    free(str);
    assert(std_impl == dummy_impl);
    assert(std_impl == vector_impl);
    puts("----------------------------------------");
    printf("size = %zu\n", size);
    printf("std time = %.6f\n", (double)std_time / CLOCKS_PER_SEC);
    printf("dummy time = %.6f\n", (double)dummy_time / CLOCKS_PER_SEC);
    printf("vector time = %.6f\n", (double)vector_time / CLOCKS_PER_SEC);
    puts("----------------------------------------");
}

int main(void)
{
    test(0); // special case
    for (uint8_t pow = 0; pow <= 10; ++pow)
    {
        static size_t size = 1;
        test(size);
        size *= 10;
    }
    return 0;
}
