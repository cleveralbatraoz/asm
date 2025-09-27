#include "print.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum AlignFlags
{
    fAlignLeft = 1,
};

enum SignFlags
{
    fSignBoth = 1,
    fSignSpace = 2,
};

enum PrependFlags
{
    fPrependZeroes = 1,
};

struct Format
{
    enum AlignFlags align_flags;
    enum SignFlags sign_flags;
    enum PrependFlags prepend_flags;
    uint32_t width;
};

static uint32_t hexval(char c)
{
    return ((uint32_t)((uint32_t)c & 0xFu)) + (((uint32_t)c >> 6) * 9u);
}

static int is_zero(const uint8_t num[SIZE])
{
    int i;
    for (i = 0; i < SIZE; ++i)
    {
        if (num[i] != 0)
        {
            return 0;
        }
    }
    return 1;
}

static void invert(uint8_t num[SIZE])
{
    for (uint32_t i = 0; i < SIZE; ++i)
    {
        num[i] = ~num[i];
    }
}

static void add(uint8_t num[SIZE], uint8_t digit)
{
    uint32_t carry = digit;
    for (uint32_t i = 0; i < SIZE; ++i)
    {
        uint32_t cur = (uint32_t)num[i] + carry;
        num[i] = (uint8_t)cur;
        carry = cur >> 8;
        if (carry == 0)
        {
            break;
        }
    }
}

static void muladd16(uint8_t num[SIZE], uint32_t digit)
{
    uint32_t carry = digit;
    for (uint32_t i = 0; i < SIZE; ++i)
    {
        uint32_t cur = ((uint32_t)num[i] << 4) + carry;
        num[i] = (uint8_t)(cur & 0xFFu);
        carry = cur >> 8;
    }
}

static uint32_t divmod10(uint8_t num[SIZE])
{
    uint32_t r = 0;
    for (uint32_t i = SIZE; i > 0; --i)
    {
        uint32_t cur = (r << 8) | num[i - 1];
        num[i - 1] = (uint8_t)(cur / 10);
        r = cur % 10;
    }
    return r;
}

static void hex_to_i128(uint8_t num[SIZE], const char *hex)
{
    if (hex == NULL)
    {
        return;
    }

    for (uint32_t i = 0; i < SIZE; ++i)
    {
        num[i] = 0;
    }

    bool sign = false;
    if (hex[0] == '-')
    {
        sign = true;
        ++hex;
    }

    while (*hex != '\0')
    {
        uint32_t v = hexval(*hex);
        muladd16(num, v);
        ++hex;
    }

    if (sign)
    {
        invert(num);
        add(num, 1);
    }
}

static void i128_to_dec(char *out, const uint8_t in[SIZE])
{
    uint8_t num[SIZE];
    char buf[64];
    int n = 0;
    int i, j;

    for (i = 0; i < SIZE; ++i)
    {
        num[i] = in[i];
    }

    bool sign = false;
    if (num[SIZE - 1] & 0x08)
    {
        invert(num);
        add(num, 1);
        sign = true;
    }

    do
    {
        buf[n] = (char)('0' + divmod10(num));
        ++n;
    } while (!is_zero(num));

    if (sign)
    {
        buf[n] = '-';
        ++n;
    }

    for (j = 0; j < n; ++j)
    {
        out[j] = buf[n - 1 - j];
    }
    out[n] = '\0';
}

static void parse_format(const char *fmt, struct Format *spec)
{
    spec->align_flags = 0;
    spec->sign_flags = 0;
    spec->prepend_flags = 0;
    spec->width = 0;

    if (*fmt != '%')
    {
        return;
    }

    ++fmt;

    bool run = true;
    while (run && *fmt != '\0')
    {
        switch (*fmt)
        {
        case '-': {
            spec->align_flags = fAlignLeft;
            break;
        }
        case '+': {
            spec->sign_flags = fSignBoth;
            break;
        }
        case ' ': {
            spec->sign_flags = fSignSpace;
            break;
        }
        case '0': {
            spec->prepend_flags = fPrependZeroes;
            break;
        }
        default: {
            run = false;
        }
        }

        ++fmt;
    }

    while ('0' <= *fmt && *fmt <= '9')
    {
        spec->width = spec->width * 10 + *fmt - '0';
        ++fmt;
    }
}

void print(char *out, const char *format, const char *hex)
{
    uint8_t num[SIZE];
    hex_to_i128(num, hex);

    struct Format spec;
    parse_format(format, &spec);

    i128_to_dec(out, num);
}
