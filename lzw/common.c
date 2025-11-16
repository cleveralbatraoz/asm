#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool is_power_of_two(uint16_t value)
{
    return (value & (value - 1)) == 0;
}

bool has_enough_bits(size_t byte_index, size_t bit_index, size_t size, uint8_t bits_count)
{
    return byte_index + (bit_index + bits_count + 7) / 8 <= size;
}

void advance(size_t *byte_index, uint8_t *bit_index, uint8_t bits_count)
{
    uint8_t advanced_bit_index = *bit_index + bits_count;
    *bit_index = advanced_bit_index % 8;
    *byte_index += advanced_bit_index / 8;
}

bool error(int16_t code)
{
    return code < 0;
}

char const *error_message(int16_t error_code)
{
    switch (error_code)
    {
    case INVALID_CODE: {
        return "Invalid code";
    }
    case TABLE_OVERFLOW: {
        return "Table overflow";
    }
    case DECODE_TABLE_INVARIANT_VIOLATION: {
        return "Table invariant violation";
    }
    case INPUT_INVARIANT_VIOLATION: {
        return "Input invariant violation";
    }
    case BYTE_WRITER_OVERFLOW: {
        return "Byte writer overflow";
    }
    case WRITER_OVERFLOW: {
        return "Writer overflow";
    }
    }
    return "Unknown error";
}

bool is_valid_code(int16_t code)
{
    return code >= 0 && code < MAX_CODE && code != CLEAR_CODE && code != END_OF_INFORMATION;
}
