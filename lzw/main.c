#include "../minunit/minunit.h"

#include "common.h"
#include "decode.h"
#include "encode.h"

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
    FILE *f_encoded = fopen("test_data/in", "rb");
    mu_assert(f_encoded != NULL, "failed to open test_data/in");

    FILE *f_expected = fopen("test_data/out", "rb");
    mu_assert(f_expected != NULL, "failed to open test_data/out");

    // Get file sizes
    fseek(f_encoded, 0, SEEK_END);
    long encoded_size = ftell(f_encoded);
    fseek(f_encoded, 0, SEEK_SET);

    fseek(f_expected, 0, SEEK_END);
    long expected_size = ftell(f_expected);
    fseek(f_expected, 0, SEEK_SET);

    mu_assert(encoded_size > 0, "encoded file should not be empty");
    mu_assert(expected_size > 0, "expected file should not be empty");

    // Allocate buffers
    uint8_t *encoded = malloc((size_t)encoded_size);
    uint8_t *expected = malloc((size_t)expected_size);
    uint8_t *decoded = malloc((size_t)expected_size * 2); // Extra space for safety

    mu_assert(encoded != NULL && expected != NULL && decoded != NULL, "memory allocation failed");

    // Read files
    size_t read_encoded = fread(encoded, 1, (size_t)encoded_size, f_encoded);
    size_t read_expected = fread(expected, 1, (size_t)expected_size, f_expected);

    fclose(f_encoded);
    fclose(f_expected);

    mu_assert(read_encoded == (size_t)encoded_size, "failed to read all encoded data");
    mu_assert(read_expected == (size_t)expected_size, "failed to read all expected data");

    // Decode
    size_t decoded_size = lzw_decode(encoded, (size_t)encoded_size, decoded, (size_t)expected_size * 2);
    mu_assert(decoded_size != (size_t)-1, "decoding failed");
    mu_assert(decoded_size == (size_t)expected_size, "decoded size mismatch");

    // Compare
    mu_assert(memcmp(expected, decoded, (size_t)expected_size) == 0, "decoded content does not match expected");

    free(encoded);
    free(expected);
    free(decoded);
}

MU_TEST_SUITE(test_suite)
{
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
    // MU_RUN_TEST(test_from_test_data);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
