#include "reader.h"

#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void reader_init(struct reader *r, uint8_t const *data, size_t size)
{
    r->data = data;
    r->size = size;
    r->byte_index = 0;
    r->bit_index = 0;
}

uint16_t reader_next(struct reader *r, uint8_t bits_count)
{
    uint32_t data = ((uint32_t)r->data[r->byte_index] << 24);

    data |= ((uint32_t)r->data[r->byte_index + 1] << 16);

    if (r->bit_index + bits_count > 16)
        data |= ((uint32_t)r->data[r->byte_index + 2] << 8);

    data <<= r->bit_index;

    uint16_t result = (data >> (32 - bits_count));

    uint8_t advanced_bit_index = r->bit_index + bits_count;
    r->bit_index = advanced_bit_index % 8;
    r->byte_index += advanced_bit_index / 8;

    return result;
}

bool reader_has_next(struct reader const *r, uint8_t bits_count)
{
    return r->byte_index + (r->bit_index + bits_count + 7) / 8 <= r->size;
}
