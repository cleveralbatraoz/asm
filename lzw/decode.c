#include "decode.h"

#include "common.h"
#include "decode_table.h"
#include "reader.h"
#include "writer.h"

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

    struct writer w;
    writer_init(&w, out, out_size);

    uint8_t bits_count = 9;

    int16_t previous_code = CLEAR_CODE;
    int16_t code;
    while (reader_has_next(&r, bits_count))
    {
        code = reader_next(&r, bits_count);
        if (error(code))
        {
            return -1;
        }

        if (code == CLEAR_CODE)
        {
            decode_table_init(&table);
            bits_count = 9;
        }
        else if (code == END_OF_INFORMATION)
        {
            break;
        }
        else if (!is_valid_code(code))
        {
            return -1;
        }
        else if (previous_code == CLEAR_CODE)
        {
            if (error(writer_write(&w, code)))
            {
                return -1;
            }
        }
        else
        {
            bool contains = decode_table_contains(&table, code);
            int16_t handled_code = contains ? code : previous_code;

            if (error(decode_table_write_bytes(&w, handled_code, &table)))
            {
                return -1;
            }

            int16_t append_byte = decode_table_get_first_byte(&table, handled_code);
            if (error(append_byte))
            {
                return -1;
            }

            if (!contains)
            {
                if (error(writer_write(&w, (uint8_t)append_byte)))
                {
                    return -1;
                }
            }

            if (error(decode_table_append(&table, previous_code, (uint8_t)append_byte)))
            {
                return -1;
            }

            if (is_power_of_two(table.next_code + 1) && bits_count < MAX_BITS_COUNT)
            {
                bits_count++;
            }
        }

        previous_code = code;
    }

    return writer_written_bytes(&w);
}
