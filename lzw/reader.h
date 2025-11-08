#pragma once

#include <stddef.h>
#include <stdint.h>

struct reader
{
    int16_t const *data;
    size_t size;
    size_t read_index;
};

void reader_init(struct reader *r, uint8_t const *data, size_t size);
int16_t reader_read(struct reader *r);
int reader_has_next(struct reader const *r);
