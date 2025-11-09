#include "writer.h"

#include "common.h"

#include <stddef.h>
#include <stdint.h>

void writer_init(struct writer *w, uint8_t *data, size_t size)
{
    // TODO: zeroify data

    w->data = data;
    w->size = size;
    w->byte_index = 0;
    w->bit_index = 0;
}

int16_t writer_write(struct writer *w, int16_t code, uint8_t bits_count)
{
    if (!has_enough_bits(w->byte_index, w->bit_index, w->size, bits_count))
    {
        return -1;
    }

    uint32_t code32 = code;
    code32 &= (((uint32_t)1) << bits_count) - 1;
    code32 <<= w->bit_index;

    // TODO: fix index out of bounds
    uint32_t *ptr = (uint32_t *)(w->data + w->byte_index);
    *ptr |= code32;

    advance(&w->byte_index, &w->bit_index, bits_count);

    return 0;
}

size_t writer_written_bytes(struct writer const *w)
{
    return w->byte_index + (w->bit_index > 0);
}
