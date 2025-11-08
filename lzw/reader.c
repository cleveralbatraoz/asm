#include "reader.h"

#include <stddef.h>
#include <stdint.h>

void reader_init(struct reader *r, uint8_t const *data, size_t size)
{
    r->data = (int16_t *)data;
    r->size = size / 2;
    r->read_index = 0;
}

int16_t reader_read(struct reader *r)
{
    return r->data[r->read_index++];
}

int reader_has_next(struct reader const *r)
{
    return r->read_index < r->size;
}
