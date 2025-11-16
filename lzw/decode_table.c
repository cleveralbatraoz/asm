#include "decode_table.h"

#include "writer.h"
#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void decode_table_init(struct decode_table *table)
{
    for (size_t i = 0; i < MAX_CODE; i++)
    {
        table->entries[i].previous_code = 0;
        table->entries[i].byte = 0;
        table->entries[i].has_value = 0;
    }
    table->next_code = FIRST_CODE;
}

bool decode_table_contains(struct decode_table const *table, int16_t code)
{
    if (!is_valid_code(code))
    {
        return false;
    }
    return code < FIRST_CODE || table->entries[code].has_value == 1;
}

int16_t decode_table_append(struct decode_table *table, int16_t code, uint8_t byte)
{
    if (!is_valid_code(code))
    {
        return INVALID_CODE;
    }

    if (table->next_code >= MAX_CODE)
    {
        return TABLE_OVERFLOW;
    }

    table->entries[table->next_code].previous_code = code;
    table->entries[table->next_code].byte = byte;
    table->entries[table->next_code].has_value = 1;

    ++table->next_code;

    return table->next_code;
}

int16_t decode_table_get_first_byte(struct decode_table const *table, int16_t code)
{
    if (!is_valid_code(code))
    {
        return INVALID_CODE;
    }

    while (code >= FIRST_CODE)
    {
        if (!is_valid_code(code))
        {
            return INVALID_CODE;
        }

        if (table->entries[code].has_value == 0)
        {
            return DECODE_TABLE_INVARIANT_VIOLATION;
        }

        code = table->entries[code].previous_code;
    }

    if (!is_valid_code(code))
    {
        return INVALID_CODE;
    }

    return code;
}

int16_t decode_table_write_bytes(struct writer *w, int16_t code, struct decode_table const *table)
{
    if (!is_valid_code(code))
    {
        return INVALID_CODE;
    }

    if (code < FIRST_CODE)
    {
        return writer_write(w, code);
    }

    if (table->entries[code].has_value == 0)
    {
        return DECODE_TABLE_INVARIANT_VIOLATION;
    }

    int16_t result = decode_table_write_bytes(w, table->entries[code].previous_code, table);

    if (error(result))
    {
        return result;
    }

    return writer_write(w, table->entries[code].byte);
}
