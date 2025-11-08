#include "writer.h"

#include <stddef.h>
#include <stdint.h>

void writer_init(struct writer *w, uint8_t *data, size_t size)
{
    w->data = (int16_t *)data;
    w->size = size / 2;
    w->write_index = 0;
}

int32_t writer_write(struct writer *w, int16_t data)
{
    if (w->write_index >= w->size)
    {
        return -1;
    }

    w->data[w->write_index] = data;
    ++w->write_index;

    return 2;
}

size_t writer_written_bytes(struct writer const *w)
{
    return 2 * w->write_index;
}
