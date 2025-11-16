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

bool error(int16_t code);

char const *error_message(int16_t error_code);
