#include "byte_writer.h"

#include "common.h"

#include <stddef.h>
#include <stdint.h>

void byte_writer_init(struct byte_writer *w, uint8_t *data, size_t size)
{
    w->data = data;
    w->size = size;
    w->write_index = 0;
}

int16_t byte_writer_write(struct byte_writer *w, uint8_t byte)
{
    if (w->write_index >= w->size)
    {
        return BYTE_WRITER_OVERFLOW;
    }

    w->data[w->write_index] = byte;
    ++w->write_index;

    return SUCCESS;
}

size_t byte_writer_written_bytes(struct byte_writer const *w)
{
    return w->write_index;
}
