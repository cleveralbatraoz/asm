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

int16_t reader_next(struct reader *r, uint8_t bits_count)
{
    if (!reader_has_next(r, bits_count))
    {
        return -1;
    }

    uint32_t data = ((uint32_t)r->data[r->byte_index] << 24);

    data |= ((uint32_t)r->data[r->byte_index + 1] << 16);

    if (r->bit_index + bits_count > 16)
        data |= ((uint32_t)r->data[r->byte_index + 2] << 8);

    data <<= r->bit_index;

    uint16_t result = (data >> (32 - bits_count));

    advance(&r->byte_index, &r->bit_index, bits_count);

    return (int16_t)result;
}

bool reader_has_next(struct reader const *r, uint8_t bits_count)
{
    return has_enough_bits(r->byte_index, r->bit_index, r->size, bits_count);
}
