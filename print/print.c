#include "print.h"

#include "flags.h"
#include "format.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

static uint32_t monus(uint32_t a, uint32_t b)
{
    if (a < b)
    {
        return 0;
    }
    return a - b;
}

static uint32_t hexval(char c)
{
    return ((uint32_t)((uint32_t)c & 0xFu)) + (((uint32_t)c >> 6) * 9u);
}

static void be_negate(uint8_t num[SIZE])
{
    uint32_t carry = 1;
    for (uint32_t i = SIZE; i > 0; --i)
    {
        num[i - 1] = ~num[i - 1];
        uint32_t sum = (uint32_t)num[i - 1] + carry;
        num[i - 1] = (uint8_t)sum;
        carry = sum >> 8;
    }
}

bool divmod10(char *out, uint8_t num[SIZE])
{
    uint32_t remainder = 0;
    bool is_zero = true;
    for (uint32_t i = 0; i < SIZE; ++i)
    {
        is_zero = is_zero && (num[i] == 0x0);
        uint32_t current = (remainder << 8) | num[i];
        num[i] = (uint8_t)(current / 10);
        remainder = current % 10;
    }
    *out = '0' + remainder;
    return is_zero;
}

static bool store(uint8_t num[SIZE], const char *hex)
{
    for (uint32_t i = 0; i < SIZE; ++i)
    {
        num[i] = 0x00;
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
        num[SIZE - write_idx + i] = buf[i];
    }

    if (num[0] & 0x80)
    {
        is_negative = !is_negative;
        be_negate(num);
    }

    return is_negative;
}

static uint32_t print_digits(char *out, uint8_t num[SIZE])
{
    uint32_t written = 0;

    char digits[64];
    uint32_t digit_count = 0;
    while (!divmod10(digits + digit_count, num))
    {
        ++digit_count;
    }
    if (digit_count == 0)
    {
        digits[digit_count++] = '0';
    }

    for (uint32_t i = digit_count; i > 0; --i)
    {
        out[written++] = digits[i - 1];
    }

    return written;
}

static void bytes_to_dec(char *out, const struct Format *spec, uint8_t num[SIZE], bool is_negative)
{
    uint32_t written = 0;

    char digits[4 * SIZE];
    uint32_t digit_counts = print_digits(digits, num);

    uint32_t for_sign = is_negative || isSignSpace(spec->flags) || isSignAll(spec->flags);

    uint32_t prepend_spaces = 0;
    uint32_t leading_zeroes = 0;
    uint32_t append_spaces = 0;

    if (isAlignLeft(spec->flags))
    {
        append_spaces = monus(spec->width, digit_counts + for_sign);
    }
    else if (isLeadingZeroesEnable(spec->flags))
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
    else if (isSignAll(spec->flags))
    {
        out[written++] = '+';
    }
    else if (isSignSpace(spec->flags))
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
    bool is_negative = store(num, hex);

    bytes_to_dec(out, &spec, num, is_negative);
}
