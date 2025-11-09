#include "decode_table.h"

#include "byte_writer.h"

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
    table->next_code = 256;
}

int decode_table_contains(struct decode_table const *table, int16_t code)
{
    return code < 256 || table->entries[code].has_value;
}

void decode_table_append(struct decode_table *table, int16_t code, uint8_t byte)
{
    if (table->next_code >= MAX_CODE)
    {
        return;
    }

    table->entries[table->next_code].previous_code = code;
    table->entries[table->next_code].byte = byte;
    table->entries[table->next_code].has_value = 1;
    ++table->next_code;
}

uint8_t decode_table_get_first_byte(struct decode_table const *table, int16_t code)
{
    while (code >= 256)
    {
        code = table->entries[code].previous_code;
    }

    return code;
}

int16_t decode_table_write_bytes(struct byte_writer *w, int16_t code, struct decode_table const *table)
{
    if (code < 256)
    {
        return byte_writer_write(w, code);
    }

    if (decode_table_write_bytes(w, table->entries[code].previous_code, table) == -1)
    {
        return -1;
    }

    return byte_writer_write(w, table->entries[code].byte);
}
