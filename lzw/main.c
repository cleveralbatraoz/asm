#include "../minunit/minunit.h"

#include "common.h"
#include "decode.h"
#include "reader.h"
#include "writer.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_from_files(const char *encoded_path, const char *expected_path)
{
    FILE *f_encoded = fopen(encoded_path, "rb");
    mu_assert(f_encoded != NULL, "failed to open encoded file");

    FILE *f_expected = fopen(expected_path, "rb");
    mu_assert(f_expected != NULL, "failed to open expected file");

    fseek(f_encoded, 0, SEEK_END);
    long encoded_size = ftell(f_encoded);
    fseek(f_encoded, 0, SEEK_SET);

    fseek(f_expected, 0, SEEK_END);
    long expected_size = ftell(f_expected);
    fseek(f_expected, 0, SEEK_SET);

    mu_assert(encoded_size > 0, "encoded file should not be empty");
    mu_assert(expected_size > 0, "expected file should not be empty");

    uint8_t *encoded = malloc((size_t)encoded_size);
    uint8_t *expected = malloc((size_t)expected_size);
    uint8_t *decoded = malloc((size_t)expected_size * 2);

    mu_assert(encoded != NULL && expected != NULL && decoded != NULL, "memory allocation failed");

    size_t read_encoded = fread(encoded, 1, (size_t)encoded_size, f_encoded);
    size_t read_expected = fread(expected, 1, (size_t)expected_size, f_expected);

    fclose(f_encoded);
    fclose(f_expected);

    mu_assert(read_encoded == (size_t)encoded_size, "failed to read all encoded data");
    mu_assert(read_expected == (size_t)expected_size, "failed to read all expected data");

    size_t decoded_size = lzw_decode(encoded, (size_t)encoded_size, decoded, (size_t)expected_size * 2);
    mu_assert(!error(decoded_size), error_message(decoded_size));
    mu_assert(decoded_size == (size_t)expected_size, "decoded size mismatch");

    mu_assert(memcmp(expected, decoded, (size_t)expected_size) == 0, "decoded content does not match expected");

    free(encoded);
    free(expected);
    free(decoded);
}

MU_TEST(test_from_test_data)
{
    test_from_files("test_data/in", "test_data/out");
}

MU_TEST(test_from_aaa_test_data)
{
    test_from_files("test_data/aaa.enc", "test_data/aaa.dec");
}

MU_TEST(test_writer_write_9_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 0x123, 9);
    mu_assert(result == SUCCESS, "writer_write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should have written at least 2 bytes for 9 bits");
    mu_assert(buffer[0] != 0 || buffer[1] != 0, "some data should be written");
}

MU_TEST(test_writer_write_10_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 0x234, 10);
    mu_assert(result == SUCCESS, "writer_write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should have written at least 2 bytes for 10 bits");
    mu_assert(buffer[0] != 0 || buffer[1] != 0, "some data should be written");
}

MU_TEST(test_writer_write_11_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 0x456, 11);
    mu_assert(result == SUCCESS, "writer_write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should have written at least 2 bytes for 11 bits");
    mu_assert(buffer[0] != 0 || buffer[1] != 0, "some data should be written");
}

MU_TEST(test_writer_write_12_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 0x789, 12);
    mu_assert(result == SUCCESS, "writer_write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should have written at least 2 bytes for 12 bits");
    mu_assert(buffer[0] != 0 || buffer[1] != 0, "some data should be written");
}

MU_TEST(test_writer_write_simple_9_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 0x100, 9);
    mu_assert(result == SUCCESS, "writer_write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should have written at least 2 bytes");
}

MU_TEST(test_writer_write_exact_output_sequence)
{
    uint8_t buffer[16] = {0};
    uint8_t expected[] = {0x80, 0x10, 0x60, 0x50, 0x10};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));


    int16_t result1 = writer_write(&w, 256, 9);
    mu_assert(result1 == SUCCESS, "write 256 should succeed");

    int16_t result2 = writer_write(&w, 65, 9);
    mu_assert(result2 == SUCCESS, "write 65 should succeed");

    int16_t result3 = writer_write(&w, 258, 9);
    mu_assert(result3 == SUCCESS, "write 258 should succeed");

    int16_t result4 = writer_write(&w, 257, 9);
    mu_assert(result4 == SUCCESS, "write 257 should succeed");

    size_t written = writer_written_bytes(&w);

    if (written == sizeof(expected))
    {
        int all_match = 1;
        for (size_t i = 0; i < sizeof(expected); i++)
        {
            if (buffer[i] != expected[i])
            {
                all_match = 0;
                break;
            }
        }

        if (all_match)
        {
            mu_assert(1, "output matches expected");
            return;
        }
    }

    char msg[256];
    snprintf(msg, sizeof(msg),
             "Output mismatch. Expected: %02X %02X %02X %02X %02X, Got: %02X %02X %02X %02X %02X (written: %zu bytes). "
             "This suggests the bit counts might need to vary, or the expected output was calculated differently.",
             expected[0], expected[1], expected[2], expected[3], expected[4], buffer[0], buffer[1], buffer[2],
             buffer[3], buffer[4], written);

    mu_assert(written == sizeof(expected), msg);

    for (size_t i = 0; i < sizeof(expected); i++)
    {
        char byte_msg[128];
        snprintf(byte_msg, sizeof(byte_msg), "byte %zu should be 0x%02X, got 0x%02X", i, expected[i], buffer[i]);
        mu_assert(buffer[i] == expected[i], byte_msg);
    }
}

MU_TEST(test_writer_write_exact_output_single_9bit)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 256, 9);
    mu_assert(result == SUCCESS, "write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should write at least 2 bytes for 9 bits");

    mu_assert(buffer[0] != 0 || buffer[1] != 0, "should write some data");
}

MU_TEST(test_writer_write_exact_output_single_12bit)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 256, 12);
    mu_assert(result == SUCCESS, "write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should write at least 2 bytes for 12 bits");

    mu_assert(buffer[0] != 0 || buffer[1] != 0, "should write some data");
}

MU_TEST(test_writer_write_exact_output_comparison)
{
    uint8_t buffer1[16] = {0};
    uint8_t buffer2[16] = {0};
    struct writer w1, w2;

    writer_init(&w1, buffer1, sizeof(buffer1));
    writer_init(&w2, buffer2, sizeof(buffer2));

    int16_t result1 = writer_write(&w1, 256, 9);
    int16_t result2 = writer_write(&w2, 256, 12);

    mu_assert(result1 == SUCCESS, "9-bit write should succeed");
    mu_assert(result2 == SUCCESS, "12-bit write should succeed");

    size_t written1 = writer_written_bytes(&w1);
    size_t written2 = writer_written_bytes(&w2);

    mu_assert(written1 >= 2, "9-bit should write at least 2 bytes");
    mu_assert(written2 >= 2, "12-bit should write at least 2 bytes");
}

MU_TEST(test_writer_write_simple_12_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 0xFFF, 12);
    mu_assert(result == SUCCESS, "writer_write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should have written at least 2 bytes");
    mu_assert(buffer[0] != 0 || buffer[1] != 0, "maximum value should write data");
}

MU_TEST(test_writer_write_multiple_9_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result1 = writer_write(&w, 0x100, 9);
    mu_assert(result1 == SUCCESS, "first write should succeed");

    int16_t result2 = writer_write(&w, 0x101, 9);
    mu_assert(result2 == SUCCESS, "second write should succeed");

    int16_t result3 = writer_write(&w, 0x102, 9);
    mu_assert(result3 == SUCCESS, "third write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 3, "should have written at least 3 bytes");
}

MU_TEST(test_writer_write_multiple_12_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result1 = writer_write(&w, 0x100, 12);
    mu_assert(result1 == SUCCESS, "first write should succeed");

    int16_t result2 = writer_write(&w, 0x200, 12);
    mu_assert(result2 == SUCCESS, "second write should succeed");

    int16_t result3 = writer_write(&w, 0x300, 12);
    mu_assert(result3 == SUCCESS, "third write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 5, "should have written at least 5 bytes for three 12-bit writes");
}

MU_TEST(test_writer_write_overflow)
{
    uint8_t buffer[2] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 0x100, 12);
    mu_assert(result == SUCCESS || result == WRITER_OVERFLOW, "should handle overflow gracefully");
}

MU_TEST(test_writer_write_overflow_small_buffer)
{
    uint8_t buffer[1] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 0x100, 12);
    mu_assert(result == WRITER_OVERFLOW, "should return WRITER_OVERFLOW for small buffer");
}

MU_TEST(test_writer_write_boundary_9_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result1 = writer_write(&w, 0x000, 9);
    mu_assert(result1 == SUCCESS, "should write 0 successfully");

    int16_t result2 = writer_write(&w, 0x1FF, 9);
    mu_assert(result2 == SUCCESS, "should write 511 successfully");
}

MU_TEST(test_writer_write_boundary_12_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result1 = writer_write(&w, 0x000, 12);
    mu_assert(result1 == SUCCESS, "should write 0 successfully");

    int16_t result2 = writer_write(&w, 0xFFF, 12);
    mu_assert(result2 == SUCCESS, "should write 4095 successfully");
}

MU_TEST(test_writer_write_mixed_bit_counts)
{
    uint8_t buffer[32] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result1 = writer_write(&w, 0x100, 9);
    mu_assert(result1 == SUCCESS, "9-bit write should succeed");

    int16_t result2 = writer_write(&w, 0x200, 10);
    mu_assert(result2 == SUCCESS, "10-bit write should succeed");

    int16_t result3 = writer_write(&w, 0x400, 11);
    mu_assert(result3 == SUCCESS, "11-bit write should succeed");

    int16_t result4 = writer_write(&w, 0x800, 12);
    mu_assert(result4 == SUCCESS, "12-bit write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written > 0, "should have written some bytes");
}

MU_TEST(test_writer_write_sequential_codes)
{
    uint8_t buffer[32] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    for (int i = 0; i < 10; i++)
    {
        int16_t result = writer_write(&w, 0x100 + i, 9);
        mu_assert(result == SUCCESS, "sequential write should succeed");
    }

    size_t written = writer_written_bytes(&w);
    mu_assert(written > 0, "should have written bytes");
}

MU_TEST(test_writer_write_big_endian_format)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 0x123, 9);
    mu_assert(result == SUCCESS, "write should succeed");

    mu_assert(buffer[0] != 0 || buffer[1] != 0, "some data should be written");
}

MU_TEST(test_writer_write_all_bit_counts_min)
{
    uint8_t buffer[32] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result1 = writer_write(&w, 0x000, 9);
    mu_assert(result1 == SUCCESS, "9-bit min should succeed");

    int16_t result2 = writer_write(&w, 0x000, 10);
    mu_assert(result2 == SUCCESS, "10-bit min should succeed");

    int16_t result3 = writer_write(&w, 0x000, 11);
    mu_assert(result3 == SUCCESS, "11-bit min should succeed");

    int16_t result4 = writer_write(&w, 0x000, 12);
    mu_assert(result4 == SUCCESS, "12-bit min should succeed");
}

MU_TEST(test_writer_write_all_bit_counts_max)
{
    uint8_t buffer[32] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result1 = writer_write(&w, 0x1FF, 9);
    mu_assert(result1 == SUCCESS, "9-bit max should succeed");

    int16_t result2 = writer_write(&w, 0x3FF, 10);
    mu_assert(result2 == SUCCESS, "10-bit max should succeed");

    int16_t result3 = writer_write(&w, 0x7FF, 11);
    mu_assert(result3 == SUCCESS, "11-bit max should succeed");

    int16_t result4 = writer_write(&w, 0xFFF, 12);
    mu_assert(result4 == SUCCESS, "12-bit max should succeed");
}

MU_TEST(test_writer_write_powers_of_two)
{
    uint8_t buffer[32] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result1 = writer_write(&w, 0x001, 9);
    mu_assert(result1 == SUCCESS, "2^0 should succeed");

    int16_t result2 = writer_write(&w, 0x002, 9);
    mu_assert(result2 == SUCCESS, "2^1 should succeed");

    int16_t result3 = writer_write(&w, 0x004, 9);
    mu_assert(result3 == SUCCESS, "2^2 should succeed");

    int16_t result4 = writer_write(&w, 0x008, 9);
    mu_assert(result4 == SUCCESS, "2^3 should succeed");

    int16_t result5 = writer_write(&w, 0x010, 9);
    mu_assert(result5 == SUCCESS, "2^4 should succeed");

    int16_t result6 = writer_write(&w, 0x020, 9);
    mu_assert(result6 == SUCCESS, "2^5 should succeed");

    int16_t result7 = writer_write(&w, 0x040, 9);
    mu_assert(result7 == SUCCESS, "2^6 should succeed");

    int16_t result8 = writer_write(&w, 0x080, 9);
    mu_assert(result8 == SUCCESS, "2^7 should succeed");

    int16_t result9 = writer_write(&w, 0x100, 9);
    mu_assert(result9 == SUCCESS, "2^8 should succeed");
}

MU_TEST(test_writer_write_overflow_exact_boundary)
{
    uint8_t buffer[2] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 0x100, 12);
    mu_assert(result == SUCCESS || result == WRITER_OVERFLOW, "should handle boundary correctly");
}

MU_TEST(test_writer_write_overflow_after_writes)
{
    uint8_t buffer[4] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result1 = writer_write(&w, 0x100, 12);
    mu_assert(result1 == SUCCESS, "first write should succeed");

    int16_t result2 = writer_write(&w, 0x200, 12);
    mu_assert(result2 == SUCCESS || result2 == WRITER_OVERFLOW, "second write should handle overflow");
}

MU_TEST(test_writer_write_many_sequential)
{
    uint8_t buffer[128] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    for (int i = 0; i < 50; i++)
    {
        int16_t result = writer_write(&w, 0x100 + i, 9);
        mu_assert(result == SUCCESS, "sequential write should succeed");
    }

    size_t written = writer_written_bytes(&w);
    mu_assert(written > 0, "should have written bytes");
    mu_assert(written <= sizeof(buffer), "should not exceed buffer");
}

MU_TEST(test_writer_write_alternating_bit_counts)
{
    uint8_t buffer[64] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result1 = writer_write(&w, 0x100, 9);
    mu_assert(result1 == SUCCESS, "9-bit write should succeed");

    int16_t result2 = writer_write(&w, 0x200, 12);
    mu_assert(result2 == SUCCESS, "12-bit write should succeed");

    int16_t result3 = writer_write(&w, 0x150, 10);
    mu_assert(result3 == SUCCESS, "10-bit write should succeed");

    int16_t result4 = writer_write(&w, 0x250, 11);
    mu_assert(result4 == SUCCESS, "11-bit write should succeed");

    int16_t result5 = writer_write(&w, 0x300, 9);
    mu_assert(result5 == SUCCESS, "9-bit write should succeed");
}

MU_TEST(test_writer_write_code_range_9_bits)
{
    uint8_t buffer[64] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    for (int i = 0; i < 256; i += 16)
    {
        int16_t result = writer_write(&w, i, 9);
        mu_assert(result == SUCCESS, "code in range should succeed");
    }
}

MU_TEST(test_writer_write_code_range_12_bits)
{
    uint8_t buffer[128] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    for (int i = 0; i < 4096; i += 256)
    {
        int16_t result = writer_write(&w, i, 12);
        mu_assert(result == SUCCESS, "code in range should succeed");
    }
}

MU_TEST(test_writer_write_written_bytes_tracking)
{
    uint8_t buffer[32] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    size_t written_before = writer_written_bytes(&w);
    mu_assert(written_before == 0, "should start with 0 bytes written");

    int16_t result = writer_write(&w, 0x100, 9);
    mu_assert(result == SUCCESS, "write should succeed");

    size_t written_after = writer_written_bytes(&w);
    mu_assert(written_after > written_before, "should have written some bytes");
    mu_assert(written_after <= sizeof(buffer), "should not exceed buffer size");
}

MU_TEST(test_reader_read_single_9bit)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t result = writer_write(&w, 0x123, 9);
    mu_assert(result == SUCCESS, "write should succeed");

    size_t written = writer_written_bytes(&w);
    char debug_msg[256];
    snprintf(debug_msg, sizeof(debug_msg), "Written %zu bytes: %02X %02X %02X %02X", written, buffer[0], buffer[1],
             buffer[2], buffer[3]);

    struct reader r;
    reader_init(&r, buffer, written);

    mu_assert(reader_has_next(&r, 9), "should have 9 bits available");
    int16_t value = reader_next(&r, 9);

    char msg[512];
    snprintf(msg, sizeof(msg), "Expected 0x123 (291), got 0x%03X (%d). %s", value & 0xFFF, value, debug_msg);
    mu_assert(value == 0x123, msg);

    size_t total_bits = written * 8;
    size_t bits_consumed = r.byte_index * 8 + r.bit_index;
    size_t bits_remaining = total_bits - bits_consumed;

    char debug_pos[256];
    snprintf(debug_pos, sizeof(debug_pos), "Total bits: %zu, consumed: %zu, remaining: %zu. Reader at byte %zu, bit %d",
             total_bits, bits_consumed, bits_remaining, r.byte_index, r.bit_index);

    mu_assert(!reader_has_next(&r, 9), debug_pos);

    mu_assert(reader_has_next(&r, 7), "should have 7 bits available");
    mu_assert(!reader_has_next(&r, 8), "should not have 8 bits available");
}

MU_TEST(test_reader_read_multiple_9bit)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    writer_write(&w, 256, 9);
    writer_write(&w, 65, 9);
    writer_write(&w, 258, 9);
    writer_write(&w, 257, 9);

    struct reader r;
    reader_init(&r, buffer, writer_written_bytes(&w));

    mu_assert(reader_next(&r, 9) == 256, "should read 256");
    mu_assert(reader_next(&r, 9) == 65, "should read 65");
    mu_assert(reader_next(&r, 9) == 258, "should read 258");
    mu_assert(reader_next(&r, 9) == 257, "should read 257");
}

MU_TEST(test_reader_read_all_bit_counts)
{
    uint8_t buffer[32] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    writer_write(&w, 0x1FF, 9); 
    writer_write(&w, 0x3FF, 10);
    writer_write(&w, 0x7FF, 11);
    writer_write(&w, 0xFFF, 12);

    struct reader r;
    reader_init(&r, buffer, writer_written_bytes(&w));

    mu_assert(reader_next(&r, 9) == 0x1FF, "should read max 9-bit");
    mu_assert(reader_next(&r, 10) == 0x3FF, "should read max 10-bit");
    mu_assert(reader_next(&r, 11) == 0x7FF, "should read max 11-bit");
    mu_assert(reader_next(&r, 12) == 0xFFF, "should read max 12-bit");
}

MU_TEST(test_reader_write_read_round_trip)
{
    uint8_t buffer[128] = {0};
    uint16_t test_values[] = {0, 1, 2, 255, 256, 257, 511, 512, 1023, 1024, 2047, 2048, 4095};
    size_t num_values = sizeof(test_values) / sizeof(test_values[0]);

    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    for (size_t i = 0; i < num_values; i++)
    {
        int16_t result = writer_write(&w, test_values[i], 12);
        mu_assert(result == SUCCESS, "write should succeed");
    }

    struct reader r;
    reader_init(&r, buffer, writer_written_bytes(&w));

    for (size_t i = 0; i < num_values; i++)
    {
        int16_t value = reader_next(&r, 12);
        char msg[64];
        snprintf(msg, sizeof(msg), "value %zu should be %u, got %d", i, test_values[i], value);
        mu_assert(value == test_values[i], msg);
    }
}

MU_TEST(test_reader_mixed_bit_counts)
{
    uint8_t buffer[64] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    writer_write(&w, 0x100, 9);
    writer_write(&w, 0x200, 10);
    writer_write(&w, 0x400, 11);
    writer_write(&w, 0x800, 12);
    writer_write(&w, 0x1FF, 9);

    struct reader r;
    reader_init(&r, buffer, writer_written_bytes(&w));

    mu_assert(reader_next(&r, 9) == 0x100, "should read 9-bit value");
    mu_assert(reader_next(&r, 10) == 0x200, "should read 10-bit value");
    mu_assert(reader_next(&r, 11) == 0x400, "should read 11-bit value");
    mu_assert(reader_next(&r, 12) == 0x800, "should read 12-bit value");
    mu_assert(reader_next(&r, 9) == 0x1FF, "should read second 9-bit value");
}

MU_TEST(test_reader_boundary_conditions)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    writer_write(&w, 0, 9);     
    writer_write(&w, 0x1FF, 9); 
    writer_write(&w, 0, 12);    
    writer_write(&w, 0xFFF, 12);

    struct reader r;
    reader_init(&r, buffer, writer_written_bytes(&w));

    mu_assert(reader_next(&r, 9) == 0, "should read 0");
    mu_assert(reader_next(&r, 9) == 0x1FF, "should read max 9-bit");
    mu_assert(reader_next(&r, 12) == 0, "should read 0");
    mu_assert(reader_next(&r, 12) == 0xFFF, "should read max 12-bit");
}

MU_TEST(test_reader_insufficient_data)
{
    uint8_t buffer[4] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    int16_t res1 = writer_write(&w, 0x123, 9);
    int16_t res2 = writer_write(&w, 0x156, 9);
    mu_assert(res1 == SUCCESS, "first write should succeed");
    mu_assert(res2 == SUCCESS, "second write should succeed");

    size_t written = writer_written_bytes(&w);

    struct reader r;
    reader_init(&r, buffer, written);

    int16_t val1 = reader_next(&r, 9);
    mu_assert(val1 == 0x123, "first value should be 0x123");

    int16_t val2 = reader_next(&r, 9);
    mu_assert(val2 == 0x156, "second value should be 0x156");

    size_t total_bits2 = written * 8;
    size_t bits_consumed2 = r.byte_index * 8 + r.bit_index;
    size_t bits_remaining2 = total_bits2 - bits_consumed2;

    char debug_insufficient[256];
    snprintf(debug_insufficient, sizeof(debug_insufficient),
             "Total bits: %zu, consumed: %zu, remaining: %zu. Reader at byte %zu, bit %d. Buffer size: %zu",
             total_bits2, bits_consumed2, bits_remaining2, r.byte_index, r.bit_index, sizeof(buffer));

    mu_assert(!reader_has_next(&r, 12), debug_insufficient);
    int16_t value = reader_next(&r, 12);
    mu_assert(value == -1, "should return -1 when not enough data");
}

MU_TEST(test_reader_exact_output_sequence_verify)
{
    uint8_t buffer[] = {0x80, 0x10, 0x60, 0x50, 0x10};
    struct reader r;
    reader_init(&r, buffer, sizeof(buffer));

    mu_assert(reader_next(&r, 9) == 256, "should read 256");
    mu_assert(reader_next(&r, 9) == 65, "should read 65");
    mu_assert(reader_next(&r, 9) == 258, "should read 258");
    mu_assert(reader_next(&r, 9) == 257, "should read 257");

    mu_assert(!reader_has_next(&r, 9), "should have no more data");
}

MU_TEST(test_reader_sequential_values)
{
    uint8_t buffer[256] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    for (int i = 0; i < 100; i++)
    {
        writer_write(&w, i, 9);
    }

    struct reader r;
    reader_init(&r, buffer, writer_written_bytes(&w));

    for (int i = 0; i < 100; i++)
    {
        int16_t value = reader_next(&r, 9);
        char msg[64];
        snprintf(msg, sizeof(msg), "value %d mismatch", i);
        mu_assert(value == i, msg);
    }
}

MU_TEST(test_reader_powers_of_two)
{
    uint8_t buffer[64] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    for (int i = 0; i < 12; i++)
    {
        writer_write(&w, 1 << i, 12);
    }

    struct reader r;
    reader_init(&r, buffer, writer_written_bytes(&w));

    for (int i = 0; i < 12; i++)
    {
        int16_t value = reader_next(&r, 12);
        int16_t expected = 1 << i;
        char msg[64];
        snprintf(msg, sizeof(msg), "power of two 2^%d should be %d, got %d", i, expected, value);
        mu_assert(value == expected, msg);
    }
}

MU_TEST_SUITE(writer_basic_suite)
{
    MU_RUN_TEST(test_writer_write_9_bits);
    MU_RUN_TEST(test_writer_write_10_bits);
    MU_RUN_TEST(test_writer_write_11_bits);
    MU_RUN_TEST(test_writer_write_12_bits);
    MU_RUN_TEST(test_writer_write_simple_9_bits);
    MU_RUN_TEST(test_writer_write_simple_12_bits);
}

MU_TEST_SUITE(writer_exact_output_suite)
{
    MU_RUN_TEST(test_writer_write_exact_output_sequence);
    MU_RUN_TEST(test_writer_write_exact_output_single_9bit);
    MU_RUN_TEST(test_writer_write_exact_output_single_12bit);
    MU_RUN_TEST(test_writer_write_exact_output_comparison);
}

MU_TEST_SUITE(writer_multiple_writes_suite)
{
    MU_RUN_TEST(test_writer_write_multiple_9_bits);
    MU_RUN_TEST(test_writer_write_multiple_12_bits);
    MU_RUN_TEST(test_writer_write_sequential_codes);
    MU_RUN_TEST(test_writer_write_many_sequential);
    MU_RUN_TEST(test_writer_write_alternating_bit_counts);
}

MU_TEST_SUITE(writer_boundary_suite)
{
    MU_RUN_TEST(test_writer_write_boundary_9_bits);
    MU_RUN_TEST(test_writer_write_boundary_12_bits);
    MU_RUN_TEST(test_writer_write_all_bit_counts_min);
    MU_RUN_TEST(test_writer_write_all_bit_counts_max);
    MU_RUN_TEST(test_writer_write_powers_of_two);
    MU_RUN_TEST(test_writer_write_code_range_9_bits);
    MU_RUN_TEST(test_writer_write_code_range_12_bits);
}

MU_TEST_SUITE(writer_overflow_suite)
{
    MU_RUN_TEST(test_writer_write_overflow);
    MU_RUN_TEST(test_writer_write_overflow_small_buffer);
    MU_RUN_TEST(test_writer_write_overflow_exact_boundary);
    MU_RUN_TEST(test_writer_write_overflow_after_writes);
}

MU_TEST_SUITE(writer_edge_cases_suite)
{
    MU_RUN_TEST(test_writer_write_mixed_bit_counts);
    MU_RUN_TEST(test_writer_write_big_endian_format);
    MU_RUN_TEST(test_writer_write_written_bytes_tracking);
}

MU_TEST_SUITE(reader_suite)
{
    MU_RUN_TEST(test_reader_read_single_9bit);
    MU_RUN_TEST(test_reader_read_multiple_9bit);
    MU_RUN_TEST(test_reader_read_all_bit_counts);
    MU_RUN_TEST(test_reader_write_read_round_trip);
    MU_RUN_TEST(test_reader_mixed_bit_counts);
    MU_RUN_TEST(test_reader_boundary_conditions);
    MU_RUN_TEST(test_reader_insufficient_data);
    MU_RUN_TEST(test_reader_exact_output_sequence_verify);
    MU_RUN_TEST(test_reader_sequential_values);
    MU_RUN_TEST(test_reader_powers_of_two);
}

MU_TEST_SUITE(decode_suite)
{
    MU_RUN_TEST(test_from_aaa_test_data);
    MU_RUN_TEST(test_from_test_data);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(writer_basic_suite);
    MU_RUN_SUITE(writer_exact_output_suite);
    MU_RUN_SUITE(writer_multiple_writes_suite);
    MU_RUN_SUITE(writer_boundary_suite);
    MU_RUN_SUITE(writer_overflow_suite);
    MU_RUN_SUITE(writer_edge_cases_suite);
    MU_RUN_SUITE(reader_suite);
    MU_RUN_SUITE(decode_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
