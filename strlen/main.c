#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <x86intrin.h>

extern size_t simd_strlen(const char *);

void test(const char *str, bool need_print)
{
    unsigned aux;
    uint64_t libc_start, libc_end, simd_start, simd_end;
    size_t libc_len, simd_len;

    aux = 0;
    libc_start = __rdtscp(&aux);
    libc_len = strlen(str);
    libc_end = __rdtscp(&aux);

    aux = 0;
    simd_start = __rdtscp(&aux);
    simd_len = simd_strlen(str);
    simd_end = __rdtscp(&aux);

    if (need_print)
    {
        printf("%zu, %zu, libc=%zu, simd=%zu\n", libc_len, simd_len, libc_end - libc_start, simd_end - simd_start);
    }
}

int main()
{
    char str[1234567 + 1];
    memset(str, 'A', 1234567);
    str[1234567] = '\0';

    for (size_t i = 0; i < 100000; ++i)
    {
        test(str, false);
    }

    test(str, true);

    return 0;
}
