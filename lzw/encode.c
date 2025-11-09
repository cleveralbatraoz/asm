#include "encode.h"

#include "common.h"
#include "encode_table.h"
#include "writer.h"

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

size_t lzw_encode(uint8_t const *in, size_t in_size, uint8_t *restrict out, size_t out_size)
{
    if (in == NULL || in_size == 0 || out == NULL || out_size == 0)
    {
        return 0;
    }

    struct writer w;
    writer_init(&w, out, out_size);

    uint8_t bits_count = 9;

    int16_t result = writer_write(&w, CLEAR_CODE, bits_count);
    if (error(result))
    {
        return result;
    }

    struct encode_table table;
    encode_table_init(&table);

    int16_t current_sequence_code = *in;
    size_t read_index = 1;

    while (read_index < in_size)
    {
        uint8_t next_byte = in[read_index];

        if (encode_table_contains(&table, current_sequence_code, next_byte))
        {
            current_sequence_code = encode_table_get_next_sequence_code(&table, current_sequence_code, next_byte);
        }
        else
        {
            int16_t result = writer_write(&w, current_sequence_code, bits_count);
            if (error(result))
            {
                return result;
            }

            result = encode_table_append(&table, current_sequence_code, next_byte);
            if (error(result))
            {
                return result;
            }

            if (is_power_of_two(table.next_code))
            {
                if (bits_count < MAX_BITS_COUNT)
                {
                    ++bits_count;
                }
                else
                {
                    writer_write(&w, CLEAR_CODE, bits_count);

                    encode_table_init(&table);
                    bits_count = 9;
                }
            }

            current_sequence_code = next_byte;
        }

        ++read_index;
    }

    result = writer_write(&w, current_sequence_code, bits_count);
    if (error(result))
    {
        return result;
    }

    writer_write(&w, END_OF_INFORMATION, bits_count);
    if (error(result))
    {
        return result;
    }

    return writer_written_bytes(&w);
}
