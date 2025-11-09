#pragma once

#define MAX_CODE 4096
#define MAX_BITS_COUNT 12

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool is_power_of_two(uint16_t value);
bool has_enough_bits(size_t byte_index, size_t bit_index, size_t size, uint8_t bits_count);
void advance(size_t *byte_index, uint8_t *bit_index, uint8_t bits_count);
