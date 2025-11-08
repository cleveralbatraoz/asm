#include "byte_writer.h"

#include <stddef.h>
#include <stdint.h>

void byte_writer_init(struct byte_writer *w, uint8_t *data, size_t size)
{
    w->data = data;
    w->size = size;
    w->write_index = 0;
}

int32_t byte_writer_write(struct byte_writer *w, int16_t data)
{
    if (w->write_index >= w->size)
    {
        return -1;
    }

    w->data[w->write_index] = data;
    ++w->write_index;

    return 1;
}

size_t byte_writer_written_bytes(struct byte_writer const *w)
{
    return w->write_index;
}
