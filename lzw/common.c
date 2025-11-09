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
