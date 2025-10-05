#include "print.h"

#include "flags.h"
#include "format.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint32_t monus(uint32_t a, uint32_t b)
{
    if (a < b)
    {
        return 0;
    }
    return a - b;
}

uint32_t hexval(char c)
{
    return ((uint32_t)((uint32_t)c & 0xFu)) + (((uint32_t)c >> 6) * 9u);
}

void negate(uint8_t num[SIZE])
{
    for (uint32_t i = 0; i < SIZE; i++)
    {
        num[i] = ~num[i];
    }
    for (int i = 0; i < SIZE; i++)
    {
        if (++num[i] != 0)
        {
            return;
        }
    }
}

bool divmod10(char *out, uint8_t num[SIZE])
{
    uint32_t *dwords = (uint32_t *)num;
    uint64_t remainder = 0;

    for (int i = (SIZE / 4) - 1; i >= 0; --i)
    {
        uint64_t current = (remainder << 32) | dwords[i];
        dwords[i] = (uint32_t)(current / 10);
        remainder = current % 10;
    }

    *out = '0' + remainder;

    uint32_t result = dwords[0] | dwords[1] | dwords[2] | dwords[3];
    return result == 0;
}

void store(uint8_t num[SIZE], const char *hex)
{
    for (uint32_t i = 0; i < SIZE; ++i)
    {
        num[i] = 0x00;
    }

    if (hex == NULL)
    {
        return;
    }

    bool is_negative = false;
    if (hex[0] == '-')
    {
        is_negative = true;
        ++hex;
    }

    uint8_t buf[SIZE];
    uint32_t write_idx = 0;

    const char *end = hex;
    while (*end != '\0')
    {
        ++end;
    }

    if ((end - hex) % 2 == 1)
    {
        buf[write_idx++] = hexval(*hex);
        ++hex;
    }

    while (*hex != '\0')
    {
        uint32_t high = hexval(*hex);
        uint32_t low = hexval(*(hex + 1));
        buf[write_idx++] = (high << 4) | low;
        hex += 2;
    }

    for (uint32_t i = 0; i < write_idx; ++i)
    {
        num[i] = buf[write_idx - 1 - i];
    }

    if (is_negative)
    {
        negate(num);
    }
}

uint32_t print_digits(char *out, uint8_t num[SIZE])
{
    uint32_t written = 0;

    char digits[64];
    uint32_t digit_count = 0;

    do
    {
        bool is_zero = divmod10(digits + digit_count, num);
        ++digit_count;
        if (is_zero)
        {
            break;
        }
    } while (true);

    for (uint32_t i = digit_count; i > 0; --i)
    {
        out[written++] = digits[i - 1];
    }

    return written;
}

void bytes_to_dec(char *out, const struct Format *spec, uint8_t num[SIZE])
{
    uint32_t written = 0;

    bool is_negative = num[SIZE - 1] & 0x80;
    if (is_negative)
    {
        negate(num);
    }

    char digits[4 * SIZE];
    uint32_t digit_counts = print_digits(digits, num);

    uint32_t for_sign = is_negative || HAS_SIGN_SPACE(spec->flags) || HAS_SIGN_PLUS(spec->flags);

    uint32_t prepend_spaces = 0;
    uint32_t leading_zeroes = 0;
    uint32_t append_spaces = 0;

    if (HAS_LEFT_ALIGN(spec->flags))
    {
        append_spaces = monus(spec->width, digit_counts + for_sign);
    }
    else if (HAS_ZERO_PAD(spec->flags))
    {
        leading_zeroes = monus(spec->width, digit_counts + for_sign);
    }
    else
    {
        prepend_spaces = monus(spec->width, digit_counts + for_sign);
    }

    for (uint32_t i = 0; i < prepend_spaces; ++i)
    {
        out[written++] = ' ';
    }

    if (is_negative)
    {
        out[written++] = '-';
    }
    else if (HAS_SIGN_PLUS(spec->flags))
    {
        out[written++] = '+';
    }
    else if (HAS_SIGN_SPACE(spec->flags))
    {
        out[written++] = ' ';
    }

    for (uint32_t i = 0; i < leading_zeroes; ++i)
    {
        out[written++] = '0';
    }

    for (uint32_t i = 0; i < digit_counts; ++i)
    {
        out[written++] = digits[i];
    }

    for (uint32_t i = 0; i < append_spaces; ++i)
    {
        out[written++] = ' ';
    }

    out[written] = '\0';
}

void print(char *out, const char *format, const char *hex)
{
    struct Format spec;
    parse_format(format, &spec);

    uint8_t num[SIZE];
    store(num, hex);

    bytes_to_dec(out, &spec, num);
}
