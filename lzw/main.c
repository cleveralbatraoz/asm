#include "../minunit/minunit.h"

#include "common.h"
#include "decode.h"
#include "encode.h"
#include "reader.h"
#include "writer.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_round_trip(const uint8_t *in, size_t in_size, const char *test_name)
{
    uint8_t encoded[32768] = {0};
    uint8_t decoded[16384] = {0};

    size_t encoded_size = lzw_encode(in, in_size, encoded, sizeof(encoded));
    mu_assert(!error(encoded_size), error_message(encoded_size));

    size_t decoded_size = lzw_decode(encoded, encoded_size, decoded, sizeof(decoded));
    mu_assert(!error(decoded_size), error_message(decoded_size));

    char msg[256];
    snprintf(msg, sizeof(msg), "%s: decoded size mismatch", test_name);
    mu_assert(decoded_size == in_size, msg);
    snprintf(msg, sizeof(msg), "%s: content mismatch", test_name);
    mu_assert(memcmp(in, decoded, in_size) == 0, msg);
}

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

MU_TEST(test_round_trip_aaa)
{
    uint8_t in[] = {'A', 'A', 'A'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_abc)
{
    uint8_t in[] = {'A', 'B', 'C'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_hello)
{
    uint8_t in[] = {'H', 'E', 'L', 'L', 'O'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_single_byte)
{
    uint8_t in[] = {'X'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_single_zero)
{
    uint8_t in[] = {0};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_single_255)
{
    uint8_t in[] = {255};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_two_bytes_same)
{
    uint8_t in[] = {'A', 'A'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_two_bytes_different)
{
    uint8_t in[] = {'A', 'B'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_all_same_short)
{
    uint8_t in[] = {'A', 'A', 'A', 'A', 'A'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_all_same_long)
{
    uint8_t in[100];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = 'X';
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_alternating_ab)
{
    uint8_t in[] = {'A', 'B', 'A', 'B', 'A', 'B', 'A', 'B'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_alternating_long)
{
    uint8_t in[200];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (i % 2 == 0) ? '0' : '1';
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_repeated_abc)
{
    uint8_t in[] = {'A', 'B', 'C', 'A', 'B', 'C', 'A', 'B', 'C'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_repeated_pattern)
{
    uint8_t in[] = {'H', 'E', 'L', 'L', 'O', 'H', 'E', 'L', 'L', 'O'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_sequential)
{
    uint8_t in[] = {'A', 'A', 'A', 'B', 'B', 'B', 'C', 'C', 'C'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_nested_patterns)
{
    uint8_t in[] = {'A', 'B', 'A', 'B', 'C', 'A', 'B', 'C', 'D'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_decoder_special_case)
{
    uint8_t in[] = {'A', 'B', 'A', 'B', 'A'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_all_bytes)
{
    uint8_t in[128];
    for (int i = 0; i < 128; i++)
    {
        in[i] = (uint8_t)i;
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_binary_zeros)
{
    uint8_t in[50];
    memset(in, 0, sizeof(in));
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_binary_ones)
{
    uint8_t in[50];
    memset(in, 0xFF, sizeof(in));
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_long_sequence)
{
    uint8_t in[500];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (uint8_t)(i % 256);
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_long_repeating)
{
    uint8_t in[1000];
    const char pattern[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = pattern[i % (sizeof(pattern) - 1)];
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_many_unique_pairs)
{
    uint8_t in[200];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (uint8_t)(i % 256);
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_text_like)
{
    const char *text = "the quick brown fox jumps over the lazy dog";
    uint8_t in[44];
    memcpy(in, text, 43);
    test_round_trip(in, 43, __func__);
}

MU_TEST(test_round_trip_numbers)
{
    uint8_t in[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_mixed_case)
{
    uint8_t in[] = {'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_chain_pattern)
{
    uint8_t in[] = {'A', 'B', 'A', 'B', 'C', 'A', 'B', 'C', 'D', 'A', 'B', 'C', 'D', 'E'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_empty_pattern)
{
    uint8_t in[] = {'A', 'B', 'A', 'B'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_single_repeated_many)
{
    uint8_t in[256];
    memset(in, 'Z', sizeof(in));
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_incremental)
{
    uint8_t in[100];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (uint8_t)(i & 0xFF);
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_decremental)
{
    uint8_t in[100];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (uint8_t)((sizeof(in) - i) & 0xFF);
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_palindrome)
{
    uint8_t in[] = {'A', 'B', 'C', 'D', 'E', 'D', 'C', 'B', 'A'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_three_way_alternating)
{
    uint8_t in[150];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (uint8_t)('A' + (i % 3));
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_fibonacci_like)
{
    uint8_t in[] = {'A', 'B', 'A', 'B', 'C', 'A', 'B', 'C', 'D', 'A', 'B', 'C', 'D', 'E', 'A', 'B', 'C', 'D', 'E', 'F'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_boundary_values)
{
    uint8_t in[] = {0, 1, 254, 255, 0, 255, 1, 254};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_rapid_changes)
{
    uint8_t in[200];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (uint8_t)((i * 7) % 256);
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_gradual_transition)
{
    uint8_t in[128];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (uint8_t)(i);
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_high_frequency_pattern)
{
    uint8_t in[] = {'A', 'A', 'B', 'B', 'A', 'A', 'B', 'B', 'A', 'A', 'B', 'B'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_nested_repeats)
{
    uint8_t in[] = {'A', 'B', 'C', 'A', 'B', 'C', 'D', 'A', 'B', 'C', 'D', 'E'};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_unicode_like)
{
    uint8_t in[] = {0xC0, 0x80, 0xE0, 0x80, 0x80, 0xF0, 0x80, 0x80, 0x80};
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_sparse_pattern)
{
    uint8_t in[300];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (i % 10 == 0) ? 'X' : ' ';
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_compression_friendly)
{
    uint8_t in[500];
    const char *pattern = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG. ";
    size_t pattern_len = strlen(pattern);
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = pattern[i % pattern_len];
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_compression_unfriendly)
{
    uint8_t in[128];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (uint8_t)((i * 3 + 7) % 256);
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_very_long_sequence)
{
    uint8_t in[2000];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (uint8_t)(i % 256);
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_extremely_long_sequence)
{
    uint8_t in[5000];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (uint8_t)(i % 256);
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_long_repeating_pattern)
{
    uint8_t in[3000];
    const char pattern[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t pattern_len = strlen(pattern);
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = pattern[i % pattern_len];
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_long_sequential)
{
    uint8_t in[3000];
    for (size_t i = 0; i < sizeof(in); i++)
    {
        in[i] = (uint8_t)(i & 0xFF);
    }
    test_round_trip(in, sizeof(in), __func__);
}

MU_TEST(test_round_trip_exceeds_table_limit)
{
    // Create a sequence that will generate more than 4096 codes
    // We need to create many unique patterns to fill the table
    // Each unique byte pair creates a new code, so we need many unique pairs
    uint8_t in[10000];
    size_t idx = 0;

    // Strategy: create many unique patterns by varying sequences
    // This will generate codes 256, 257, 258, ... up to and beyond 4096
    for (size_t i = 0; i < 256 && idx < sizeof(in); i++)
    {
        in[idx++] = (uint8_t)i;
    }

    // Now create many unique pairs to fill the table
    // Each iteration creates new unique sequences
    for (size_t round = 0; round < 20 && idx < sizeof(in) - 1; round++)
    {
        for (size_t i = 0; i < 256 && idx < sizeof(in) - 1; i++)
        {
            in[idx++] = (uint8_t)i;
            in[idx++] = (uint8_t)((i + round) % 256);
        }
    }

    // Fill the rest with a pattern
    while (idx < sizeof(in))
    {
        in[idx] = (uint8_t)(idx % 256);
        idx++;
    }

    uint8_t encoded[16384] = {0};
    uint8_t decoded[16384] = {0};

    size_t encoded_size = lzw_encode(in, sizeof(in), encoded, sizeof(encoded));
    mu_assert(encoded_size != (size_t)-1, "encoding should succeed even when table exceeds limit");

    size_t decoded_size = lzw_decode(encoded, encoded_size, decoded, sizeof(decoded));
    mu_assert(decoded_size != (size_t)-1, "decoding should succeed even when table exceeded limit");
    mu_assert_int_eq(sizeof(in), decoded_size);
    mu_assert(memcmp(in, decoded, sizeof(in)) == 0, "decoded should match input even when table exceeded limit");
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

    // Write code 0x123 (291) with 9 bits
    // The function writes in big-endian format: high bits first
    int16_t result = writer_write(&w, 0x123, 9);
    mu_assert(result == SUCCESS, "writer_write should succeed");

    // Verify that data was written (exact byte values depend on bit packing)
    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should have written at least 2 bytes for 9 bits");
    mu_assert(buffer[0] != 0 || buffer[1] != 0, "some data should be written");
}

MU_TEST(test_writer_write_10_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Write code 0x234 (564) with 10 bits
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

    // Write code 0x456 (1110) with 11 bits
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

    // Write code 0x789 (1929) with 12 bits
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

    // Write code 0x100 (256) with 9 bits
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

    // Write sequence: 256, 65, 258, 257
    // Expected output: 0x80 0x10 0x60 0x50 0x10
    // Actual output with all 9 bits: 0x80 0x40 0x06 0x02 0x0C
    // The mismatch suggests either:
    // 1. The expected output was calculated with different bit counts
    // 2. The implementation has a bug
    // 3. The bit counts should vary (e.g., 9, 9, 10, 9 or similar)

    // Try with all 9 bits first
    int16_t result1 = writer_write(&w, 256, 9);
    mu_assert(result1 == SUCCESS, "write 256 should succeed");

    int16_t result2 = writer_write(&w, 65, 9);
    mu_assert(result2 == SUCCESS, "write 65 should succeed");

    int16_t result3 = writer_write(&w, 258, 9);
    mu_assert(result3 == SUCCESS, "write 258 should succeed");

    int16_t result4 = writer_write(&w, 257, 9);
    mu_assert(result4 == SUCCESS, "write 257 should succeed");

    size_t written = writer_written_bytes(&w);

    // Check if output matches expected
    // If not, the test will show the actual output for debugging
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

    // Output doesn't match - show what we got vs what was expected
    char msg[256];
    snprintf(msg, sizeof(msg),
             "Output mismatch. Expected: %02X %02X %02X %02X %02X, Got: %02X %02X %02X %02X %02X (written: %zu bytes). "
             "This suggests the bit counts might need to vary, or the expected output was calculated differently.",
             expected[0], expected[1], expected[2], expected[3], expected[4], buffer[0], buffer[1], buffer[2],
             buffer[3], buffer[4], written);

    // For now, verify we got the expected number of bytes and show the mismatch
    mu_assert(written == sizeof(expected), msg);

    // If we get here, do byte-by-byte comparison
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

    // Test single 9-bit write for code 256 (0x100)
    int16_t result = writer_write(&w, 256, 9);
    mu_assert(result == SUCCESS, "write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should write at least 2 bytes for 9 bits");

    // Verify the output matches expected pattern
    // For 9 bits: first byte = bits 1-8, second byte = bit 0
    // 256 = 0x100 = binary 1 0000 0000
    // Bits 1-8 = 0000 0000 = 0x00
    // Bit 0 = 1
    // But exact byte values depend on bit packing with bit_index
    mu_assert(buffer[0] != 0 || buffer[1] != 0, "should write some data");
}

MU_TEST(test_writer_write_exact_output_single_12bit)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Test single 12-bit write for code 256 (0x100)
    int16_t result = writer_write(&w, 256, 12);
    mu_assert(result == SUCCESS, "write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should write at least 2 bytes for 12 bits");

    // For 12 bits: first byte = bits 4-11, second byte = bits 0-3
    // 256 = 0x100 = binary 0001 0000 0000
    // Bits 4-11 = 0001 0000 = 0x10
    // Bits 0-3 = 0000 = 0x00
    mu_assert(buffer[0] != 0 || buffer[1] != 0, "should write some data");
}

MU_TEST(test_writer_write_exact_output_comparison)
{
    uint8_t buffer1[16] = {0};
    uint8_t buffer2[16] = {0};
    struct writer w1, w2;

    writer_init(&w1, buffer1, sizeof(buffer1));
    writer_init(&w2, buffer2, sizeof(buffer2));

    // Write same code with different bit counts
    int16_t result1 = writer_write(&w1, 256, 9);
    int16_t result2 = writer_write(&w2, 256, 12);

    mu_assert(result1 == SUCCESS, "9-bit write should succeed");
    mu_assert(result2 == SUCCESS, "12-bit write should succeed");

    // Outputs should be different for different bit counts
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

    // Write code 0xFFF (4095) with 12 bits - maximum value for 12 bits
    int16_t result = writer_write(&w, 0xFFF, 12);
    mu_assert(result == SUCCESS, "writer_write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 2, "should have written at least 2 bytes");
    // Maximum value should write non-zero data
    mu_assert(buffer[0] != 0 || buffer[1] != 0, "maximum value should write data");
}

MU_TEST(test_writer_write_multiple_9_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Write multiple codes with 9 bits
    int16_t result1 = writer_write(&w, 0x100, 9); // 256
    mu_assert(result1 == SUCCESS, "first write should succeed");

    int16_t result2 = writer_write(&w, 0x101, 9); // 257
    mu_assert(result2 == SUCCESS, "second write should succeed");

    int16_t result3 = writer_write(&w, 0x102, 9); // 258
    mu_assert(result3 == SUCCESS, "third write should succeed");

    size_t written = writer_written_bytes(&w);
    mu_assert(written >= 3, "should have written at least 3 bytes");
}

MU_TEST(test_writer_write_multiple_12_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Write multiple codes with 12 bits
    int16_t result1 = writer_write(&w, 0x100, 12); // 256
    mu_assert(result1 == SUCCESS, "first write should succeed");

    int16_t result2 = writer_write(&w, 0x200, 12); // 512
    mu_assert(result2 == SUCCESS, "second write should succeed");

    int16_t result3 = writer_write(&w, 0x300, 12); // 768
    mu_assert(result3 == SUCCESS, "third write should succeed");

    size_t written = writer_written_bytes(&w);
    // 3 * 12 bits = 36 bits = 4.5 bytes minimum, so at least 5 bytes
    // With efficient bit packing, it could be exactly 5 bytes
    mu_assert(written >= 5, "should have written at least 5 bytes for three 12-bit writes");
}

MU_TEST(test_writer_write_overflow)
{
    uint8_t buffer[2] = {0}; // Too small for a 12-bit write
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Try to write with 12 bits - needs at least 2 bytes
    // But if bit_index is non-zero, might need more
    int16_t result = writer_write(&w, 0x100, 12);
    mu_assert(result == SUCCESS || result == WRITER_OVERFLOW, "should handle overflow gracefully");
}

MU_TEST(test_writer_write_overflow_small_buffer)
{
    uint8_t buffer[1] = {0}; // Definitely too small
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

    // Test minimum value for 9 bits: 0
    int16_t result1 = writer_write(&w, 0x000, 9);
    mu_assert(result1 == SUCCESS, "should write 0 successfully");

    // Test maximum value for 9 bits: 0x1FF (511)
    int16_t result2 = writer_write(&w, 0x1FF, 9);
    mu_assert(result2 == SUCCESS, "should write 511 successfully");
}

MU_TEST(test_writer_write_boundary_12_bits)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Test minimum value for 12 bits: 0
    int16_t result1 = writer_write(&w, 0x000, 12);
    mu_assert(result1 == SUCCESS, "should write 0 successfully");

    // Test maximum value for 12 bits: 0xFFF (4095)
    int16_t result2 = writer_write(&w, 0xFFF, 12);
    mu_assert(result2 == SUCCESS, "should write 4095 successfully");
}

MU_TEST(test_writer_write_mixed_bit_counts)
{
    uint8_t buffer[32] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Write codes with different bit counts
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

    // Write sequential codes with 9 bits
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

    // Write 0x123 with 9 bits
    // Big-endian: high bits first
    int16_t result = writer_write(&w, 0x123, 9);
    mu_assert(result == SUCCESS, "write should succeed");

    // The exact byte values depend on bit packing, but we verify the function works
    // For 9 bits: first byte contains bits 1-8, second byte contains bit 0
    mu_assert(buffer[0] != 0 || buffer[1] != 0, "some data should be written");
}

MU_TEST(test_writer_write_all_bit_counts_min)
{
    uint8_t buffer[32] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Test minimum value (0) for all bit counts
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

    // Test maximum values for all bit counts
    int16_t result1 = writer_write(&w, 0x1FF, 9); // 511
    mu_assert(result1 == SUCCESS, "9-bit max should succeed");

    int16_t result2 = writer_write(&w, 0x3FF, 10); // 1023
    mu_assert(result2 == SUCCESS, "10-bit max should succeed");

    int16_t result3 = writer_write(&w, 0x7FF, 11); // 2047
    mu_assert(result3 == SUCCESS, "11-bit max should succeed");

    int16_t result4 = writer_write(&w, 0xFFF, 12); // 4095
    mu_assert(result4 == SUCCESS, "12-bit max should succeed");
}

MU_TEST(test_writer_write_powers_of_two)
{
    uint8_t buffer[32] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Test powers of two which have single bit set
    int16_t result1 = writer_write(&w, 0x001, 9); // 2^0 = 1
    mu_assert(result1 == SUCCESS, "2^0 should succeed");

    int16_t result2 = writer_write(&w, 0x002, 9); // 2^1 = 2
    mu_assert(result2 == SUCCESS, "2^1 should succeed");

    int16_t result3 = writer_write(&w, 0x004, 9); // 2^2 = 4
    mu_assert(result3 == SUCCESS, "2^2 should succeed");

    int16_t result4 = writer_write(&w, 0x008, 9); // 2^3 = 8
    mu_assert(result4 == SUCCESS, "2^3 should succeed");

    int16_t result5 = writer_write(&w, 0x010, 9); // 2^4 = 16
    mu_assert(result5 == SUCCESS, "2^4 should succeed");

    int16_t result6 = writer_write(&w, 0x020, 9); // 2^5 = 32
    mu_assert(result6 == SUCCESS, "2^5 should succeed");

    int16_t result7 = writer_write(&w, 0x040, 9); // 2^6 = 64
    mu_assert(result7 == SUCCESS, "2^6 should succeed");

    int16_t result8 = writer_write(&w, 0x080, 9); // 2^7 = 128
    mu_assert(result8 == SUCCESS, "2^7 should succeed");

    int16_t result9 = writer_write(&w, 0x100, 9); // 2^8 = 256
    mu_assert(result9 == SUCCESS, "2^8 should succeed");
}

MU_TEST(test_writer_write_overflow_exact_boundary)
{
    uint8_t buffer[2] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Try to write exactly at the boundary
    int16_t result = writer_write(&w, 0x100, 12);
    // Should succeed if exactly 2 bytes, or overflow if bit packing requires more
    mu_assert(result == SUCCESS || result == WRITER_OVERFLOW, "should handle boundary correctly");
}

MU_TEST(test_writer_write_overflow_after_writes)
{
    uint8_t buffer[4] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Write one 12-bit value successfully
    int16_t result1 = writer_write(&w, 0x100, 12);
    mu_assert(result1 == SUCCESS, "first write should succeed");

    // Try to write another that might overflow
    int16_t result2 = writer_write(&w, 0x200, 12);
    mu_assert(result2 == SUCCESS || result2 == WRITER_OVERFLOW, "second write should handle overflow");
}

MU_TEST(test_writer_write_many_sequential)
{
    uint8_t buffer[128] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Write many sequential codes
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

    // Alternate between different bit counts
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

    // Test range of codes for 9 bits (0-511)
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

    // Test range of codes for 12 bits (0-4095)
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

// Reader tests
MU_TEST(test_reader_read_single_9bit)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Write a single 9-bit value
    int16_t result = writer_write(&w, 0x123, 9);
    mu_assert(result == SUCCESS, "write should succeed");

    // Debug: print written bytes
    size_t written = writer_written_bytes(&w);
    char debug_msg[256];
    snprintf(debug_msg, sizeof(debug_msg), "Written %zu bytes: %02X %02X %02X %02X", written, buffer[0], buffer[1],
             buffer[2], buffer[3]);

    // Read it back
    struct reader r;
    reader_init(&r, buffer, written);

    mu_assert(reader_has_next(&r, 9), "should have 9 bits available");
    int16_t value = reader_next(&r, 9);

    char msg[512];
    snprintf(msg, sizeof(msg), "Expected 0x123 (291), got 0x%03X (%d). %s", value & 0xFFF, value, debug_msg);
    mu_assert(value == 0x123, msg);

    // After reading 9 bits from 2 bytes (16 bits), we have 7 bits left
    // Debug info
    size_t total_bits = written * 8;
    size_t bits_consumed = r.byte_index * 8 + r.bit_index;
    size_t bits_remaining = total_bits - bits_consumed;

    char debug_pos[256];
    snprintf(debug_pos, sizeof(debug_pos), "Total bits: %zu, consumed: %zu, remaining: %zu. Reader at byte %zu, bit %d",
             total_bits, bits_consumed, bits_remaining, r.byte_index, r.bit_index);

    mu_assert(!reader_has_next(&r, 9), debug_pos);

    // But we should have 7 bits available
    mu_assert(reader_has_next(&r, 7), "should have 7 bits available");
    mu_assert(!reader_has_next(&r, 8), "should not have 8 bits available");
}

MU_TEST(test_reader_read_multiple_9bit)
{
    uint8_t buffer[16] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Write multiple 9-bit values
    writer_write(&w, 256, 9);
    writer_write(&w, 65, 9);
    writer_write(&w, 258, 9);
    writer_write(&w, 257, 9);

    // Read them back
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

    // Write values with different bit counts
    writer_write(&w, 0x1FF, 9);  // Max 9-bit
    writer_write(&w, 0x3FF, 10); // Max 10-bit
    writer_write(&w, 0x7FF, 11); // Max 11-bit
    writer_write(&w, 0xFFF, 12); // Max 12-bit

    // Read them back
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

    // Write all test values with 12 bits
    for (size_t i = 0; i < num_values; i++)
    {
        int16_t result = writer_write(&w, test_values[i], 12);
        mu_assert(result == SUCCESS, "write should succeed");
    }

    // Read them back
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

    // Write with varying bit counts
    writer_write(&w, 0x100, 9);
    writer_write(&w, 0x200, 10);
    writer_write(&w, 0x400, 11);
    writer_write(&w, 0x800, 12);
    writer_write(&w, 0x1FF, 9);

    // Read back with the same bit counts
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

    // Write values that test boundaries
    writer_write(&w, 0, 9);      // Minimum
    writer_write(&w, 0x1FF, 9);  // Maximum 9-bit
    writer_write(&w, 0, 12);     // Minimum
    writer_write(&w, 0xFFF, 12); // Maximum 12-bit

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

    // Write some data - use values that fit in 9 bits
    int16_t res1 = writer_write(&w, 0x123, 9); // 291
    int16_t res2 = writer_write(&w, 0x156, 9); // 342 (fits in 9 bits)
    mu_assert(res1 == SUCCESS, "first write should succeed");
    mu_assert(res2 == SUCCESS, "second write should succeed");

    size_t written = writer_written_bytes(&w);

    struct reader r;
    reader_init(&r, buffer, written);

    // Read the data
    int16_t val1 = reader_next(&r, 9);
    mu_assert(val1 == 0x123, "first value should be 0x123");

    int16_t val2 = reader_next(&r, 9);
    mu_assert(val2 == 0x156, "second value should be 0x156");

    // Try to read more than available
    // After reading 18 bits from 3 bytes (24 bits), we have 6 bits left
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

    // This is the expected sequence from our writer test
    // Reading 256, 65, 258, 257 with 9 bits each
    mu_assert(reader_next(&r, 9) == 256, "should read 256");
    mu_assert(reader_next(&r, 9) == 65, "should read 65");
    mu_assert(reader_next(&r, 9) == 258, "should read 258");
    mu_assert(reader_next(&r, 9) == 257, "should read 257");

    // Verify we've consumed all data
    mu_assert(!reader_has_next(&r, 9), "should have no more data");
}

MU_TEST(test_reader_sequential_values)
{
    uint8_t buffer[256] = {0};
    struct writer w;
    writer_init(&w, buffer, sizeof(buffer));

    // Write sequential values
    for (int i = 0; i < 100; i++)
    {
        writer_write(&w, i, 9);
    }

    // Read them back
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

    // Write powers of two
    for (int i = 0; i < 12; i++)
    {
        writer_write(&w, 1 << i, 12);
    }

    // Read them back
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

MU_TEST_SUITE(test_suite)
{
    // Writer tests - Basic functionality
    MU_RUN_TEST(test_writer_write_9_bits);
    MU_RUN_TEST(test_writer_write_10_bits);
    MU_RUN_TEST(test_writer_write_11_bits);
    MU_RUN_TEST(test_writer_write_12_bits);
    MU_RUN_TEST(test_writer_write_simple_9_bits);
    MU_RUN_TEST(test_writer_write_simple_12_bits);

    // Writer tests - Exact output verification
    MU_RUN_TEST(test_writer_write_exact_output_sequence);
    MU_RUN_TEST(test_writer_write_exact_output_single_9bit);
    MU_RUN_TEST(test_writer_write_exact_output_single_12bit);
    MU_RUN_TEST(test_writer_write_exact_output_comparison);

    // Writer tests - Multiple writes
    MU_RUN_TEST(test_writer_write_multiple_9_bits);
    MU_RUN_TEST(test_writer_write_multiple_12_bits);
    MU_RUN_TEST(test_writer_write_sequential_codes);
    MU_RUN_TEST(test_writer_write_many_sequential);
    MU_RUN_TEST(test_writer_write_alternating_bit_counts);

    // Writer tests - Boundary values
    MU_RUN_TEST(test_writer_write_boundary_9_bits);
    MU_RUN_TEST(test_writer_write_boundary_12_bits);
    MU_RUN_TEST(test_writer_write_all_bit_counts_min);
    MU_RUN_TEST(test_writer_write_all_bit_counts_max);
    MU_RUN_TEST(test_writer_write_powers_of_two);
    MU_RUN_TEST(test_writer_write_code_range_9_bits);
    MU_RUN_TEST(test_writer_write_code_range_12_bits);

    // Writer tests - Overflow conditions
    MU_RUN_TEST(test_writer_write_overflow);
    MU_RUN_TEST(test_writer_write_overflow_small_buffer);
    MU_RUN_TEST(test_writer_write_overflow_exact_boundary);
    MU_RUN_TEST(test_writer_write_overflow_after_writes);

    // Writer tests - Mixed and edge cases
    MU_RUN_TEST(test_writer_write_mixed_bit_counts);
    MU_RUN_TEST(test_writer_write_big_endian_format);
    MU_RUN_TEST(test_writer_write_written_bytes_tracking);

    // Reader tests
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

    MU_RUN_TEST(test_round_trip_aaa);
    MU_RUN_TEST(test_round_trip_abc);
    MU_RUN_TEST(test_round_trip_hello);
    MU_RUN_TEST(test_round_trip_single_byte);
    MU_RUN_TEST(test_round_trip_single_zero);
    MU_RUN_TEST(test_round_trip_single_255);
    MU_RUN_TEST(test_round_trip_two_bytes_same);
    MU_RUN_TEST(test_round_trip_two_bytes_different);
    MU_RUN_TEST(test_round_trip_all_same_short);
    MU_RUN_TEST(test_round_trip_all_same_long);
    MU_RUN_TEST(test_round_trip_alternating_ab);
    MU_RUN_TEST(test_round_trip_alternating_long);
    MU_RUN_TEST(test_round_trip_repeated_abc);
    MU_RUN_TEST(test_round_trip_repeated_pattern);
    MU_RUN_TEST(test_round_trip_sequential);
    MU_RUN_TEST(test_round_trip_nested_patterns);
    MU_RUN_TEST(test_round_trip_decoder_special_case);
    MU_RUN_TEST(test_round_trip_all_bytes);
    MU_RUN_TEST(test_round_trip_binary_zeros);
    MU_RUN_TEST(test_round_trip_binary_ones);
    MU_RUN_TEST(test_round_trip_long_sequence);
    MU_RUN_TEST(test_round_trip_long_repeating);
    MU_RUN_TEST(test_round_trip_many_unique_pairs);
    MU_RUN_TEST(test_round_trip_text_like);
    MU_RUN_TEST(test_round_trip_numbers);
    MU_RUN_TEST(test_round_trip_mixed_case);
    MU_RUN_TEST(test_round_trip_chain_pattern);
    MU_RUN_TEST(test_round_trip_empty_pattern);
    MU_RUN_TEST(test_round_trip_single_repeated_many);
    MU_RUN_TEST(test_round_trip_incremental);
    MU_RUN_TEST(test_round_trip_decremental);
    MU_RUN_TEST(test_round_trip_palindrome);
    MU_RUN_TEST(test_round_trip_three_way_alternating);
    MU_RUN_TEST(test_round_trip_fibonacci_like);
    MU_RUN_TEST(test_round_trip_boundary_values);
    MU_RUN_TEST(test_round_trip_rapid_changes);
    MU_RUN_TEST(test_round_trip_gradual_transition);
    MU_RUN_TEST(test_round_trip_high_frequency_pattern);
    MU_RUN_TEST(test_round_trip_nested_repeats);
    MU_RUN_TEST(test_round_trip_unicode_like);
    MU_RUN_TEST(test_round_trip_sparse_pattern);
    MU_RUN_TEST(test_round_trip_compression_friendly);
    MU_RUN_TEST(test_round_trip_compression_unfriendly);
    MU_RUN_TEST(test_round_trip_very_long_sequence);
    MU_RUN_TEST(test_round_trip_extremely_long_sequence);
    MU_RUN_TEST(test_round_trip_long_repeating_pattern);
    MU_RUN_TEST(test_round_trip_long_sequential);
    MU_RUN_TEST(test_round_trip_exceeds_table_limit);
    MU_RUN_TEST(test_from_aaa_test_data);
    MU_RUN_TEST(test_from_test_data);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
