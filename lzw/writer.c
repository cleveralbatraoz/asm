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

    uint32_t code32 = code;
    code32 &= (((uint32_t)1) << bits_count) - 1;

    code32 = reverse_bits(code32, bits_count);
    code32 <<= w->bit_index;

    uint32_t *ptr = (uint32_t *)(w->data + w->byte_index);
    uint32_t clear_mask = (((uint32_t)1) << bits_count) - 1;
    clear_mask <<= w->bit_index;
    *ptr &= ~clear_mask;

    *ptr |= code32;

    advance(&w->byte_index, &w->bit_index, bits_count);

    return SUCCESS;
}

size_t writer_written_bytes(struct writer const *w)
{
    return w->byte_index + (w->bit_index > 0);
}
