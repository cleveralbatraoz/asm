#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct reader
{
    uint8_t const *data;
    size_t size;
    size_t byte_index;
    uint8_t bit_index;
};

void reader_init(struct reader *r, uint8_t const *data, size_t size);
int16_t reader_next(struct reader *r, uint8_t bits_count);
bool reader_has_next(struct reader const *r, uint8_t bits_count);
