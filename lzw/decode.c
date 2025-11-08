#include "decode.h"
#include "byte_writer.h"
#include "decode_table.h"
#include "reader.h"

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

    int16_t code = reader_read(&r);
    if (code < 256)
    {
        byte_writer_write(&w, code);
    }
    else
    {
        return -1;
    }

    int16_t previous_code = code;
    while (reader_has_next(&r))
    {
        code = reader_read(&r);

        uint8_t append_byte;

        if (decode_table_contains(&table, code))
        {
            if (decode_table_write_bytes(&w, code, &table) == -1)
            {
                return -1;
            }

            append_byte = decode_table_get_first_byte(&table, code);
        }
        else
        {
            if (decode_table_write_bytes(&w, previous_code, &table) == -1)
            {
                return -1;
            }

            append_byte = decode_table_get_first_byte(&table, previous_code);
            byte_writer_write(&w, append_byte);
        }

        decode_table_append(&table, previous_code, append_byte);

        previous_code = code;
    }

    return byte_writer_written_bytes(&w);
}
