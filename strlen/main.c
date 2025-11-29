#include <stddef.h>
#include <stdio.h>

extern size_t simd_strlen(const char *);

int main()
{
    const char str[1024] = "hello, world, hello, world, hello, world, hello, world";
    size_t len = simd_strlen(str);
    printf("%zu\n", len);
    return 0;
}
