#include "../minunit/minunit.h"

#include "common.h"
#include "decode.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_base(const char *encoded_path, const char *expected_path)
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

MU_TEST(test_data_in_out)
{
    test_base("test_data/in", "test_data/out");
}

MU_TEST(test_data_aaa)
{
    test_base("test_data/aaa.enc", "test_data/aaa.dec");
}

MU_TEST(test_data_10_1)
{
    test_base("test_data/10_1.enc", "test_data/10_1.dec");
}

MU_TEST(test_data_10_2)
{
    test_base("test_data/10_2.enc", "test_data/10_2.dec");
}

MU_TEST(test_data_10_3)
{
    test_base("test_data/10_3.enc", "test_data/10_3.dec");
}

MU_TEST(test_data_100_1)
{
    test_base("test_data/100_1.enc", "test_data/100_1.dec");
}

MU_TEST(test_data_100_2)
{
    test_base("test_data/100_2.enc", "test_data/100_2.dec");
}

MU_TEST(test_data_100_3)
{
    test_base("test_data/100_3.enc", "test_data/100_3.dec");
}

MU_TEST(test_data_1000)
{
    test_base("test_data/1000.enc", "test_data/1000.dec");
}

MU_TEST_SUITE(decode_suite)
{
    MU_RUN_TEST(test_data_aaa);
    MU_RUN_TEST(test_data_in_out);
    MU_RUN_TEST(test_data_10_1);
    MU_RUN_TEST(test_data_10_2);
    MU_RUN_TEST(test_data_10_3);
    MU_RUN_TEST(test_data_100_1);
    MU_RUN_TEST(test_data_100_2);
    MU_RUN_TEST(test_data_100_3);
    MU_RUN_TEST(test_data_1000);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(decode_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
