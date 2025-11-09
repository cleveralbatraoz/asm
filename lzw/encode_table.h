#pragma once

#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct encode_table_entry
{
    int16_t next_sequence_code;
    int16_t has_value : 2;
};

struct encode_table
{
    struct encode_table_entry entries[MAX_CODE][256];
    int16_t next_code;
};

void encode_table_init(struct encode_table *table);
bool encode_table_contains(struct encode_table const *table, int16_t code, uint8_t byte);
int16_t encode_table_append(struct encode_table *table, int16_t code, uint8_t byte);
int16_t encode_table_get_next_sequence_code(struct encode_table const *table, int16_t code, uint8_t byte);
