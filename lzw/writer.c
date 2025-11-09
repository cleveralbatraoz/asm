#include "writer.h"

#include "common.h"

#include <stddef.h>
#include <stdint.h>

void writer_init(struct writer *w, uint8_t *data, size_t size)
{
    w->data = data;
    w->size = size;
    w->byte_index = 0;
    w->bit_index = 0;
}

int16_t writer_write(struct writer *w, int16_t code, uint8_t bits_count)
{
    if (!has_enough_bits(w->byte_index, w->bit_index, w->size, bits_count))
    {
        return WRITER_OVERFLOW;
    }

    uint32_t data = (uint32_t)code << (32 - bits_count - w->bit_index);

    uint8_t bits_in_first = 8 - w->bit_index;

    uint8_t mask1 = ((1 << bits_in_first) - 1) << (8 - w->bit_index - bits_in_first);
    w->data[w->byte_index] = (w->data[w->byte_index] & ~mask1) | ((data >> 24) & mask1);

    uint8_t bits_in_second = bits_count - bits_in_first;
    if (bits_in_second > 8)
        bits_in_second = 8;

    uint8_t mask2 = (1 << bits_in_second) - 1;
    mask2 <<= (8 - bits_in_second);
    w->data[w->byte_index + 1] = (w->data[w->byte_index + 1] & ~mask2) | ((data >> 16) & mask2);

    if (bits_count > bits_in_first + 8)
    {
        uint8_t bits_in_third = bits_count - bits_in_first - 8;
        uint8_t mask3 = (1 << bits_in_third) - 1;
        mask3 <<= (8 - bits_in_third);
        w->data[w->byte_index + 2] = (w->data[w->byte_index + 2] & ~mask3) | ((data >> 8) & mask3);
    }

    advance(&w->byte_index, &w->bit_index, bits_count);

    return SUCCESS;
}

size_t writer_written_bytes(struct writer const *w)
{
    return w->byte_index + (w->bit_index > 0);
}
