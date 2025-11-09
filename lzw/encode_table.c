#include "encode_table.h"

#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void encode_table_init(struct encode_table *table)
{
    for (int16_t i = 0; i < MAX_CODE; ++i)
    {
        for (int16_t j = 0; j < 256; ++j)
        {
            table->entries[i][j].next_sequence_code = 0;
            table->entries[i][j].has_value = 0;
        }
    }
    table->next_code = FIRST_CODE;
}

bool encode_table_contains(struct encode_table const *table, int16_t code, uint8_t byte)
{
    if (!is_valid_code(code))
    {
        return false;
    }
    return table->entries[code][byte].has_value == 1;
}

int16_t encode_table_append(struct encode_table *table, int16_t code, uint8_t byte)
{
    if (!is_valid_code(code))
    {
        return INVALID_CODE;
    }

    table->entries[code][byte].next_sequence_code = table->next_code;
    table->entries[code][byte].has_value = 1;

    ++table->next_code;

    return SUCCESS;
}

int16_t encode_table_get_next_sequence_code(struct encode_table const *table, int16_t code, uint8_t byte)
{
    if (!is_valid_code(code))
    {
        return INVALID_CODE;
    }

    if (table->entries[code][byte].has_value == 0)
    {
        return ENCODE_TABLE_INVARIANT_VIOLATION;
    }

    return table->entries[code][byte].next_sequence_code;
}
