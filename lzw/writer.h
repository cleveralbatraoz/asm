#pragma once

#include <stddef.h>
#include <stdint.h>

struct writer
{
    uint8_t *data;
    size_t size;
    size_t byte_index;
    uint8_t bit_index;
};

void writer_init(struct writer *w, uint8_t *data, size_t size);
int16_t writer_write(struct writer *w, int16_t code, uint8_t bits_count);
size_t writer_written_bytes(struct writer const *w);
