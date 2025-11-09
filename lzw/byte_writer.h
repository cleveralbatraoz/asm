#pragma once

#include <stddef.h>
#include <stdint.h>

struct byte_writer
{
    uint8_t *data;
    size_t size;
    size_t write_index;
};

void byte_writer_init(struct byte_writer *w, uint8_t *data, size_t size);
int16_t byte_writer_write(struct byte_writer *w, uint8_t byte);
size_t byte_writer_written_bytes(struct byte_writer const *w);
