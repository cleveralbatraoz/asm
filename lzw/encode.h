#pragma once

#include <stddef.h>
#include <stdint.h>

size_t lzw_encode(const uint8_t *in, size_t in_size, uint8_t *restrict out, size_t out_size);
