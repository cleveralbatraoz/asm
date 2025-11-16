#include "writer.h"

#include "common.h"

#include <stddef.h>
#include <stdint.h>

void writer_init(struct writer *w, uint8_t *data, size_t size)
{
    w->data = data;
    w->size = size;
    w->write_index = 0;
}

int16_t writer_write(struct writer *w, uint8_t byte)
{
    if (w->write_index >= w->size)
    {
        return WRITER_OVERFLOW;
    }

    w->data[w->write_index] = byte;
    ++w->write_index;

    return SUCCESS;
}

size_t writer_written_bytes(struct writer const *w)
{
    return w->write_index;
}
