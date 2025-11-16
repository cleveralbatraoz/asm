#include "decode_table.h"

#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ENTRY_GET_PREVIOUS_CODE(entry) ((int16_t)((entry) & 0xFFF))
#define ENTRY_GET_BYTE(entry) ((uint8_t)(((entry) >> 12) & 0xFF))
#define ENTRY_GET_HAS_VALUE(entry) ((bool)(((entry) >> 20) & 0x1))
#define ENTRY_PACK(previous_code, byte, has_value)                                                                     \
    ((uint32_t)((previous_code) & 0xFFF) | (uint32_t)(((byte) & 0xFF) << 12) | (uint32_t)(((has_value) & 0x1) << 20))

void decode_table_init(struct decode_table *table)
{
    for (size_t i = 0; i < 256; i++)
    {
        table->entries[i] = ENTRY_PACK(0, 0, 1);
    }
    for (size_t i = 256; i < MAX_CODE; i++)
    {
        table->entries[i] = 0;
    }
    table->next_code = FIRST_CODE;
}

bool decode_table_contains(struct decode_table const *table, uint16_t code)
{
    return ENTRY_GET_HAS_VALUE(table->entries[code]);
}

void decode_table_append(struct decode_table *table, int16_t code, uint8_t byte)
{
    table->entries[table->next_code] = ENTRY_PACK(code, byte, 1);
    ++table->next_code;
}

uint8_t decode_table_get_first_byte(struct decode_table const *table, uint16_t code)
{
    while (code >= FIRST_CODE)
    {
        uint32_t entry = table->entries[code];
        code = ENTRY_GET_PREVIOUS_CODE(entry);
    }

    return code;
}

uint16_t decode_table_write_bytes(uint8_t *w, uint16_t code, struct decode_table const *table)
{
    if (code < FIRST_CODE)
    {
        w[0] = code;
        return 1;
    }

    uint32_t entry = table->entries[code];

    uint16_t bytes = decode_table_write_bytes(w, ENTRY_GET_PREVIOUS_CODE(entry), table);

    w[bytes] = ENTRY_GET_BYTE(entry);

    return bytes + 1;
}
