#include "encode.h"
#include "common.h"
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

    uint32_t table[MAX_CODE][256] = {0};
    uint32_t next_code = 256;

    int16_t current_sequence_code = *in;
    size_t read_index = 1;

    while (read_index < in_size)
    {
        uint8_t next_byte = in[read_index];

        uint32_t next_sequence_code = table[current_sequence_code][next_byte];
        if (next_sequence_code == 0) // TODO: properly handle zeroes
        {
            if (writer_write(&w, current_sequence_code, bits_count) == -1)
            {
                return -1;
            }

            table[current_sequence_code][next_byte] = next_code;
            ++next_code; // TODO: handle overflow

            if (is_power_of_two(next_code) && bits_count < MAX_BITS_COUNT)
            {
                ++bits_count;
            }

            current_sequence_code = next_byte;
        }
        else
        {
            current_sequence_code = next_sequence_code;
        }

        ++read_index;
    }

    if (writer_write(&w, current_sequence_code, bits_count) == -1)
    {
        return -1;
    }

    return writer_written_bytes(&w);
}
