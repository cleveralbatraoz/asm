#include "decode.h"

#include "common.h"
#include "decode_table.h"
#include "reader.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern bool is_power_of_two(uint32_t value);

size_t lzw_decode(const uint8_t *in, size_t in_size, uint8_t *restrict out, size_t out_size)
{
    if (in == NULL)
    {
        return -1;
    }
    if (out == NULL)
    {
        if (in_size == 0)
        {
            return 0;
        }

        return -1;
    }

    struct decode_table table;
    decode_table_init(&table);

    struct reader r;
    reader_init(&r, in, in_size);

    uint8_t *w = out;
    uint8_t *out_start = out;

    uint8_t bits_count = 9;

    uint16_t previous_code = CLEAR_CODE;
    uint16_t code;
    while (reader_has_next(&r, bits_count))
    {
        code = reader_next(&r, bits_count);

        if (code == CLEAR_CODE)
        {
            decode_table_init(&table);
            bits_count = 9;
        }
        else if (code == END_OF_INFORMATION)
        {
            break;
        }
        else if (previous_code == CLEAR_CODE)
        {
            *w++ = code;
        }
        else
        {
            bool contains = decode_table_contains(&table, code);
            uint16_t handled_code = contains ? code : previous_code;

            decode_table_write_bytes(&w, handled_code, &table);

            uint8_t append_byte = decode_table_get_first_byte(&table, handled_code);

            if (!contains)
            {
                *w++ = append_byte;
            }

            int16_t result = decode_table_append(&table, previous_code, append_byte);
            if (error(result))
            {
                return result;
            }

            if (is_power_of_two(table.next_code + 1) && bits_count < MAX_BITS_COUNT)
            {
                bits_count++;
            }
        }

        previous_code = code;
    }

    return w - out_start;
}
