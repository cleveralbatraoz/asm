#include "decode.h"

#include "byte_writer.h"
#include "common.h"
#include "decode_table.h"
#include "reader.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t lzw_decode(const uint8_t *in, size_t in_size, uint8_t *restrict out, size_t out_size)
{
    if (in == NULL || in_size == 0 || out == NULL || out_size == 0)
    {
        return 0;
    }

    struct decode_table table;
    decode_table_init(&table);

    struct reader r;
    reader_init(&r, in, in_size);

    struct byte_writer w;
    byte_writer_init(&w, out, out_size);

    uint8_t bits_count = 9;

    int16_t previous_code = CLEAR_CODE;
    int16_t code;
    while (reader_has_next(&r, bits_count))
    {
        code = reader_next(&r, bits_count);
        if (error(code))
        {
            return code;
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
            return INPUT_INVARIANT_VIOLATION;
        }
        else if (previous_code == CLEAR_CODE)
        {
            int16_t result = byte_writer_write(&w, code);
            if (error(result))
            {
                return result;
            }
        }
        else
        {
            uint8_t append_byte;

            if (decode_table_contains(&table, code))
            {
                int16_t result = decode_table_write_bytes(&w, code, &table);
                if (error(result))
                {
                    return result;
                }

                append_byte = decode_table_get_first_byte(&table, code);
                if (error(append_byte))
                {
                    return append_byte;
                }
            }
            else
            {
                int16_t result = decode_table_write_bytes(&w, previous_code, &table);
                if (error(result))
                {
                    return result;
                }

                append_byte = decode_table_get_first_byte(&table, previous_code);
                if (error(append_byte))
                {
                    return append_byte;
                }

                result = byte_writer_write(&w, append_byte);
                if (error(result))
                {
                    return result;
                }
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

    return byte_writer_written_bytes(&w);
}
