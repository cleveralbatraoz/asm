#pragma once

#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct decode_table
{
    uint32_t entries[MAX_CODE];
    int16_t next_code;
};

extern void decode_table_init(struct decode_table *table);
extern bool decode_table_contains(struct decode_table const *table, uint16_t code);
extern void decode_table_append(struct decode_table *table, int16_t code, uint8_t byte);
extern uint8_t decode_table_get_first_byte(struct decode_table const *table, uint16_t code);
extern uint16_t decode_table_write_bytes(uint8_t *w, uint16_t code, struct decode_table const *table);
