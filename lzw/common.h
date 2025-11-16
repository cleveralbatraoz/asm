#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_CODE 4096
#define MAX_BITS_COUNT 12

#define CLEAR_CODE 256
#define END_OF_INFORMATION 257
#define FIRST_CODE 258

#define SUCCESS 0
#define INVALID_CODE -1
#define TABLE_OVERFLOW -2
#define DECODE_TABLE_INVARIANT_VIOLATION -3
#define INPUT_INVARIANT_VIOLATION -4
#define BYTE_WRITER_OVERFLOW -5

bool is_power_of_two(uint16_t value);

bool has_enough_bits(size_t byte_index, size_t bit_index, size_t size, uint8_t bits_count);

void advance(size_t *byte_index, uint8_t *bit_index, uint8_t bits_count);

bool error(int16_t code);

char const *error_message(int16_t error_code);

bool is_valid_code(int16_t code);
