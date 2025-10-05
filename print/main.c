#include "print.h"

#include "../minunit/minunit.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static void compute_expected(const char *input, const char *format, char *out)
{
    int32_t x;
    sscanf(input, "%x", &x);
    sprintf(out, format, x);
}

static void test(const char *input, const char *format)
{
    char expected[32];
    compute_expected(input, format, expected);
    char actual[32];
    print(actual, format, input);
    mu_assert_string_eq(expected, actual);
}

MU_TEST(test_1)
{
    test("DEAD", "%i");
}

MU_TEST(test_2)
{
    test("BEEF", "%i");
}

MU_TEST(test_3)
{
    test("DEADBEEF", "%i");
}

MU_TEST(test_4)
{
    test("BEEFDEAD", "%i");
}

MU_TEST(test_5)
{
    test("FFFFFFFF", "%i");
}

MU_TEST(test_6)
{
    test("DEAD", "%10i");
}

MU_TEST(test_7)
{
    test("DEAD", "%-10i");
}

MU_TEST(test_8)
{
    test("DEAD", "% +i");
}

MU_TEST(test_zero_no_format)
{
    test("0", "%i");
}

MU_TEST(test_positive_small)
{
    test("2A", "%i");
}

MU_TEST(test_positive_max)
{
    test("7FFFFFFF", "%i");
}

MU_TEST(test_negative_small)
{
    test("FFFFFFFF", "%i");
}

MU_TEST(test_negative_min)
{
    test("80000000", "%i");
}

MU_TEST(test_width_5_positive)
{
    test("7B", "%5i");
}

MU_TEST(test_width_5_negative)
{
    test("FFFFFF85", "%5i");
}

MU_TEST(test_width_10_positive)
{
    test("DEAD", "%10i");
}

MU_TEST(test_width_10_negative)
{
    test("FFFF0000", "%10i");
}

MU_TEST(test_width_shorter_than_number)
{
    test("7FFFFFFF", "%5i");
}

MU_TEST(test_left_align_no_width)
{
    test("42", "%-i");
}

MU_TEST(test_left_align_width_positive)
{
    test("2A", "%-8i");
}

MU_TEST(test_left_align_width_negative)
{
    test("FFFFFFD6", "%-8i");
}

MU_TEST(test_left_align_width_large)
{
    test("DEADBEEF", "%-15i");
}

MU_TEST(test_plus_no_width_positive)
{
    test("42", "%+i");
}

MU_TEST(test_plus_no_width_negative)
{
    test("FFFFFFD6", "%+i");
}

MU_TEST(test_plus_width_positive)
{
    test("7B", "%+8i");
}

MU_TEST(test_plus_width_negative)
{
    test("FFFFFF85", "%+8i");
}

MU_TEST(test_plus_zero)
{
    test("0", "%+i");
}

MU_TEST(test_space_no_width_positive)
{
    test("42", "% i");
}

MU_TEST(test_space_no_width_negative)
{
    test("FFFFFFD6", "% i");
}

MU_TEST(test_space_width_positive)
{
    test("7B", "% 8i");
}

MU_TEST(test_space_width_negative)
{
    test("FFFFFF85", "% 8i");
}

MU_TEST(test_space_zero)
{
    test("0", "% i");
}

MU_TEST(test_zero_flag_width_positive)
{
    test("7B", "%08i");
}

MU_TEST(test_zero_flag_width_negative)
{
    test("FFFFFF85", "%08i");
}

MU_TEST(test_zero_flag_width_large)
{
    test("DEADBEEF", "%015i");
}

MU_TEST(test_zero_flag_no_width)
{
    test("42", "%0i");
}

MU_TEST(test_left_align_plus_positive)
{
    test("7B", "%-+10i");
}

MU_TEST(test_left_align_plus_negative)
{
    test("FFFFFF85", "%-+10i");
}

MU_TEST(test_left_align_space_positive)
{
    test("7B", "%- 10i");
}

MU_TEST(test_left_align_space_negative)
{
    test("FFFFFF85", "%- 10i");
}

MU_TEST(test_plus_zero_flag_positive)
{
    test("7B", "%+08i");
}

MU_TEST(test_plus_zero_flag_negative)
{
    test("FFFFFF85", "%+08i");
}

MU_TEST(test_space_zero_flag_positive)
{
    test("7B", "% 08i");
}

MU_TEST(test_space_zero_flag_negative)
{
    test("FFFFFF85", "% 08i");
}

MU_TEST(test_left_align_plus_zero_positive)
{
    test("7B", "%-+08i");
}

MU_TEST(test_left_align_plus_zero_negative)
{
    test("FFFFFF85", "%-+08i");
}

MU_TEST(test_left_align_space_zero_positive)
{
    test("7B", "%- 08i");
}

MU_TEST(test_left_align_space_zero_negative)
{
    test("FFFFFF85", "%- 08i");
}

MU_TEST(test_plus_space_conflict)
{
    test("42", "% +8i");
}

MU_TEST(test_all_flags_combined)
{
    test("DEADBEEF", "%-+ 015i");
}

MU_TEST(test_width_1_positive)
{
    test("5", "%1i");
}

MU_TEST(test_width_1_negative)
{
    test("FFFFFFFB", "%1i");
}

MU_TEST(test_boundary_positive_max_minus_1)
{
    test("7FFFFFFE", "%+012i");
}

MU_TEST(test_boundary_negative_min_plus_1)
{
    test("80000001", "%+012i");
}

MU_TEST(test_zero_with_all_formats)
{
    test("0", "%-+010i");
}

MU_TEST(test_minus_one)
{
    test("FFFFFFFF", "%i");
}

MU_TEST(test_one)
{
    test("1", "%i");
}

MU_TEST(test_int32_max)
{
    test("7FFFFFFF", "%i");
}

MU_TEST(test_int32_min)
{
    test("80000000", "%i");
}

MU_TEST(test_width_zero)
{
    test("42", "%0i");
}

MU_TEST(test_width_exact_match_positive)
{
    test("7B", "%3i");
}

MU_TEST(test_width_exact_match_negative)
{
    test("FFFFFF85", "%4i");
}

MU_TEST(test_width_exact_match_with_plus)
{
    test("7B", "%+4i");
}

MU_TEST(test_width_exact_match_with_space)
{
    test("7B", "% 4i");
}

MU_TEST(test_very_large_width)
{
    test("42", "%25i");
}

MU_TEST(test_int32_max_with_plus)
{
    test("7FFFFFFF", "%+i");
}

MU_TEST(test_int32_min_with_plus)
{
    test("80000000", "%+i");
}

MU_TEST(test_int32_max_with_space)
{
    test("7FFFFFFF", "% i");
}

MU_TEST(test_int32_min_with_space)
{
    test("80000000", "% i");
}

MU_TEST(test_int32_max_with_width)
{
    test("7FFFFFFF", "%15i");
}

MU_TEST(test_int32_min_with_width)
{
    test("80000000", "%15i");
}

MU_TEST(test_int32_max_with_zero_flag)
{
    test("7FFFFFFF", "%015i");
}

MU_TEST(test_int32_min_with_zero_flag)
{
    test("80000000", "%015i");
}

MU_TEST(test_int32_max_left_align)
{
    test("7FFFFFFF", "%-15i");
}

MU_TEST(test_int32_min_left_align)
{
    test("80000000", "%-15i");
}

MU_TEST(test_single_digit_all_flags)
{
    test("5", "%-+010i");
}

MU_TEST(test_two_digits_all_flags)
{
    test("42", "%-+010i");
}

MU_TEST(test_three_digits_all_flags)
{
    test("1F4", "%-+010i");
}

MU_TEST(test_no_flags_no_width)
{
    test("ABC", "%i");
}

MU_TEST(test_only_minus_no_width)
{
    test("ABC", "%-i");
}

MU_TEST(test_only_plus_no_width)
{
    test("ABC", "%+i");
}

MU_TEST(test_only_space_no_width)
{
    test("ABC", "% i");
}

MU_TEST(test_only_zero_no_width)
{
    test("ABC", "%0i");
}

MU_TEST(test_minus_plus_no_width)
{
    test("ABC", "%-+i");
}

MU_TEST(test_minus_space_no_width)
{
    test("ABC", "%- i");
}

MU_TEST(test_minus_zero_no_width)
{
    test("ABC", "%-0i");
}

MU_TEST(test_plus_zero_no_width)
{
    test("ABC", "%+0i");
}

MU_TEST(test_space_zero_no_width)
{
    test("ABC", "% 0i");
}

MU_TEST(test_plus_space_no_width)
{
    test("ABC", "%+ i");
}

MU_TEST(test_minus_plus_space_no_width)
{
    test("ABC", "%-+ i");
}

MU_TEST(test_minus_plus_zero_no_width)
{
    test("ABC", "%-+0i");
}

MU_TEST(test_minus_space_zero_no_width)
{
    test("ABC", "%- 0i");
}

MU_TEST(test_plus_space_zero_no_width)
{
    test("ABC", "%+ 0i");
}

MU_TEST(test_all_four_flags_no_width)
{
    test("ABC", "%-+ 0i");
}

MU_TEST(test_zero_flag_width_1)
{
    test("5", "%01i");
}

MU_TEST(test_zero_flag_width_2)
{
    test("5", "%02i");
}

MU_TEST(test_plus_width_1_positive)
{
    test("5", "%+1i");
}

MU_TEST(test_plus_width_2_positive)
{
    test("5", "%+2i");
}

MU_TEST(test_plus_width_3_positive)
{
    test("5", "%+3i");
}

MU_TEST(test_space_width_1_positive)
{
    test("5", "% 1i");
}

MU_TEST(test_space_width_2_positive)
{
    test("5", "% 2i");
}

MU_TEST(test_space_width_3_positive)
{
    test("5", "% 3i");
}

MU_TEST(test_negative_with_width_exact)
{
    test("FFFFFFFB", "%2i");
}

MU_TEST(test_negative_with_width_plus_one)
{
    test("FFFFFFFB", "%3i");
}

MU_TEST(test_zero_only_plus)
{
    test("0", "%+i");
}

MU_TEST(test_zero_only_space)
{
    test("0", "% i");
}

MU_TEST(test_zero_only_zero_flag)
{
    test("0", "%0i");
}

MU_TEST(test_zero_only_minus)
{
    test("0", "%-i");
}

MU_TEST(test_zero_plus_width)
{
    test("0", "%+5i");
}

MU_TEST(test_zero_space_width)
{
    test("0", "% 5i");
}

MU_TEST(test_zero_zero_flag_width)
{
    test("0", "%05i");
}

MU_TEST(test_zero_minus_width)
{
    test("0", "%-5i");
}

MU_TEST(test_zero_plus_zero_flag_width)
{
    test("0", "%+05i");
}

MU_TEST(test_zero_space_zero_flag_width)
{
    test("0", "% 05i");
}

MU_TEST(test_zero_minus_plus_width)
{
    test("0", "%-+5i");
}

MU_TEST(test_zero_minus_space_width)
{
    test("0", "%- 5i");
}

MU_TEST(test_minus_one_all_formats)
{
    test("FFFFFFFF", "%-+012i");
}

MU_TEST(test_one_all_formats)
{
    test("1", "%-+012i");
}

MU_TEST(test_100_no_format)
{
    test("64", "%i");
}

MU_TEST(test_1000_no_format)
{
    test("3E8", "%i");
}

MU_TEST(test_10000_no_format)
{
    test("2710", "%i");
}

MU_TEST(test_100000_no_format)
{
    test("186A0", "%i");
}

MU_TEST(test_1000000_no_format)
{
    test("F4240", "%i");
}

MU_TEST(test_negative_100)
{
    test("FFFFFF9C", "%i");
}

MU_TEST(test_negative_1000)
{
    test("FFFFFC18", "%i");
}

MU_TEST(test_negative_10000)
{
    test("FFFFD8F0", "%i");
}

MU_TEST(test_powers_of_two_1)
{
    test("1", "%+010i");
}

MU_TEST(test_powers_of_two_2)
{
    test("2", "%+010i");
}

MU_TEST(test_powers_of_two_4)
{
    test("4", "%+010i");
}

MU_TEST(test_powers_of_two_8)
{
    test("8", "%+010i");
}

MU_TEST(test_powers_of_two_16)
{
    test("10", "%+010i");
}

MU_TEST(test_powers_of_two_256)
{
    test("100", "%+010i");
}

MU_TEST(test_powers_of_two_1024)
{
    test("400", "%+010i");
}

MU_TEST(test_powers_of_two_65536)
{
    test("10000", "%+010i");
}

MU_TEST(test_combo_minus_plus_width_5)
{
    test("2A", "%-+5i");
}

MU_TEST(test_combo_minus_plus_width_10)
{
    test("2A", "%-+10i");
}

MU_TEST(test_combo_minus_plus_width_15)
{
    test("2A", "%-+15i");
}

MU_TEST(test_combo_minus_space_width_5)
{
    test("2A", "%- 5i");
}

MU_TEST(test_combo_minus_space_width_10)
{
    test("2A", "%- 10i");
}

MU_TEST(test_combo_plus_zero_width_5)
{
    test("2A", "%+05i");
}

MU_TEST(test_combo_plus_zero_width_10)
{
    test("2A", "%+010i");
}

MU_TEST(test_combo_plus_zero_width_15)
{
    test("2A", "%+015i");
}

MU_TEST(test_combo_space_zero_width_5)
{
    test("2A", "% 05i");
}

MU_TEST(test_combo_space_zero_width_10)
{
    test("2A", "% 010i");
}

MU_TEST(test_combo_all_flags_width_5)
{
    test("2A", "%-+ 05i");
}

MU_TEST(test_combo_all_flags_width_10)
{
    test("2A", "%-+ 010i");
}

MU_TEST(test_combo_all_flags_width_20)
{
    test("2A", "%-+ 020i");
}

MU_TEST(test_negative_combo_minus_plus_width_5)
{
    test("FFFFFFD6", "%-+5i");
}

MU_TEST(test_negative_combo_minus_space_width_5)
{
    test("FFFFFFD6", "%- 5i");
}

MU_TEST(test_negative_combo_plus_zero_width_5)
{
    test("FFFFFFD6", "%+05i");
}

MU_TEST(test_negative_combo_space_zero_width_5)
{
    test("FFFFFFD6", "% 05i");
}

MU_TEST(test_negative_combo_all_flags_width_5)
{
    test("FFFFFFD6", "%-+ 05i");
}

MU_TEST(test_max_digits_no_sign)
{
    test("7FFFFFFF", "%10i");
}

MU_TEST(test_max_digits_with_sign)
{
    test("7FFFFFFF", "%+11i");
}

MU_TEST(test_min_digits_no_extra_sign)
{
    test("80000000", "%11i");
}

MU_TEST(test_width_20_positive)
{
    test("2A", "%20i");
}

MU_TEST(test_width_20_negative)
{
    test("FFFFFFD6", "%20i");
}

MU_TEST(test_width_20_plus_positive)
{
    test("2A", "%+20i");
}

MU_TEST(test_width_20_space_positive)
{
    test("2A", "% 20i");
}

MU_TEST(test_width_20_zero_positive)
{
    test("2A", "%020i");
}

MU_TEST(test_width_20_minus_positive)
{
    test("2A", "%-20i");
}

MU_TEST(test_all_combos_width_no_flags)
{
    test("2A", "%8i");
}

MU_TEST(test_all_combos_width_minus)
{
    test("2A", "%-8i");
}

MU_TEST(test_all_combos_width_plus)
{
    test("2A", "%+8i");
}

MU_TEST(test_all_combos_width_space)
{
    test("2A", "% 8i");
}

MU_TEST(test_all_combos_width_zero)
{
    test("2A", "%08i");
}

MU_TEST(test_all_combos_width_minus_plus)
{
    test("2A", "%-+8i");
}

MU_TEST(test_all_combos_width_minus_space)
{
    test("2A", "%- 8i");
}

MU_TEST(test_all_combos_width_minus_zero)
{
    test("2A", "%-08i");
}

MU_TEST(test_all_combos_width_plus_zero)
{
    test("2A", "%+08i");
}

MU_TEST(test_all_combos_width_space_zero)
{
    test("2A", "% 08i");
}

MU_TEST(test_all_combos_width_plus_space)
{
    test("2A", "%+ 8i");
}

MU_TEST(test_all_combos_width_minus_plus_space)
{
    test("2A", "%-+ 8i");
}

MU_TEST(test_all_combos_width_minus_plus_zero)
{
    test("2A", "%-+08i");
}

MU_TEST(test_all_combos_width_minus_space_zero)
{
    test("2A", "%- 08i");
}

MU_TEST(test_all_combos_width_plus_space_zero)
{
    test("2A", "%+ 08i");
}

MU_TEST(test_all_combos_width_all_four)
{
    test("2A", "%-+ 08i");
}

MU_TEST(test_all_combos_neg_no_flags)
{
    test("FFFFFFD6", "%8i");
}

MU_TEST(test_all_combos_neg_minus)
{
    test("FFFFFFD6", "%-8i");
}

MU_TEST(test_all_combos_neg_plus)
{
    test("FFFFFFD6", "%+8i");
}

MU_TEST(test_all_combos_neg_space)
{
    test("FFFFFFD6", "% 8i");
}

MU_TEST(test_all_combos_neg_zero)
{
    test("FFFFFFD6", "%08i");
}

MU_TEST(test_all_combos_neg_minus_plus)
{
    test("FFFFFFD6", "%-+8i");
}

MU_TEST(test_all_combos_neg_minus_space)
{
    test("FFFFFFD6", "%- 8i");
}

MU_TEST(test_all_combos_neg_minus_zero)
{
    test("FFFFFFD6", "%-08i");
}

MU_TEST(test_all_combos_neg_plus_zero)
{
    test("FFFFFFD6", "%+08i");
}

MU_TEST(test_all_combos_neg_space_zero)
{
    test("FFFFFFD6", "% 08i");
}

MU_TEST(test_all_combos_neg_plus_space)
{
    test("FFFFFFD6", "%+ 8i");
}

MU_TEST(test_all_combos_neg_minus_plus_space)
{
    test("FFFFFFD6", "%-+ 8i");
}

MU_TEST(test_all_combos_neg_minus_plus_zero)
{
    test("FFFFFFD6", "%-+08i");
}

MU_TEST(test_all_combos_neg_minus_space_zero)
{
    test("FFFFFFD6", "%- 08i");
}

MU_TEST(test_all_combos_neg_plus_space_zero)
{
    test("FFFFFFD6", "%+ 08i");
}

MU_TEST(test_all_combos_neg_all_four)
{
    test("FFFFFFD6", "%-+ 08i");
}

MU_TEST(test_zero_all_combos_minus_plus)
{
    test("0", "%-+8i");
}

MU_TEST(test_zero_all_combos_plus_space_zero)
{
    test("0", "%+ 08i");
}

MU_TEST(test_zero_all_combos_all_four)
{
    test("0", "%-+ 08i");
}

MU_TEST(test_max_all_combos_plus_zero)
{
    test("7FFFFFFF", "%+012i");
}

MU_TEST(test_max_all_combos_minus_plus)
{
    test("7FFFFFFF", "%-+12i");
}

MU_TEST(test_max_all_combos_all_four)
{
    test("7FFFFFFF", "%-+ 015i");
}

MU_TEST(test_min_all_combos_plus_zero)
{
    test("80000000", "%+013i");
}

MU_TEST(test_min_all_combos_minus_plus)
{
    test("80000000", "%-+13i");
}

MU_TEST(test_min_all_combos_space_zero)
{
    test("80000000", "% 013i");
}

MU_TEST(test_reversed_order_space_plus)
{
    test("2A", "% +8i");
}

MU_TEST(test_reversed_order_zero_plus)
{
    test("2A", "%0+8i");
}

MU_TEST(test_reversed_order_zero_minus)
{
    test("2A", "%0-8i");
}

MU_TEST(test_reversed_order_space_plus_minus)
{
    test("2A", "% +-8i");
}

MU_TEST(test_exact_width_no_flags)
{
    test("2A", "%2i");
}

MU_TEST(test_exact_width_plus)
{
    test("2A", "%+3i");
}

MU_TEST(test_exact_width_space)
{
    test("2A", "% 3i");
}

MU_TEST(test_exact_width_neg_no_flags)
{
    test("FFFFFFD6", "%3i");
}

MU_TEST(test_exact_width_neg_plus)
{
    test("FFFFFFD6", "%+4i");
}

MU_TEST(test_shorter_width_plus)
{
    test("3E8", "%+2i");
}

MU_TEST(test_shorter_width_space)
{
    test("3E8", "% 2i");
}

MU_TEST(test_shorter_width_zero)
{
    test("3E8", "%02i");
}

MU_TEST(test_multiple_digits_all_combos_1)
{
    test("64", "%-+010i");
}

MU_TEST(test_multiple_digits_all_combos_2)
{
    test("3E8", "%+ 012i");
}

MU_TEST(test_multiple_digits_all_combos_3)
{
    test("2710", "%-08i");
}

MU_TEST(test_negative_via_minus_sign_1)
{
    test("-A", "%i");
}

MU_TEST(test_negative_via_minus_sign_2)
{
    test("-1F", "%+8i");
}

MU_TEST(test_negative_via_minus_sign_3)
{
    test("-64", "% 010i");
}

MU_TEST(test_negative_via_minus_sign_large)
{
    test("-7FFFFFFF", "%+12i");
}

MU_TEST(test_negative_via_high_bit_1)
{
    test("FFFFFFFE", "%i");
}

MU_TEST(test_negative_via_high_bit_2)
{
    test("FFFFFF00", "%+8i");
}

MU_TEST(test_negative_via_high_bit_3)
{
    test("80000001", "% 012i");
}

MU_TEST(test_negative_via_high_bit_small)
{
    test("FFFFFFF0", "%-8i");
}

MU_TEST(test_double_negation_1)
{
    test("-FFFFFFFF", "%i");
}

MU_TEST(test_double_negation_2)
{
    test("-FFFFFFFE", "%+8i");
}

MU_TEST(test_double_negation_3)
{
    test("-80000001", "%i");
}

MU_TEST(test_double_negation_with_format)
{
    test("-80000010", "%-+012i");
}

MU_TEST(test_varied_positive_1)
{
    test("7B", "%+010i");
}

MU_TEST(test_varied_positive_2)
{
    test("C8", "% 08i");
}

MU_TEST(test_varied_positive_3)
{
    test("3E7", "%-+10i");
}

MU_TEST(test_varied_positive_4)
{
    test("1388", "%012i");
}

MU_TEST(test_varied_positive_5)
{
    test("4E20", "%+08i");
}

MU_TEST(test_varied_negative_via_minus_1)
{
    test("-C8", "%08i");
}

MU_TEST(test_varied_negative_via_minus_2)
{
    test("-3E8", "%-+10i");
}

MU_TEST(test_varied_negative_via_minus_3)
{
    test("-1388", "%+012i");
}

MU_TEST(test_varied_negative_via_high_bit_1)
{
    test("FFFFFF38", "%010i");
}

MU_TEST(test_varied_negative_via_high_bit_2)
{
    test("FFFFFC18", "%-+12i");
}

MU_TEST(test_varied_negative_via_high_bit_3)
{
    test("FFFFEC78", "% 012i");
}

MU_TEST(test_odd_length_positive_1)
{
    test("F", "%i");
}

MU_TEST(test_odd_length_positive_2)
{
    test("FFF", "%+8i");
}

MU_TEST(test_odd_length_positive_3)
{
    test("12345", "% 010i");
}

MU_TEST(test_odd_length_negative_minus_1)
{
    test("-F", "%i");
}

MU_TEST(test_odd_length_negative_minus_2)
{
    test("-ABC", "%+08i");
}

MU_TEST(test_odd_length_negative_minus_3)
{
    test("-12345", "%-+010i");
}

MU_TEST(test_odd_length_negative_high_bit_1)
{
    test("FFFFFFF1", "%i");
}

MU_TEST(test_odd_length_negative_high_bit_2)
{
    test("FFFFF544", "%+08i");
}

MU_TEST(test_odd_length_double_negation_1)
{
    test("-FFFFFFF1", "%i");
}

MU_TEST(test_odd_length_double_negation_2)
{
    test("-FFFFF544", "%-+010i");
}

MU_TEST(test_edge_around_zero_1)
{
    test("1", "%+5i");
}

MU_TEST(test_edge_around_zero_2)
{
    test("-1", "%+5i");
}

MU_TEST(test_edge_around_zero_3)
{
    test("FFFFFFFF", "%+5i");
}

MU_TEST(test_edge_around_zero_4)
{
    test("-FFFFFFFF", "%+5i");
}

MU_TEST(test_powers_positive_varied)
{
    test("80", "%+08i");
}

MU_TEST(test_powers_negative_minus)
{
    test("-80", "%+08i");
}

MU_TEST(test_powers_negative_high_bit)
{
    test("FFFFFF80", "%+08i");
}

MU_TEST(test_max_positive_variations_1)
{
    test("7FFFFFFE", "%-+15i");
}

MU_TEST(test_max_positive_variations_2)
{
    test("7FFFFFFD", "% 015i");
}

MU_TEST(test_max_positive_variations_3)
{
    test("7FFFFFF0", "%+012i");
}

MU_TEST(test_min_negative_variations_1)
{
    test("80000001", "%-+15i");
}

MU_TEST(test_min_negative_variations_2)
{
    test("80000010", "% 015i");
}

MU_TEST(test_min_negative_variations_3)
{
    test("80000100", "%+012i");
}

MU_TEST(test_combo_varied_1)
{
    test("5DC", "%-+ 010i");
}

MU_TEST(test_combo_varied_2)
{
    test("-5DC", "%-+ 010i");
}

MU_TEST(test_combo_varied_3)
{
    test("FFFFFA24", "%-+ 010i");
}

MU_TEST(test_combo_varied_4)
{
    test("-FFFFFA24", "%-+ 010i");
}

MU_TEST(test_small_numbers_varied_1)
{
    test("7", "%+05i");
}

MU_TEST(test_small_numbers_varied_2)
{
    test("-9", "%+05i");
}

MU_TEST(test_small_numbers_varied_3)
{
    test("FFFFFFF7", "%+05i");
}

MU_TEST(test_small_numbers_varied_4)
{
    test("-FFFFFFF7", "%+05i");
}

MU_TEST(test_three_digit_variations_1)
{
    test("1C8", "%-+8i");
}

MU_TEST(test_three_digit_variations_2)
{
    test("-1C8", "% 08i");
}

MU_TEST(test_three_digit_variations_3)
{
    test("FFFFFE38", "%+010i");
}

MU_TEST(test_boundary_near_max_1)
{
    test("7FFFFFF1", "%+i");
}

MU_TEST(test_boundary_near_max_2)
{
    test("7FFFFFF2", "%-+12i");
}

MU_TEST(test_boundary_near_min_1)
{
    test("80000011", "%+i");
}

MU_TEST(test_boundary_near_min_2)
{
    test("80000012", "%-+12i");
}

MU_TEST(test_all_hex_digits_A)
{
    test("AAA", "%+08i");
}

MU_TEST(test_all_hex_digits_B)
{
    test("BBB", "% 08i");
}

MU_TEST(test_all_hex_digits_C)
{
    test("CCC", "%-+08i");
}

MU_TEST(test_all_hex_digits_D)
{
    test("DDD", "%010i");
}

MU_TEST(test_all_hex_digits_E)
{
    test("EEE", "%+08i");
}

MU_TEST(test_all_hex_digits_F)
{
    test("FFF", "% 08i");
}

MU_TEST(test_mixed_case_hex_1)
{
    test("AbC", "%i");
}

MU_TEST(test_mixed_case_hex_2)
{
    test("dEf", "%+8i");
}

MU_TEST(test_mixed_case_hex_3)
{
    test("FaDe", "% 010i");
}

MU_TEST_SUITE(no_format)
{
    MU_RUN_TEST(test_1);
    MU_RUN_TEST(test_2);
    MU_RUN_TEST(test_3);
    MU_RUN_TEST(test_4);
    MU_RUN_TEST(test_5);
    MU_RUN_TEST(test_zero_no_format);
    MU_RUN_TEST(test_positive_small);
    MU_RUN_TEST(test_positive_max);
    MU_RUN_TEST(test_negative_small);
    MU_RUN_TEST(test_negative_min);
}

MU_TEST_SUITE(width_only)
{
    MU_RUN_TEST(test_6);
    MU_RUN_TEST(test_7);
    MU_RUN_TEST(test_8);
    MU_RUN_TEST(test_width_5_positive);
    MU_RUN_TEST(test_width_5_negative);
    MU_RUN_TEST(test_width_10_positive);
    MU_RUN_TEST(test_width_10_negative);
    MU_RUN_TEST(test_width_shorter_than_number);
}

MU_TEST_SUITE(left_align_flag)
{
    MU_RUN_TEST(test_left_align_no_width);
    MU_RUN_TEST(test_left_align_width_positive);
    MU_RUN_TEST(test_left_align_width_negative);
    MU_RUN_TEST(test_left_align_width_large);
}

MU_TEST_SUITE(plus_flag)
{
    MU_RUN_TEST(test_plus_no_width_positive);
    MU_RUN_TEST(test_plus_no_width_negative);
    MU_RUN_TEST(test_plus_width_positive);
    MU_RUN_TEST(test_plus_width_negative);
    MU_RUN_TEST(test_plus_zero);
}

MU_TEST_SUITE(space_flag)
{
    MU_RUN_TEST(test_space_no_width_positive);
    MU_RUN_TEST(test_space_no_width_negative);
    MU_RUN_TEST(test_space_width_positive);
    MU_RUN_TEST(test_space_width_negative);
    MU_RUN_TEST(test_space_zero);
}

MU_TEST_SUITE(zero_flag)
{
    MU_RUN_TEST(test_zero_flag_width_positive);
    MU_RUN_TEST(test_zero_flag_width_negative);
    MU_RUN_TEST(test_zero_flag_width_large);
    MU_RUN_TEST(test_zero_flag_no_width);
}

MU_TEST_SUITE(flag_combinations)
{
    MU_RUN_TEST(test_left_align_plus_positive);
    MU_RUN_TEST(test_left_align_plus_negative);
    MU_RUN_TEST(test_left_align_space_positive);
    MU_RUN_TEST(test_left_align_space_negative);
    MU_RUN_TEST(test_plus_zero_flag_positive);
    MU_RUN_TEST(test_plus_zero_flag_negative);
    MU_RUN_TEST(test_space_zero_flag_positive);
    MU_RUN_TEST(test_space_zero_flag_negative);
    MU_RUN_TEST(test_left_align_plus_zero_positive);
    MU_RUN_TEST(test_left_align_plus_zero_negative);
    MU_RUN_TEST(test_left_align_space_zero_positive);
    MU_RUN_TEST(test_left_align_space_zero_negative);
    MU_RUN_TEST(test_plus_space_conflict);
    MU_RUN_TEST(test_all_flags_combined);
}

MU_TEST_SUITE(edge_cases)
{
    MU_RUN_TEST(test_width_1_positive);
    MU_RUN_TEST(test_width_1_negative);
    MU_RUN_TEST(test_boundary_positive_max_minus_1);
    MU_RUN_TEST(test_boundary_negative_min_plus_1);
    MU_RUN_TEST(test_zero_with_all_formats);
    MU_RUN_TEST(test_minus_one);
    MU_RUN_TEST(test_one);
    MU_RUN_TEST(test_int32_max);
    MU_RUN_TEST(test_int32_min);
    MU_RUN_TEST(test_width_zero);
    MU_RUN_TEST(test_width_exact_match_positive);
    MU_RUN_TEST(test_width_exact_match_negative);
    MU_RUN_TEST(test_width_exact_match_with_plus);
    MU_RUN_TEST(test_width_exact_match_with_space);
    MU_RUN_TEST(test_very_large_width);
    MU_RUN_TEST(test_negative_with_width_exact);
    MU_RUN_TEST(test_negative_with_width_plus_one);
}

MU_TEST_SUITE(boundary_values_int32)
{
    MU_RUN_TEST(test_int32_max_with_plus);
    MU_RUN_TEST(test_int32_min_with_plus);
    MU_RUN_TEST(test_int32_max_with_space);
    MU_RUN_TEST(test_int32_min_with_space);
    MU_RUN_TEST(test_int32_max_with_width);
    MU_RUN_TEST(test_int32_min_with_width);
    MU_RUN_TEST(test_int32_max_with_zero_flag);
    MU_RUN_TEST(test_int32_min_with_zero_flag);
    MU_RUN_TEST(test_int32_max_left_align);
    MU_RUN_TEST(test_int32_min_left_align);
    MU_RUN_TEST(test_max_digits_no_sign);
    MU_RUN_TEST(test_max_digits_with_sign);
    MU_RUN_TEST(test_min_digits_no_extra_sign);
}

MU_TEST_SUITE(digit_variations)
{
    MU_RUN_TEST(test_single_digit_all_flags);
    MU_RUN_TEST(test_two_digits_all_flags);
    MU_RUN_TEST(test_three_digits_all_flags);
    MU_RUN_TEST(test_100_no_format);
    MU_RUN_TEST(test_1000_no_format);
    MU_RUN_TEST(test_10000_no_format);
    MU_RUN_TEST(test_100000_no_format);
    MU_RUN_TEST(test_1000000_no_format);
    MU_RUN_TEST(test_negative_100);
    MU_RUN_TEST(test_negative_1000);
    MU_RUN_TEST(test_negative_10000);
}

MU_TEST_SUITE(all_flag_combinations_no_width)
{
    MU_RUN_TEST(test_no_flags_no_width);
    MU_RUN_TEST(test_only_minus_no_width);
    MU_RUN_TEST(test_only_plus_no_width);
    MU_RUN_TEST(test_only_space_no_width);
    MU_RUN_TEST(test_only_zero_no_width);
    MU_RUN_TEST(test_minus_plus_no_width);
    MU_RUN_TEST(test_minus_space_no_width);
    MU_RUN_TEST(test_minus_zero_no_width);
    MU_RUN_TEST(test_plus_zero_no_width);
    MU_RUN_TEST(test_space_zero_no_width);
    MU_RUN_TEST(test_plus_space_no_width);
    MU_RUN_TEST(test_minus_plus_space_no_width);
    MU_RUN_TEST(test_minus_plus_zero_no_width);
    MU_RUN_TEST(test_minus_space_zero_no_width);
    MU_RUN_TEST(test_plus_space_zero_no_width);
    MU_RUN_TEST(test_all_four_flags_no_width);
}

MU_TEST_SUITE(small_widths)
{
    MU_RUN_TEST(test_zero_flag_width_1);
    MU_RUN_TEST(test_zero_flag_width_2);
    MU_RUN_TEST(test_plus_width_1_positive);
    MU_RUN_TEST(test_plus_width_2_positive);
    MU_RUN_TEST(test_plus_width_3_positive);
    MU_RUN_TEST(test_space_width_1_positive);
    MU_RUN_TEST(test_space_width_2_positive);
    MU_RUN_TEST(test_space_width_3_positive);
}

MU_TEST_SUITE(zero_special_cases)
{
    MU_RUN_TEST(test_zero_only_plus);
    MU_RUN_TEST(test_zero_only_space);
    MU_RUN_TEST(test_zero_only_zero_flag);
    MU_RUN_TEST(test_zero_only_minus);
    MU_RUN_TEST(test_zero_plus_width);
    MU_RUN_TEST(test_zero_space_width);
    MU_RUN_TEST(test_zero_zero_flag_width);
    MU_RUN_TEST(test_zero_minus_width);
    MU_RUN_TEST(test_zero_plus_zero_flag_width);
    MU_RUN_TEST(test_zero_space_zero_flag_width);
    MU_RUN_TEST(test_zero_minus_plus_width);
    MU_RUN_TEST(test_zero_minus_space_width);
}

MU_TEST_SUITE(one_and_minus_one)
{
    MU_RUN_TEST(test_minus_one_all_formats);
    MU_RUN_TEST(test_one_all_formats);
}

MU_TEST_SUITE(powers_of_two)
{
    MU_RUN_TEST(test_powers_of_two_1);
    MU_RUN_TEST(test_powers_of_two_2);
    MU_RUN_TEST(test_powers_of_two_4);
    MU_RUN_TEST(test_powers_of_two_8);
    MU_RUN_TEST(test_powers_of_two_16);
    MU_RUN_TEST(test_powers_of_two_256);
    MU_RUN_TEST(test_powers_of_two_1024);
    MU_RUN_TEST(test_powers_of_two_65536);
}

MU_TEST_SUITE(combo_variations_positive)
{
    MU_RUN_TEST(test_combo_minus_plus_width_5);
    MU_RUN_TEST(test_combo_minus_plus_width_10);
    MU_RUN_TEST(test_combo_minus_plus_width_15);
    MU_RUN_TEST(test_combo_minus_space_width_5);
    MU_RUN_TEST(test_combo_minus_space_width_10);
    MU_RUN_TEST(test_combo_plus_zero_width_5);
    MU_RUN_TEST(test_combo_plus_zero_width_10);
    MU_RUN_TEST(test_combo_plus_zero_width_15);
    MU_RUN_TEST(test_combo_space_zero_width_5);
    MU_RUN_TEST(test_combo_space_zero_width_10);
    MU_RUN_TEST(test_combo_all_flags_width_5);
    MU_RUN_TEST(test_combo_all_flags_width_10);
    MU_RUN_TEST(test_combo_all_flags_width_20);
}

MU_TEST_SUITE(combo_variations_negative)
{
    MU_RUN_TEST(test_negative_combo_minus_plus_width_5);
    MU_RUN_TEST(test_negative_combo_minus_space_width_5);
    MU_RUN_TEST(test_negative_combo_plus_zero_width_5);
    MU_RUN_TEST(test_negative_combo_space_zero_width_5);
    MU_RUN_TEST(test_negative_combo_all_flags_width_5);
}

MU_TEST_SUITE(large_widths)
{
    MU_RUN_TEST(test_width_20_positive);
    MU_RUN_TEST(test_width_20_negative);
    MU_RUN_TEST(test_width_20_plus_positive);
    MU_RUN_TEST(test_width_20_space_positive);
    MU_RUN_TEST(test_width_20_zero_positive);
    MU_RUN_TEST(test_width_20_minus_positive);
}

MU_TEST_SUITE(complete_flag_combinations_positive)
{
    MU_RUN_TEST(test_all_combos_width_no_flags);
    MU_RUN_TEST(test_all_combos_width_minus);
    MU_RUN_TEST(test_all_combos_width_plus);
    MU_RUN_TEST(test_all_combos_width_space);
    MU_RUN_TEST(test_all_combos_width_zero);
    MU_RUN_TEST(test_all_combos_width_minus_plus);
    MU_RUN_TEST(test_all_combos_width_minus_space);
    MU_RUN_TEST(test_all_combos_width_minus_zero);
    MU_RUN_TEST(test_all_combos_width_plus_zero);
    MU_RUN_TEST(test_all_combos_width_space_zero);
    MU_RUN_TEST(test_all_combos_width_plus_space);
    MU_RUN_TEST(test_all_combos_width_minus_plus_space);
    MU_RUN_TEST(test_all_combos_width_minus_plus_zero);
    MU_RUN_TEST(test_all_combos_width_minus_space_zero);
    MU_RUN_TEST(test_all_combos_width_plus_space_zero);
    MU_RUN_TEST(test_all_combos_width_all_four);
}

MU_TEST_SUITE(complete_flag_combinations_negative)
{
    MU_RUN_TEST(test_all_combos_neg_no_flags);
    MU_RUN_TEST(test_all_combos_neg_minus);
    MU_RUN_TEST(test_all_combos_neg_plus);
    MU_RUN_TEST(test_all_combos_neg_space);
    MU_RUN_TEST(test_all_combos_neg_zero);
    MU_RUN_TEST(test_all_combos_neg_minus_plus);
    MU_RUN_TEST(test_all_combos_neg_minus_space);
    MU_RUN_TEST(test_all_combos_neg_minus_zero);
    MU_RUN_TEST(test_all_combos_neg_plus_zero);
    MU_RUN_TEST(test_all_combos_neg_space_zero);
    MU_RUN_TEST(test_all_combos_neg_plus_space);
    MU_RUN_TEST(test_all_combos_neg_minus_plus_space);
    MU_RUN_TEST(test_all_combos_neg_minus_plus_zero);
    MU_RUN_TEST(test_all_combos_neg_minus_space_zero);
    MU_RUN_TEST(test_all_combos_neg_plus_space_zero);
    MU_RUN_TEST(test_all_combos_neg_all_four);
}

MU_TEST_SUITE(special_combinations)
{
    MU_RUN_TEST(test_zero_all_combos_minus_plus);
    MU_RUN_TEST(test_zero_all_combos_plus_space_zero);
    MU_RUN_TEST(test_zero_all_combos_all_four);
    MU_RUN_TEST(test_max_all_combos_plus_zero);
    MU_RUN_TEST(test_max_all_combos_minus_plus);
    MU_RUN_TEST(test_max_all_combos_all_four);
    MU_RUN_TEST(test_min_all_combos_plus_zero);
    MU_RUN_TEST(test_min_all_combos_minus_plus);
    MU_RUN_TEST(test_min_all_combos_space_zero);
}

MU_TEST_SUITE(flag_order_variations)
{
    MU_RUN_TEST(test_reversed_order_space_plus);
    MU_RUN_TEST(test_reversed_order_zero_plus);
    MU_RUN_TEST(test_reversed_order_zero_minus);
    MU_RUN_TEST(test_reversed_order_space_plus_minus);
}

MU_TEST_SUITE(exact_and_shorter_widths)
{
    MU_RUN_TEST(test_exact_width_no_flags);
    MU_RUN_TEST(test_exact_width_plus);
    MU_RUN_TEST(test_exact_width_space);
    MU_RUN_TEST(test_exact_width_neg_no_flags);
    MU_RUN_TEST(test_exact_width_neg_plus);
    MU_RUN_TEST(test_shorter_width_plus);
    MU_RUN_TEST(test_shorter_width_space);
    MU_RUN_TEST(test_shorter_width_zero);
}

MU_TEST_SUITE(multi_digit_combinations)
{
    MU_RUN_TEST(test_multiple_digits_all_combos_1);
    MU_RUN_TEST(test_multiple_digits_all_combos_2);
    MU_RUN_TEST(test_multiple_digits_all_combos_3);
}

MU_TEST_SUITE(negative_via_minus_sign)
{
    MU_RUN_TEST(test_negative_via_minus_sign_1);
    MU_RUN_TEST(test_negative_via_minus_sign_2);
    MU_RUN_TEST(test_negative_via_minus_sign_3);
    MU_RUN_TEST(test_negative_via_minus_sign_large);
}

MU_TEST_SUITE(negative_via_high_bit)
{
    MU_RUN_TEST(test_negative_via_high_bit_1);
    MU_RUN_TEST(test_negative_via_high_bit_2);
    MU_RUN_TEST(test_negative_via_high_bit_3);
    MU_RUN_TEST(test_negative_via_high_bit_small);
}

MU_TEST_SUITE(double_negation)
{
    MU_RUN_TEST(test_double_negation_1);
    MU_RUN_TEST(test_double_negation_2);
    MU_RUN_TEST(test_double_negation_3);
    MU_RUN_TEST(test_double_negation_with_format);
}

MU_TEST_SUITE(varied_numbers_positive)
{
    MU_RUN_TEST(test_varied_positive_1);
    MU_RUN_TEST(test_varied_positive_2);
    MU_RUN_TEST(test_varied_positive_3);
    MU_RUN_TEST(test_varied_positive_4);
    MU_RUN_TEST(test_varied_positive_5);
}

MU_TEST_SUITE(varied_numbers_negative)
{
    MU_RUN_TEST(test_varied_negative_via_minus_1);
    MU_RUN_TEST(test_varied_negative_via_minus_2);
    MU_RUN_TEST(test_varied_negative_via_minus_3);
    MU_RUN_TEST(test_varied_negative_via_high_bit_1);
    MU_RUN_TEST(test_varied_negative_via_high_bit_2);
    MU_RUN_TEST(test_varied_negative_via_high_bit_3);
}

MU_TEST_SUITE(odd_length_hex_strings)
{
    MU_RUN_TEST(test_odd_length_positive_1);
    MU_RUN_TEST(test_odd_length_positive_2);
    MU_RUN_TEST(test_odd_length_positive_3);
    MU_RUN_TEST(test_odd_length_negative_minus_1);
    MU_RUN_TEST(test_odd_length_negative_minus_2);
    MU_RUN_TEST(test_odd_length_negative_minus_3);
    MU_RUN_TEST(test_odd_length_negative_high_bit_1);
    MU_RUN_TEST(test_odd_length_negative_high_bit_2);
    MU_RUN_TEST(test_odd_length_double_negation_1);
    MU_RUN_TEST(test_odd_length_double_negation_2);
}

MU_TEST_SUITE(edge_around_zero)
{
    MU_RUN_TEST(test_edge_around_zero_1);
    MU_RUN_TEST(test_edge_around_zero_2);
    MU_RUN_TEST(test_edge_around_zero_3);
    MU_RUN_TEST(test_edge_around_zero_4);
    MU_RUN_TEST(test_powers_positive_varied);
    MU_RUN_TEST(test_powers_negative_minus);
    MU_RUN_TEST(test_powers_negative_high_bit);
}

MU_TEST_SUITE(boundary_variations)
{
    MU_RUN_TEST(test_max_positive_variations_1);
    MU_RUN_TEST(test_max_positive_variations_2);
    MU_RUN_TEST(test_max_positive_variations_3);
    MU_RUN_TEST(test_min_negative_variations_1);
    MU_RUN_TEST(test_min_negative_variations_2);
    MU_RUN_TEST(test_min_negative_variations_3);
    MU_RUN_TEST(test_boundary_near_max_1);
    MU_RUN_TEST(test_boundary_near_max_2);
    MU_RUN_TEST(test_boundary_near_min_1);
    MU_RUN_TEST(test_boundary_near_min_2);
}

MU_TEST_SUITE(combo_all_negation_types)
{
    MU_RUN_TEST(test_combo_varied_1);
    MU_RUN_TEST(test_combo_varied_2);
    MU_RUN_TEST(test_combo_varied_3);
    MU_RUN_TEST(test_combo_varied_4);
}

MU_TEST_SUITE(small_varied_numbers)
{
    MU_RUN_TEST(test_small_numbers_varied_1);
    MU_RUN_TEST(test_small_numbers_varied_2);
    MU_RUN_TEST(test_small_numbers_varied_3);
    MU_RUN_TEST(test_small_numbers_varied_4);
}

MU_TEST_SUITE(three_digit_tests)
{
    MU_RUN_TEST(test_three_digit_variations_1);
    MU_RUN_TEST(test_three_digit_variations_2);
    MU_RUN_TEST(test_three_digit_variations_3);
}

MU_TEST_SUITE(hex_digit_coverage)
{
    MU_RUN_TEST(test_all_hex_digits_A);
    MU_RUN_TEST(test_all_hex_digits_B);
    MU_RUN_TEST(test_all_hex_digits_C);
    MU_RUN_TEST(test_all_hex_digits_D);
    MU_RUN_TEST(test_all_hex_digits_E);
    MU_RUN_TEST(test_all_hex_digits_F);
}

MU_TEST_SUITE(mixed_case_hex)
{
    MU_RUN_TEST(test_mixed_case_hex_1);
    MU_RUN_TEST(test_mixed_case_hex_2);
    MU_RUN_TEST(test_mixed_case_hex_3);
}

int main(void)
{
    MU_RUN_SUITE(no_format);
    MU_RUN_SUITE(width_only);
    MU_RUN_SUITE(left_align_flag);
    MU_RUN_SUITE(plus_flag);
    MU_RUN_SUITE(space_flag);
    MU_RUN_SUITE(zero_flag);
    MU_RUN_SUITE(flag_combinations);
    MU_RUN_SUITE(edge_cases);
    MU_RUN_SUITE(boundary_values_int32);
    MU_RUN_SUITE(digit_variations);
    MU_RUN_SUITE(all_flag_combinations_no_width);
    MU_RUN_SUITE(small_widths);
    MU_RUN_SUITE(zero_special_cases);
    MU_RUN_SUITE(one_and_minus_one);
    MU_RUN_SUITE(powers_of_two);
    MU_RUN_SUITE(combo_variations_positive);
    MU_RUN_SUITE(combo_variations_negative);
    MU_RUN_SUITE(large_widths);
    MU_RUN_SUITE(complete_flag_combinations_positive);
    MU_RUN_SUITE(complete_flag_combinations_negative);
    MU_RUN_SUITE(special_combinations);
    MU_RUN_SUITE(flag_order_variations);
    MU_RUN_SUITE(exact_and_shorter_widths);
    MU_RUN_SUITE(multi_digit_combinations);
    MU_RUN_SUITE(negative_via_minus_sign);
    MU_RUN_SUITE(negative_via_high_bit);
    MU_RUN_SUITE(double_negation);
    MU_RUN_SUITE(varied_numbers_positive);
    MU_RUN_SUITE(varied_numbers_negative);
    MU_RUN_SUITE(odd_length_hex_strings);
    MU_RUN_SUITE(edge_around_zero);
    MU_RUN_SUITE(boundary_variations);
    MU_RUN_SUITE(combo_all_negation_types);
    MU_RUN_SUITE(small_varied_numbers);
    MU_RUN_SUITE(three_digit_tests);
    MU_RUN_SUITE(hex_digit_coverage);
    MU_RUN_SUITE(mixed_case_hex);
    MU_REPORT();
    return MU_EXIT_CODE;
}
