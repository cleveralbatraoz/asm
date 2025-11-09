#pragma once

#include "byte_writer.h"
#include "common.h"

#include <stddef.h>
#include <stdint.h>

struct decode_table_entry
{
    int16_t previous_code;
    uint16_t byte : 8;
    uint16_t has_value : 1;
};

struct decode_table
{
    struct decode_table_entry entries[MAX_CODE];
    uint16_t next_code;
};

void decode_table_init(struct decode_table *table);
int decode_table_contains(struct decode_table const *table, int16_t code);
void decode_table_append(struct decode_table *table, int16_t code, uint8_t byte);
uint8_t decode_table_get_first_byte(struct decode_table const *table, int16_t code);
int16_t decode_table_write_bytes(struct byte_writer *w, int16_t code, struct decode_table const *table);
