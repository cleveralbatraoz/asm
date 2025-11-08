#pragma once

#include <stddef.h>
#include <stdint.h>

struct writer
{
    int16_t *data;
    size_t size;
    size_t write_index;
};

void writer_init(struct writer *w, uint8_t *data, size_t size);
int32_t writer_write(struct writer *w, int16_t data);
size_t writer_written_bytes(struct writer const *w);
