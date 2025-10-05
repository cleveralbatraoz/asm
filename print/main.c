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

static void test_explicit(const char *input, const char *format, const char *expected_result)
{
    char actual[128];
    print(actual, format, input);
    mu_assert_string_eq(expected_result, actual);
}

MU_TEST(test_1)
{
    test("DEAD", "%i");
}

MU_TEST(test_128bit_large_positive)
{
    test_explicit("123456789ABCDEF0123456789ABCDEF0", "%i", "24197857203266734864793317670504947440");
}

MU_TEST(test_128bit_negative_via_high_bit)
{
    test_explicit("FFFFFFFFFFFFFFFF0000000000000000", "%+i", "-18446744073709551616");
}

MU_TEST(test_128bit_explicit_minus)
{
    test_explicit("-1000000000000000", "% i", "-1152921504606846976");
}

MU_TEST(test_128bit_with_formatting)
{
    test_explicit("FEDCBA9876543210FEDCBA9876543210", "%+50i", "            -1512366075204170929049582354406559216");
}

MU_TEST(test_128bit_max_value)
{
    test_explicit("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "%i", "170141183460469231731687303715884105727");
}

MU_TEST(test_128bit_min_value)
{
    test_explicit("80000000000000000000000000000000", "%+i", "-170141183460469231731687303715884105728");
}

MU_TEST(test_128bit_explicit_minus_large)
{
    test_explicit("-123456789ABCDEF0123456789ABCDEF0", "%i", "-24197857203266734864793317670504947440");
}

MU_TEST(test_128bit_double_negation_full)
{
    test_explicit("-FEDCBA9876543210FEDCBA9876543210", "%+i", "+1512366075204170929049582354406559216");
}

MU_TEST(test_2)
{
    test("BEEF", "%i");
}

MU_TEST(test_3)
{
    test_explicit("DEADBEEF", "%i", "3735928559");
}

MU_TEST(test_4)
{
    test_explicit("BEEFDEAD", "%i", "3203391149");
}

MU_TEST(test_5)
{
    test_explicit("FFFFFFFF", "%i", "4294967295");
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
    test_explicit("FFFFFFFF", "%i", "4294967295");
}

MU_TEST(test_negative_min)
{
    test_explicit("80000000", "%i", "2147483648");
}

MU_TEST(test_width_5_positive)
{
    test("7B", "%5i");
}

MU_TEST(test_width_5_negative)
{
    test_explicit("FFFFFF85", "%5i", "4294967173");
}

MU_TEST(test_width_10_positive)
{
    test("DEAD", "%10i");
}

MU_TEST(test_width_10_negative)
{
    test_explicit("FFFF0000", "%10i", "4294901760");
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
    test_explicit("FFFFFFD6", "%-8i", "4294967254");
}

MU_TEST(test_left_align_width_large)
{
    test_explicit("DEADBEEF", "%-15i", "3735928559     ");
}

MU_TEST(test_plus_no_width_positive)
{
    test("42", "%+i");
}

MU_TEST(test_plus_no_width_negative)
{
    test_explicit("FFFFFFD6", "%+i", "+4294967254");
}

MU_TEST(test_plus_width_positive)
{
    test("7B", "%+8i");
}

MU_TEST(test_plus_width_negative)
{
    test_explicit("FFFFFF85", "%+8i", "+4294967173");
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
    test_explicit("FFFFFFD6", "% i", " 4294967254");
}

MU_TEST(test_space_width_positive)
{
    test("7B", "% 8i");
}

MU_TEST(test_space_width_negative)
{
    test_explicit("FFFFFF85", "% 8i", " 4294967173");
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
    test_explicit("FFFFFF85", "%08i", "4294967173");
}

MU_TEST(test_zero_flag_width_large)
{
    test_explicit("DEADBEEF", "%015i", "000003735928559");
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
    test_explicit("FFFFFF85", "%-+10i", "+4294967173");
}

MU_TEST(test_left_align_space_positive)
{
    test("7B", "%- 10i");
}

MU_TEST(test_left_align_space_negative)
{
    test_explicit("FFFFFF85", "%- 10i", " 4294967173");
}

MU_TEST(test_plus_zero_flag_positive)
{
    test("7B", "%+08i");
}

MU_TEST(test_plus_zero_flag_negative)
{
    test_explicit("FFFFFF85", "%+08i", "+4294967173");
}

MU_TEST(test_space_zero_flag_positive)
{
    test("7B", "% 08i");
}

MU_TEST(test_space_zero_flag_negative)
{
    test_explicit("FFFFFF85", "% 08i", " 4294967173");
}

MU_TEST(test_left_align_plus_zero_positive)
{
    test("7B", "%-+08i");
}

MU_TEST(test_left_align_plus_zero_negative)
{
    test_explicit("FFFFFF85", "%-+08i", "+4294967173");
}

MU_TEST(test_left_align_space_zero_positive)
{
    test("7B", "%- 08i");
}

MU_TEST(test_left_align_space_zero_negative)
{
    test_explicit("FFFFFF85", "%- 08i", " 4294967173");
}

MU_TEST(test_plus_space_conflict)
{
    test("42", "% +8i");
}

MU_TEST(test_all_flags_combined)
{
    test_explicit("DEADBEEF", "%-+ 015i", "+3735928559    ");
}

MU_TEST(test_width_1_positive)
{
    test("5", "%1i");
}

MU_TEST(test_width_1_negative)
{
    test_explicit("FFFFFFFB", "%1i", "4294967291");
}

MU_TEST(test_boundary_positive_max_minus_1)
{
    test("7FFFFFFE", "%+012i");
}

MU_TEST(test_boundary_negative_min_plus_1)
{
    test_explicit("80000001", "%+012i", "+02147483649");
}

MU_TEST(test_zero_with_all_formats)
{
    test("0", "%-+010i");
}

MU_TEST(test_minus_one)
{
    test_explicit("FFFFFFFF", "%i", "4294967295");
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
    test_explicit("80000000", "%i", "2147483648");
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
    test_explicit("FFFFFF85", "%4i", "4294967173");
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
    test_explicit("80000000", "%+i", "+2147483648");
}

MU_TEST(test_int32_max_with_space)
{
    test("7FFFFFFF", "% i");
}

MU_TEST(test_int32_min_with_space)
{
    test_explicit("80000000", "% i", " 2147483648");
}

MU_TEST(test_int32_max_with_width)
{
    test("7FFFFFFF", "%15i");
}

MU_TEST(test_int32_min_with_width)
{
    test_explicit("80000000", "%15i", "     2147483648");
}

MU_TEST(test_int32_max_with_zero_flag)
{
    test("7FFFFFFF", "%015i");
}

MU_TEST(test_int32_min_with_zero_flag)
{
    test_explicit("80000000", "%015i", "000002147483648");
}

MU_TEST(test_int32_max_left_align)
{
    test("7FFFFFFF", "%-15i");
}

MU_TEST(test_int32_min_left_align)
{
    test_explicit("80000000", "%-15i", "2147483648     ");
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
    test_explicit("FFFFFFFB", "%2i", "4294967291");
}

MU_TEST(test_negative_with_width_plus_one)
{
    test_explicit("FFFFFFFB", "%3i", "4294967291");
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
    test_explicit("FFFFFFFF", "%-+012i", "+4294967295 ");
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
    test_explicit("FFFFFF9C", "%i", "4294967196");
}

MU_TEST(test_negative_1000)
{
    test_explicit("FFFFFC18", "%i", "4294966296");
}

MU_TEST(test_negative_10000)
{
    test_explicit("FFFFD8F0", "%i", "4294957296");
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
    test_explicit("FFFFFFD6", "%-+5i", "+4294967254");
}

MU_TEST(test_negative_combo_minus_space_width_5)
{
    test_explicit("FFFFFFD6", "%- 5i", " 4294967254");
}

MU_TEST(test_negative_combo_plus_zero_width_5)
{
    test_explicit("FFFFFFD6", "%+05i", "+4294967254");
}

MU_TEST(test_negative_combo_space_zero_width_5)
{
    test_explicit("FFFFFFD6", "% 05i", " 4294967254");
}

MU_TEST(test_negative_combo_all_flags_width_5)
{
    test_explicit("FFFFFFD6", "%-+ 05i", "+4294967254");
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
    test_explicit("80000000", "%11i", " 2147483648");
}

MU_TEST(test_width_20_positive)
{
    test("2A", "%20i");
}

MU_TEST(test_width_20_negative)
{
    test_explicit("FFFFFFD6", "%20i", "          4294967254");
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
    test_explicit("FFFFFFD6", "%8i", "4294967254");
}

MU_TEST(test_all_combos_neg_minus)
{
    test_explicit("FFFFFFD6", "%-8i", "4294967254");
}

MU_TEST(test_all_combos_neg_plus)
{
    test_explicit("FFFFFFD6", "%+8i", "+4294967254");
}

MU_TEST(test_all_combos_neg_space)
{
    test_explicit("FFFFFFD6", "% 8i", " 4294967254");
}

MU_TEST(test_all_combos_neg_zero)
{
    test_explicit("FFFFFFD6", "%08i", "4294967254");
}

MU_TEST(test_all_combos_neg_minus_plus)
{
    test_explicit("FFFFFFD6", "%-+8i", "+4294967254");
}

MU_TEST(test_all_combos_neg_minus_space)
{
    test_explicit("FFFFFFD6", "%- 8i", " 4294967254");
}

MU_TEST(test_all_combos_neg_minus_zero)
{
    test_explicit("FFFFFFD6", "%-08i", "4294967254");
}

MU_TEST(test_all_combos_neg_plus_zero)
{
    test_explicit("FFFFFFD6", "%+08i", "+4294967254");
}

MU_TEST(test_all_combos_neg_space_zero)
{
    test_explicit("FFFFFFD6", "% 08i", " 4294967254");
}

MU_TEST(test_all_combos_neg_plus_space)
{
    test_explicit("FFFFFFD6", "%+ 8i", "+4294967254");
}

MU_TEST(test_all_combos_neg_minus_plus_space)
{
    test_explicit("FFFFFFD6", "%-+ 8i", "+4294967254");
}

MU_TEST(test_all_combos_neg_minus_plus_zero)
{
    test_explicit("FFFFFFD6", "%-+08i", "+4294967254");
}

MU_TEST(test_all_combos_neg_minus_space_zero)
{
    test_explicit("FFFFFFD6", "%- 08i", " 4294967254");
}

MU_TEST(test_all_combos_neg_plus_space_zero)
{
    test_explicit("FFFFFFD6", "%+ 08i", "+4294967254");
}

MU_TEST(test_all_combos_neg_all_four)
{
    test_explicit("FFFFFFD6", "%-+ 08i", "+4294967254");
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
    test_explicit("80000000", "%+013i", "+002147483648");
}

MU_TEST(test_min_all_combos_minus_plus)
{
    test_explicit("80000000", "%-+13i", "+2147483648  ");
}

MU_TEST(test_min_all_combos_space_zero)
{
    test_explicit("80000000", "% 013i", " 002147483648");
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
    test_explicit("FFFFFFD6", "%3i", "4294967254");
}

MU_TEST(test_exact_width_neg_plus)
{
    test_explicit("FFFFFFD6", "%+4i", "+4294967254");
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
    test_explicit("FFFFFFFE", "%i", "4294967294");
}

MU_TEST(test_negative_via_high_bit_2)
{
    test_explicit("FFFFFF00", "%+8i", "+4294967040");
}

MU_TEST(test_negative_via_high_bit_3)
{
    test_explicit("80000001", "% 012i", " 02147483649");
}

MU_TEST(test_negative_via_high_bit_small)
{
    test_explicit("FFFFFFF0", "%-8i", "4294967280");
}

MU_TEST(test_double_negation_1)
{
    test_explicit("-FFFFFFFF", "%i", "-4294967295");
}

MU_TEST(test_double_negation_2)
{
    test_explicit("-FFFFFFFE", "%+8i", "-4294967294");
}

MU_TEST(test_double_negation_3)
{
    test_explicit("-80000001", "%i", "-2147483649");
}

MU_TEST(test_double_negation_with_format)
{
    test_explicit("-80000010", "%-+012i", "-2147483664 ");
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
    test_explicit("FFFFFF38", "%010i", "4294967096");
}

MU_TEST(test_varied_negative_via_high_bit_2)
{
    test_explicit("FFFFFC18", "%-+12i", "+4294966296 ");
}

MU_TEST(test_varied_negative_via_high_bit_3)
{
    test_explicit("FFFFEC78", "% 012i", " 04294962296");
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
    test_explicit("FFFFFFF1", "%i", "4294967281");
}

MU_TEST(test_odd_length_negative_high_bit_2)
{
    test_explicit("FFFFF544", "%+08i", "+4294964548");
}

MU_TEST(test_odd_length_double_negation_1)
{
    test_explicit("-FFFFFFF1", "%i", "-4294967281");
}

MU_TEST(test_odd_length_double_negation_2)
{
    test_explicit("-FFFFF544", "%-+010i", "-4294964548");
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
    test_explicit("FFFFFFFF", "%+5i", "+4294967295");
}

MU_TEST(test_edge_around_zero_4)
{
    test_explicit("-FFFFFFFF", "%+5i", "-4294967295");
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
    test_explicit("FFFFFF80", "%+08i", "+4294967168");
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
    test_explicit("80000001", "%-+15i", "+2147483649    ");
}

MU_TEST(test_min_negative_variations_2)
{
    test_explicit("80000010", "% 015i", " 00002147483664");
}

MU_TEST(test_min_negative_variations_3)
{
    test_explicit("80000100", "%+012i", "+02147483904");
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
    test_explicit("FFFFFA24", "%-+ 010i", "+4294965796");
}

MU_TEST(test_combo_varied_4)
{
    test_explicit("-FFFFFA24", "%-+ 010i", "-4294965796");
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
    test_explicit("FFFFFFF7", "%+05i", "+4294967287");
}

MU_TEST(test_small_numbers_varied_4)
{
    test_explicit("-FFFFFFF7", "%+05i", "-4294967287");
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
    test_explicit("FFFFFE38", "%+010i", "+4294966840");
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
    test_explicit("80000011", "%+i", "+2147483665");
}

MU_TEST(test_boundary_near_min_2)
{
    test_explicit("80000012", "%-+12i", "+2147483666 ");
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
    MU_RUN_TEST(test_128bit_large_positive);
    MU_RUN_TEST(test_128bit_negative_via_high_bit);
    MU_RUN_TEST(test_128bit_explicit_minus);
    MU_RUN_TEST(test_128bit_with_formatting);
    MU_RUN_TEST(test_128bit_max_value);
    MU_RUN_TEST(test_128bit_min_value);
    MU_RUN_TEST(test_128bit_explicit_minus_large);
    MU_RUN_TEST(test_128bit_double_negation_full);
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
MU_TEST(test_128bit_max_positive)
{
    test_explicit("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "%i", "170141183460469231731687303715884105727");
}

MU_TEST(test_128bit_max_negative)
{
    test_explicit("80000000000000000000000000000000", "%i", "-170141183460469231731687303715884105728");
}

MU_TEST(test_128bit_near_max_positive)
{
    test_explicit("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE", "%i", "170141183460469231731687303715884105726");
}

MU_TEST(test_128bit_near_max_negative)
{
    test_explicit("80000000000000000000000000000001", "%i", "-170141183460469231731687303715884105727");
}

MU_TEST(test_128bit_power_of_10)
{
    test_explicit("4B3B4CA85A86C47A098A224000000000", "%i", "100000000000000000000000000000000000000");
}

MU_TEST(test_128bit_googol_minus_1)
{
    test_explicit("4B3B4CA85A86C47A098A223FFFFFFFFF", "%i", "99999999999999999999999999999999999999");
}

MU_TEST(test_128bit_large_prime)
{
    test_explicit("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF43", "%i", "-189");
}

MU_TEST(test_128bit_alternating_bytes)
{
    test_explicit("AA55AA55AA55AA55AA55AA55AA55AA55", "%i", "-113868807607784855478016405599735666091");
}

MU_TEST(test_128bit_all_F_except_low)
{
    test_explicit("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00", "%i", "-256");
}

MU_TEST(test_128bit_power_of_2_100)
{
    test_explicit("10000000000000000000000000", "%i", "1267650600228229401496703205376");
}

MU_TEST(test_128bit_power_of_2_120)
{
    test_explicit("1000000000000000000000000000000", "%i", "1329227995784915872903807060280344576");
}

MU_TEST(test_128bit_with_width)
{
    test_explicit("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "%50d", "           170141183460469231731687303715884105727");
}

MU_TEST(test_128bit_with_plus)
{
    test_explicit("DEADBEEFCAFEBABE", "%+d", "+16045690984503098046");
}

MU_TEST(test_128bit_with_zero_pad)
{
    test_explicit("ABCDEF123456789", "%050d", "00000000000000000000000000000000773738363261118345");
}

MU_TEST(test_128bit_with_left_align)
{
    test_explicit("123456789ABCDEF", "%-50d", "81985529216486895                                 ");
}

MU_TEST(test_128bit_negative_with_flags)
{
    test_explicit("80000000000000000000000000000001", "%+50d", "          -170141183460469231731687303715884105727");
}

MU_TEST(test_128bit_negative_with_zero_pad)
{
    test_explicit("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "%050d", "-0000000000000000000000000000000000000000000000001");
}

MU_TEST(test_128bit_large_decimal)
{
    test_explicit("18EE90FF6C373E0EE4E3F0AD2", "%i", "123456789012345678901234567890");
}

MU_TEST(test_128bit_fibonacci_related)
{
    test_explicit("E1A6B8D5A0F3481B0C2B404165", "%i", "17877934528523698526040426037605");
}

MU_TEST(test_128bit_mersenne_prime_exp)
{
    test_explicit("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "% d", " 170141183460469231731687303715884105727");
}

MU_TEST(test_128bit_negative_space_flag)
{
    test_explicit("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE", "% d", "-2");
}

MU_TEST(test_128bit_very_large_width)
{
    test_explicit(
        "FF", "%100d",
        "                                                                                                 255");
}

MU_TEST(test_128bit_exact_max_decimal_digits)
{
    test_explicit("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "%i", "170141183460469231731687303715884105727");
}

MU_TEST(test_128bit_high_byte_patterns)
{
    test_explicit("7F000000000000000000000000000000", "%i", "168811955464684315858783496655603761152");
}

MU_TEST(test_128bit_low_byte_patterns)
{
    test_explicit("FF", "%i", "255");
}

MU_TEST(test_128bit_middle_bytes)
{
    test_explicit("FFFFFFFFFFFFFFFF0000000000000000", "%i", "-18446744073709551616");
}

MU_TEST(test_128bit_explicit_minus_very_large)
{
    test_explicit("-7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "%i", "-170141183460469231731687303715884105727");
}

MU_TEST(test_128bit_factorial_related)
{
    test_explicit("E5DCEB1B53C189F54F400000000000", "%i", "1193516737792819816361966985503834112");
}

MU_TEST(test_128bit_combo_left_plus)
{
    test_explicit("FEDCBA9876543210", "%-+60d", "+18364758544493064720                                       ");
}

MU_TEST(test_128bit_combo_zero_plus)
{
    test_explicit("ABCD", "%+60d", "                                                      +43981");
}

MU_TEST(test_128bit_all_nines)
{
    test_explicit("4B3B4CA85A86C47A098A223FFFFFFFFF", "%i", "99999999999999999999999999999999999999");
}

MU_TEST(test_128bit_power_sequence)
{
    test_explicit("10000000000000000", "%i", "18446744073709551616");
}

MU_TEST(test_128bit_power_sequence_2)
{
    test_explicit("1000000000000000000000000", "%i", "79228162514264337593543950336");
}

MU_TEST(test_128bit_all_f_lowercase)
{
    test_explicit("ffffffffffffffffffffffffffffffff", "%i", "-1");
}

MU_TEST(test_128bit_explicit_minus_all_f)
{
    test_explicit("-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "%i", "1");
}

MU_TEST(test_128bit_explicit_minus_all_f_with_plus)
{
    test_explicit("-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "%+i", "+1");
}

MU_TEST_SUITE(bit_128_extremes)
{
    MU_RUN_TEST(test_128bit_max_positive);
    MU_RUN_TEST(test_128bit_max_negative);
    MU_RUN_TEST(test_128bit_near_max_positive);
    MU_RUN_TEST(test_128bit_near_max_negative);
}

MU_TEST_SUITE(bit_128_powers)
{
    MU_RUN_TEST(test_128bit_power_of_10);
    MU_RUN_TEST(test_128bit_googol_minus_1);
    MU_RUN_TEST(test_128bit_power_of_2_100);
    MU_RUN_TEST(test_128bit_power_of_2_120);
    MU_RUN_TEST(test_128bit_power_sequence);
    MU_RUN_TEST(test_128bit_power_sequence_2);
}

MU_TEST_SUITE(bit_128_special_numbers)
{
    MU_RUN_TEST(test_128bit_large_prime);
    MU_RUN_TEST(test_128bit_fibonacci_related);
    MU_RUN_TEST(test_128bit_factorial_related);
}

MU_TEST_SUITE(bit_128_patterns)
{
    MU_RUN_TEST(test_128bit_alternating_bytes);
    MU_RUN_TEST(test_128bit_all_F_except_low);
    MU_RUN_TEST(test_128bit_high_byte_patterns);
    MU_RUN_TEST(test_128bit_low_byte_patterns);
    MU_RUN_TEST(test_128bit_middle_bytes);
    MU_RUN_TEST(test_128bit_all_f_lowercase);
}

MU_TEST_SUITE(bit_128_formatting)
{
    MU_RUN_TEST(test_128bit_with_width);
    MU_RUN_TEST(test_128bit_with_plus);
    MU_RUN_TEST(test_128bit_with_zero_pad);
    MU_RUN_TEST(test_128bit_with_left_align);
    MU_RUN_TEST(test_128bit_negative_with_flags);
    MU_RUN_TEST(test_128bit_negative_with_zero_pad);
    MU_RUN_TEST(test_128bit_mersenne_prime_exp);
    MU_RUN_TEST(test_128bit_negative_space_flag);
    MU_RUN_TEST(test_128bit_very_large_width);
}

MU_TEST_SUITE(bit_128_large_numbers)
{
    MU_RUN_TEST(test_128bit_large_decimal);
    MU_RUN_TEST(test_128bit_exact_max_decimal_digits);
    MU_RUN_TEST(test_128bit_all_nines);
}

MU_TEST_SUITE(bit_128_negation)
{
    MU_RUN_TEST(test_128bit_explicit_minus_very_large);
    MU_RUN_TEST(test_128bit_explicit_minus_all_f);
    MU_RUN_TEST(test_128bit_explicit_minus_all_f_with_plus);
}

MU_TEST_SUITE(bit_128_combinations)
{
    MU_RUN_TEST(test_128bit_combo_left_plus);
    MU_RUN_TEST(test_128bit_combo_zero_plus);
}

MU_TEST(test_zero_leading_zeros_1)
{
    test_explicit("00", "%i", "0");
}

MU_TEST(test_zero_leading_zeros_2)
{
    test_explicit("000", "%i", "0");
}

MU_TEST(test_zero_leading_zeros_3)
{
    test_explicit("0000", "%i", "0");
}

MU_TEST(test_zero_leading_zeros_4)
{
    test_explicit("00000000", "%i", "0");
}

MU_TEST(test_zero_leading_zeros_many)
{
    test_explicit("00000000000000000000000000000000", "%i", "0");
}

MU_TEST(test_zero_negative_simple)
{
    test_explicit("-0", "%i", "0");
}

MU_TEST(test_zero_negative_with_zeros_1)
{
    test_explicit("-00", "%i", "0");
}

MU_TEST(test_zero_negative_with_zeros_2)
{
    test_explicit("-000", "%i", "0");
}

MU_TEST(test_zero_negative_with_zeros_3)
{
    test_explicit("-0000", "%i", "0");
}

MU_TEST(test_zero_negative_many_zeros)
{
    test_explicit("-00000000000000000000000000000000", "%i", "0");
}

MU_TEST(test_zero_with_plus_sign)
{
    test_explicit("0", "%+d", "+0");
}

MU_TEST(test_zero_leading_zeros_plus)
{
    test_explicit("0000", "%+d", "+0");
}

MU_TEST(test_zero_negative_plus_format)
{
    test_explicit("-0", "%+d", "+0");
}

MU_TEST(test_zero_negative_zeros_plus_format)
{
    test_explicit("-0000", "%+d", "+0");
}

MU_TEST(test_zero_width_padding)
{
    test_explicit("0", "%5d", "    0");
}

MU_TEST(test_zero_leading_zeros_width)
{
    test_explicit("0000", "%5d", "    0");
}

MU_TEST(test_zero_negative_width)
{
    test_explicit("-0", "%5d", "    0");
}

MU_TEST(test_zero_zero_padding)
{
    test_explicit("0", "%05d", "00000");
}

MU_TEST(test_zero_leading_zeros_zero_padding)
{
    test_explicit("0000", "%05d", "00000");
}

MU_TEST(test_zero_negative_zero_padding)
{
    test_explicit("-0", "%05d", "00000");
}

MU_TEST(test_zero_left_align)
{
    test_explicit("0", "%-5d", "0    ");
}

MU_TEST(test_zero_leading_zeros_left_align)
{
    test_explicit("0000", "%-5d", "0    ");
}

MU_TEST(test_zero_negative_left_align)
{
    test_explicit("-0", "%-5d", "0    ");
}

MU_TEST(test_zero_space_flag)
{
    test_explicit("0", "% d", " 0");
}

MU_TEST(test_zero_leading_zeros_space)
{
    test_explicit("0000", "% d", " 0");
}

MU_TEST(test_zero_negative_space)
{
    test_explicit("-0", "% d", " 0");
}

MU_TEST(test_zero_all_formats)
{
    test_explicit("0", "%+010d", "+000000000");
}

MU_TEST(test_zero_leading_zeros_all_formats)
{
    test_explicit("0000", "%+010d", "+000000000");
}

MU_TEST(test_zero_negative_all_formats)
{
    test_explicit("-0000", "%+010d", "+000000000");
}

MU_TEST(test_repeated_zeros_hex_input_single)
{
    test_explicit("0", "%i", "0");
}

MU_TEST(test_repeated_zeros_hex_input_double)
{
    test_explicit("00", "%i", "0");
}

MU_TEST(test_repeated_zeros_hex_input_triple)
{
    test_explicit("000", "%i", "0");
}

MU_TEST(test_repeated_zeros_hex_input_quad)
{
    test_explicit("0000", "%i", "0");
}

MU_TEST(test_repeated_zeros_hex_input_eight)
{
    test_explicit("00000000", "%i", "0");
}

MU_TEST(test_repeated_zeros_hex_input_16)
{
    test_explicit("0000000000000000", "%i", "0");
}

MU_TEST(test_repeated_zeros_hex_input_32)
{
    test_explicit("00000000000000000000000000000000", "%i", "0");
}

MU_TEST(test_negative_zero_single)
{
    test_explicit("-0", "%i", "0");
}

MU_TEST(test_negative_zero_double)
{
    test_explicit("-00", "%i", "0");
}

MU_TEST(test_negative_zero_triple)
{
    test_explicit("-000", "%i", "0");
}

MU_TEST(test_negative_zero_quad)
{
    test_explicit("-0000", "%i", "0");
}

MU_TEST(test_negative_zero_eight)
{
    test_explicit("-00000000", "%i", "0");
}

MU_TEST(test_format_single_zero)
{
    test_explicit("5", "%0d", "5");
}

MU_TEST(test_format_double_zero)
{
    test_explicit("5", "%00d", "5");
}

MU_TEST(test_format_triple_zero)
{
    test_explicit("5", "%000d", "5");
}

MU_TEST(test_format_quad_zero)
{
    test_explicit("5", "%0000d", "5");
}

MU_TEST(test_zero_with_format_double_zero)
{
    test_explicit("0", "%00d", "0");
}

MU_TEST(test_zero_with_format_triple_zero)
{
    test_explicit("0", "%000d", "0");
}

MU_TEST(test_zero_padded_width_01)
{
    test_explicit("5", "%01d", "5");
}

MU_TEST(test_zero_padded_width_05)
{
    test_explicit("5", "%05d", "00005");
}

MU_TEST(test_zero_padded_width_010)
{
    test_explicit("5", "%010d", "0000000005");
}

MU_TEST(test_hex_FF_format_05)
{
    test_explicit("FF", "%05d", "00255");
}

MU_TEST(test_hex_neg_A_format_05)
{
    test_explicit("-A", "%05d", "-0010");
}

MU_TEST(test_plus_single_zero)
{
    test_explicit("5", "%+0d", "+5");
}

MU_TEST(test_plus_double_zero)
{
    test_explicit("5", "%+00d", "+5");
}

MU_TEST(test_plus_zero_padded_05)
{
    test_explicit("5", "%+05d", "+0005");
}

MU_TEST(test_zero_with_plus_double_zero)
{
    test_explicit("0", "%+00d", "+0");
}

MU_TEST(test_negative_with_plus_05)
{
    test_explicit("-5", "%+05d", "-0005");
}

MU_TEST(test_minus_single_zero)
{
    test_explicit("5", "%-0d", "5");
}

MU_TEST(test_minus_width_5)
{
    test_explicit("5", "%-5d", "5    ");
}

MU_TEST(test_hex_ABC_minus_10)
{
    test_explicit("ABC", "%-10d", "2748      ");
}

MU_TEST(test_space_single_zero)
{
    test_explicit("5", "% 0d", " 5");
}

MU_TEST(test_space_zero_padded_05)
{
    test_explicit("5", "% 05d", " 0005");
}

MU_TEST(test_negative_with_space_05)
{
    test_explicit("-5", "% 05d", "-0005");
}

MU_TEST(test_plus_minus_width_5)
{
    test_explicit("5", "%+-5d", "+5   ");
}

MU_TEST(test_plus_minus_zero_padded_010)
{
    test_explicit("5", "%+-010d", "+5        ");
}

MU_TEST(test_zero_with_plus_minus_010)
{
    test_explicit("0", "%+-010d", "+0        ");
}

MU_TEST(test_negative_hex_A_plus_minus_010)
{
    test_explicit("-A", "%+-010d", "-10       ");
}

MU_TEST(test_leading_zeros_hex_format_010)
{
    test_explicit("00005", "%010d", "0000000005");
}

MU_TEST(test_leading_zeros_hex_A_format_05)
{
    test_explicit("0000A", "%05d", "00010");
}

MU_TEST(test_negative_leading_zeros_plus_010)
{
    test_explicit("-00005", "%+010d", "-000000005");
}

MU_TEST(test_zero_zero_format)
{
    test_explicit("0", "%0d", "0");
}

MU_TEST(test_quad_zero_quad_zero_format)
{
    test_explicit("0000", "%0000d", "0");
}

MU_TEST(test_double_zero_plus_double_zero)
{
    test_explicit("00", "%+00d", "+0");
}

MU_TEST(test_triple_zero_minus_double_zero)
{
    test_explicit("000", "%-00d", "0");
}

MU_TEST(test_negative_triple_zero_plus_triple_zero)
{
    test_explicit("-000", "%+000d", "+0");
}

MU_TEST(test_format_no_percent_single_zero)
{
    test_explicit("5", "0d", "5");
}

MU_TEST(test_format_no_percent_double_zero)
{
    test_explicit("5", "00d", "5");
}

MU_TEST(test_format_no_percent_05)
{
    test_explicit("5", "05d", "00005");
}

MU_TEST(test_format_no_percent_plus_05)
{
    test_explicit("5", "+05d", "+0005");
}

MU_TEST(test_format_no_percent_minus_10)
{
    test_explicit("ABC", "-10d", "2748      ");
}

MU_TEST(test_format_no_percent_space_05)
{
    test_explicit("5", " 05d", " 0005");
}

MU_TEST(test_empty_hex_string)
{
    test_explicit("", "%i", "0");
}

MU_TEST(test_empty_hex_with_format)
{
    test_explicit("", "%+10i", "        +0");
}

MU_TEST(test_empty_hex_left_align)
{
    test_explicit("", "%-10i", "0         ");
}

MU_TEST(test_empty_hex_zero_padding)
{
    test_explicit("", "%010i", "0000000000");
}

MU_TEST(test_null_format_string)
{
    test_explicit("42", NULL, "66");
}

MU_TEST(test_empty_format_string)
{
    test_explicit("42", "", "66");
}

MU_TEST(test_null_hex_string)
{
    char actual[32];
    print(actual, "%i", NULL);
    mu_assert_string_eq("0", actual);
}

MU_TEST(test_null_format_and_hex_null)
{
    char actual[32];
    print(actual, NULL, NULL);
    mu_assert_string_eq("0", actual);
}

MU_TEST(test_empty_both)
{
    test_explicit("", "", "0");
}

MU_TEST(test_null_format_empty_hex)
{
    test_explicit("", NULL, "0");
}

MU_TEST(test_minus_only)
{
    test_explicit("-", "%i", "0");
}

MU_TEST(test_minus_only_with_format)
{
    test_explicit("-", "%+10i", "        +0");
}

MU_TEST_SUITE(empty_and_null_cases)
{
    MU_RUN_TEST(test_empty_hex_string);
    MU_RUN_TEST(test_empty_hex_with_format);
    MU_RUN_TEST(test_empty_hex_left_align);
    MU_RUN_TEST(test_empty_hex_zero_padding);
    MU_RUN_TEST(test_null_format_string);
    MU_RUN_TEST(test_empty_format_string);
    MU_RUN_TEST(test_null_hex_string);
    MU_RUN_TEST(test_null_format_and_hex_null);
    MU_RUN_TEST(test_empty_both);
    MU_RUN_TEST(test_null_format_empty_hex);
    MU_RUN_TEST(test_minus_only);
    MU_RUN_TEST(test_minus_only_with_format);
}

MU_TEST(test_user_format_4_number_10)
{
    test_explicit("10", "4", "  16");
}

MU_TEST(test_user_format_minus4_number_minus10)
{
    test_explicit("-10", "-4", "-16 ");
}

MU_TEST(test_user_format_plus_number_10)
{
    test_explicit("10", "+", "+16");
}

MU_TEST(test_user_format_space_number_10)
{
    test_explicit("10", " ", " 16");
}

MU_TEST(test_no_percent_width_4)
{
    test_explicit("10", "4", "  16");
}

MU_TEST(test_no_percent_left_align_4)
{
    test_explicit("-10", "-4", "-16 ");
}

MU_TEST(test_no_percent_plus_flag)
{
    test_explicit("10", "+", "+16");
}

MU_TEST(test_no_percent_space_flag)
{
    test_explicit("10", " ", " 16");
}

MU_TEST(test_no_percent_zero_padding)
{
    test_explicit("10", "05", "00016");
}

MU_TEST(test_no_percent_left_align_width)
{
    test_explicit("FF", "-8", "255     ");
}

MU_TEST(test_no_percent_plus_width)
{
    test_explicit("A", "+5", "  +10");
}

MU_TEST(test_no_percent_space_width)
{
    test_explicit("14", " 6", "    20");
}

MU_TEST(test_no_percent_combo_plus_zero)
{
    test_explicit("7B", "+08", "+0000123");
}

MU_TEST(test_no_percent_combo_minus_width)
{
    test_explicit("C8", "-10", "200       ");
}

MU_TEST(test_no_percent_negative_with_plus)
{
    test_explicit("-1E", "+6", "   -30");
}

MU_TEST(test_no_percent_negative_with_space)
{
    test_explicit("-5", " 4", "  -5");
}

MU_TEST(test_with_percent_width_4)
{
    test_explicit("10", "%4", "  16");
}

MU_TEST(test_with_percent_left_align_4)
{
    test_explicit("-10", "%-4", "-16 ");
}

MU_TEST(test_with_percent_plus_flag)
{
    test_explicit("10", "%+", "+16");
}

MU_TEST(test_with_percent_space_flag)
{
    test_explicit("10", "% ", " 16");
}

MU_TEST(test_percent_only_format)
{
    test_explicit("1", "%", "1");
}

MU_TEST(test_percent_only_plus_flag)
{
    test_explicit("1", "%+", "+1");
}

MU_TEST(test_mixed_percent_usage_1)
{
    test_explicit("2A", "6", "    42");
}

MU_TEST(test_mixed_percent_usage_2)
{
    test_explicit("2A", "%6", "    42");
}

MU_TEST(test_no_percent_all_flags)
{
    test_explicit("64", "+010", "+000000100");
}

MU_TEST(test_no_percent_large_width)
{
    test_explicit("1", "15", "              1");
}

MU_TEST(test_no_percent_exact_width)
{
    test_explicit("3E8", "4", "1000");
}

MU_TEST(test_zero_flag_alone)
{
    test_explicit("5", "0", "5");
}

MU_TEST(test_format_zero_various_numbers)
{
    test_explicit("0", "0", "0");
    test_explicit("1", "0", "1");
    test_explicit("A", "0", "10");
    test_explicit("FF", "0", "255");
    test_explicit("123", "0", "291");
    test_explicit("DEADBEEF", "0", "3735928559");
    test_explicit("-1", "0", "-1");
    test_explicit("-A", "0", "-10");
    test_explicit("-FF", "0", "-255");
}

MU_TEST(test_zero_flag_double)
{
    test_explicit("5", "00", "5");
}

MU_TEST(test_zero_with_width_01)
{
    test_explicit("5", "01", "5");
}

MU_TEST(test_zero_with_width_02)
{
    test_explicit("5", "02", "05");
}

MU_TEST(test_zero_with_width_03)
{
    test_explicit("5", "03", "005");
}

MU_TEST(test_zero_with_width_004)
{
    test_explicit("5", "004", "0005");
}

MU_TEST(test_zero_with_width_00005)
{
    test_explicit("5", "00005", "00005");
}

MU_TEST(test_zero_double_digit_width)
{
    test_explicit("5", "010", "0000000005");
}

MU_TEST(test_percent_zero_flag_alone)
{
    test_explicit("5", "%0", "5");
}

MU_TEST(test_percent_zero_flag_double)
{
    test_explicit("5", "%00", "5");
}

MU_TEST(test_percent_zero_with_width_05)
{
    test_explicit("5", "%05", "00005");
}

MU_TEST(test_percent_zero_with_width_010)
{
    test_explicit("5", "%010", "0000000005");
}

MU_TEST(test_minus_zero_cancels)
{
    test_explicit("5", "%-0", "5");
}

MU_TEST(test_minus_zero_with_width)
{
    test_explicit("5", "%-05", "5    ");
}

MU_TEST(test_zero_minus_with_width)
{
    test_explicit("5", "0-5", "5    ");
}

MU_TEST(test_zero_with_plus_flag)
{
    test_explicit("5", "0+", "+5");
    test_explicit("5", "0+5", "+0005");
    test_explicit("5", "0+8", "+0000005");
    test_explicit("-5", "0+", "-5");
    test_explicit("-5", "0+5", "-0005");
}

MU_TEST(test_zero_with_space_flag)
{
    test_explicit("5", "0 ", " 5");
    test_explicit("5", "0 5", " 0005");
    test_explicit("5", "0 8", " 0000005");
    test_explicit("-5", "0 ", "-5");
    test_explicit("-5", "0 5", "-0005");
}

MU_TEST(test_plus_with_zero)
{
    test_explicit("5", "+0", "+5");
    test_explicit("5", "+05", "+0005");
    test_explicit("5", "+010", "+000000005");
    test_explicit("-A", "+0", "-10");
    test_explicit("-A", "+05", "-0010");
}

MU_TEST(test_space_with_zero)
{
    test_explicit("5", " 0", " 5");
    test_explicit("5", " 05", " 0005");
    test_explicit("5", " 010", " 000000005");
    test_explicit("-A", " 0", "-10");
    test_explicit("-A", " 05", "-0010");
}

MU_TEST(test_minus_with_zero)
{
    test_explicit("5", "-0", "5");
    test_explicit("5", "-05", "5    ");
    test_explicit("5", "-010", "5         ");
    test_explicit("FF", "-0", "255");
    test_explicit("FF", "-05", "255  ");
}

MU_TEST(test_zero_combinations_complex)
{
    test_explicit("5", "0+010", "+000000005");
    test_explicit("5", "0 010", " 000000005");
    test_explicit("5", "0-+10", "+5        ");
    test_explicit("5", "0- 10", " 5        ");
    test_explicit("-5", "0+010", "-000000005");
    test_explicit("-5", "0 010", "-000000005");
}

MU_TEST(test_many_leading_zeros_format)
{
    test_explicit("5", "000000000", "5");
    test_explicit("5", "0000000005", "00005");
    test_explicit("5", "00000000010", "0000000005");
    test_explicit("123", "0000000000", "291");
}

MU_TEST(test_percent_zero_combinations)
{
    test_explicit("5", "%0+", "+5");
    test_explicit("5", "%0+5", "+0005");
    test_explicit("5", "%0 ", " 5");
    test_explicit("5", "%0 5", " 0005");
    test_explicit("5", "%0-5", "5    ");
    test_explicit("5", "%0+010", "+000000005");
    test_explicit("5", "%0 010", " 000000005");
    test_explicit("5", "%0-+10", "+5        ");
}

MU_TEST(test_zero_flag_order_variations)
{
    test_explicit("5", "+0+5", "+0005");
    test_explicit("5", " 0 5", " 0005");
    test_explicit("5", "+0 5", "+0005");
    test_explicit("5", " 0+5", "+0005");
    test_explicit("5", "00+5", "+0005");
    test_explicit("5", "0+05", "+0005");
}

MU_TEST(test_zero_with_larger_numbers)
{
    test_explicit("FFFF", "0", "65535");
    test_explicit("FFFF", "010", "0000065535");
    test_explicit("FFFF", "0+010", "+000065535");
    test_explicit("FFFF", "0-10", "65535     ");
    test_explicit("7FFFFFFF", "0", "2147483647");
    test_explicit("7FFFFFFF", "015", "000002147483647");
}

MU_TEST(test_zero_with_negative_larger)
{
    test_explicit("-FFFF", "0", "-65535");
    test_explicit("-FFFF", "010", "-000065535");
    test_explicit("-FFFF", "0+010", "-000065535");
    test_explicit("-FFFF", "0 010", "-000065535");
    test_explicit("-7FFFFFFF", "015", "-00002147483647");
}

MU_TEST(test_format_zero_edge_cases)
{
    test_explicit("0", "0+", "+0");
    test_explicit("0", "0 ", " 0");
    test_explicit("0", "0-", "0");
    test_explicit("0", "0+5", "+0000");
    test_explicit("0", "0 5", " 0000");
    test_explicit("0", "0-5", "0    ");
    test_explicit("0", "010", "0000000000");
    test_explicit("-0", "0+", "+0");
    test_explicit("-0", "010", "0000000000");
}

MU_TEST(test_redundant_zero_flags)
{
    test_explicit("5", "000", "5");
    test_explicit("5", "0000", "5");
    test_explicit("5", "00000", "5");
    test_explicit("5", "000000", "5");
    test_explicit("5", "0005", "00005");
    test_explicit("5", "00005", "00005");
    test_explicit("5", "000005", "00005");
    test_explicit("5", "00000005", "00005");
    test_explicit("5", "000+", "+5");
    test_explicit("5", "0000+", "+5");
    test_explicit("5", "000 ", " 5");
    test_explicit("5", "0000 ", " 5");
    test_explicit("5", "000-", "5");
    test_explicit("5", "0000-", "5");
}

MU_TEST(test_redundant_plus_flags)
{
    test_explicit("5", "++", "+5");
    test_explicit("5", "+++", "+5");
    test_explicit("5", "++++", "+5");
    test_explicit("5", "++5", "   +5");
    test_explicit("5", "+++5", "   +5");
    test_explicit("5", "++++5", "   +5");
    test_explicit("5", "++05", "+0005");
    test_explicit("5", "+++05", "+0005");
    test_explicit("5", "+0+05", "+0005");
    test_explicit("5", "0++05", "+0005");
}

MU_TEST(test_redundant_space_flags)
{
    test_explicit("5", "  ", " 5");
    test_explicit("5", "   ", " 5");
    test_explicit("5", "    ", " 5");
    test_explicit("5", "  5", "    5");
    test_explicit("5", "   5", "    5");
    test_explicit("5", "    5", "    5");
    test_explicit("5", "  05", " 0005");
    test_explicit("5", "   05", " 0005");
    test_explicit("5", " 0 05", " 0005");
    test_explicit("5", "0  05", " 0005");
}

MU_TEST(test_redundant_minus_flags)
{
    test_explicit("5", "--", "5");
    test_explicit("5", "---", "5");
    test_explicit("5", "----", "5");
    test_explicit("5", "--5", "5    ");
    test_explicit("5", "---5", "5    ");
    test_explicit("5", "----5", "5    ");
    test_explicit("5", "--05", "5    ");
    test_explicit("5", "---05", "5    ");
    test_explicit("5", "-0-05", "5    ");
    test_explicit("5", "0--05", "5    ");
}

MU_TEST(test_complex_flag_redundancy)
{
    test_explicit("5", "00++", "+5");
    test_explicit("5", "++00", "+5");
    test_explicit("5", "0+0+", "+5");
    test_explicit("5", "+0+0", "+5");
    test_explicit("5", "00++5", "+0005");
    test_explicit("5", "00++05", "+0005");
    test_explicit("5", "++0005", "+0005");
    test_explicit("5", "0+0+05", "+0005");
    test_explicit("5", "0+ ", "+5");
    test_explicit("5", "0 +", "+5");
    test_explicit("5", "+0 ", "+5");
    test_explicit("5", "+ 0", "+5");
    test_explicit("5", " 0+", "+5");
    test_explicit("5", " +0", "+5");
}

MU_TEST(test_all_flag_permutations_with_zero)
{
    test_explicit("5", "0+5", "+0005");
    test_explicit("5", "+05", "+0005");
    test_explicit("5", "0 5", " 0005");
    test_explicit("5", " 05", " 0005");
    test_explicit("5", "0-5", "5    ");
    test_explicit("5", "-05", "5    ");
    test_explicit("5", "0+ 5", "+0005");
    test_explicit("5", "0 +5", "+0005");
    test_explicit("5", "+0 5", "+0005");
    test_explicit("5", "+ 05", "+0005");
    test_explicit("5", " 0+5", "+0005");
    test_explicit("5", " +05", "+0005");
    test_explicit("5", "0+-5", "+5   ");
    test_explicit("5", "0-+5", "+5   ");
    test_explicit("5", "+0-5", "+5   ");
    test_explicit("5", "+-05", "+5   ");
    test_explicit("5", "-0+5", "+5   ");
    test_explicit("5", "-+05", "+5   ");
}

MU_TEST(test_extreme_flag_redundancy)
{
    test_explicit("5", "0000000000", "5");
    test_explicit("5", "++++++++++", "+5");
    test_explicit("5", "          ", " 5");
    test_explicit("5", "----------", "5");
    test_explicit("5", "0000000000010", "0000000005");
    test_explicit("5", "++++++++++010", "+000000005");
    test_explicit("5", "          010", " 000000005");
    test_explicit("5", "----------010", "5         ");
    test_explicit("5", "00000+++++", "+5");
    test_explicit("5", "+++++00000", "+5");
    test_explicit("5", "00000+++++     ", "+5");
    test_explicit("5", "00000+++++-----", "+5");
}

MU_TEST(test_flag_order_all_combinations_2)
{
    test_explicit("A", "0+", "+10");
    test_explicit("A", "+0", "+10");
    test_explicit("A", "0 ", " 10");
    test_explicit("A", " 0", " 10");
    test_explicit("A", "0-", "10");
    test_explicit("A", "-0", "10");
    test_explicit("A", "+ ", "+10");
    test_explicit("A", " +", "+10");
    test_explicit("A", "+-", "+10");
    test_explicit("A", "-+", "+10");
    test_explicit("A", " -", " 10");
    test_explicit("A", "- ", " 10");
}

MU_TEST(test_flag_order_all_combinations_3)
{
    test_explicit("A", "0+-", "+10");
    test_explicit("A", "0-+", "+10");
    test_explicit("A", "+0-", "+10");
    test_explicit("A", "+-0", "+10");
    test_explicit("A", "-0+", "+10");
    test_explicit("A", "-+0", "+10");
    test_explicit("A", "0+ ", "+10");
    test_explicit("A", "0 +", "+10");
    test_explicit("A", "+0 ", "+10");
    test_explicit("A", "+ 0", "+10");
    test_explicit("A", " 0+", "+10");
    test_explicit("A", " +0", "+10");
    test_explicit("A", "0- ", " 10");
    test_explicit("A", "0 -", " 10");
    test_explicit("A", "-0 ", " 10");
    test_explicit("A", "- 0", " 10");
    test_explicit("A", " 0-", " 10");
    test_explicit("A", " -0", " 10");
    test_explicit("A", "+- ", "+10");
    test_explicit("A", "+ -", "+10");
    test_explicit("A", "-+ ", "+10");
    test_explicit("A", "- +", "+10");
    test_explicit("A", " +-", "+10");
    test_explicit("A", " -+", "+10");
}

MU_TEST(test_flag_order_all_combinations_4)
{
    test_explicit("A", "0+- ", "+10");
    test_explicit("A", "0+-5", "+10  ");
    test_explicit("A", "0 +-", "+10");
    test_explicit("A", "0 -+", "+10");
    test_explicit("A", "+0- ", "+10");
    test_explicit("A", "+0 -", "+10");
    test_explicit("A", "+ 0-", "+10");
    test_explicit("A", "- 0+", "+10");
    test_explicit("A", " -0+", "+10");
    test_explicit("A", " +-0", "+10");
}

MU_TEST(test_redundant_flags_with_negative)
{
    test_explicit("-A", "00++", "-10");
    test_explicit("-A", "00  ", "-10");
    test_explicit("-A", "00--", "-10");
    test_explicit("-A", "00++5", "-0010");
    test_explicit("-A", "00  5", "-0010");
    test_explicit("-A", "00--5", "-10  ");
    test_explicit("-A", "00++010", "-000000010");
    test_explicit("-A", "00  010", "-000000010");
}

MU_TEST(test_width_between_flags)
{
    test_explicit("5", "5+", "    5");
    test_explicit("5", "5 ", "    5");
    test_explicit("5", "5-", "    5");
    test_explicit("5", "50", "                                                 5");
    test_explicit("5", "+5+", "   +5");
    test_explicit("5", " 5 ", "    5");
    test_explicit("5", "-5-", "5    ");
}

MU_TEST_SUITE(zero_flag_edge_cases)
{
    MU_RUN_TEST(test_zero_flag_alone);
    MU_RUN_TEST(test_format_zero_various_numbers);
    MU_RUN_TEST(test_zero_flag_double);
    MU_RUN_TEST(test_zero_with_width_01);
    MU_RUN_TEST(test_zero_with_width_02);
    MU_RUN_TEST(test_zero_with_width_03);
    MU_RUN_TEST(test_zero_with_width_004);
    MU_RUN_TEST(test_zero_with_width_00005);
    MU_RUN_TEST(test_zero_double_digit_width);
    MU_RUN_TEST(test_percent_zero_flag_alone);
    MU_RUN_TEST(test_percent_zero_flag_double);
    MU_RUN_TEST(test_percent_zero_with_width_05);
    MU_RUN_TEST(test_percent_zero_with_width_010);
    MU_RUN_TEST(test_minus_zero_cancels);
    MU_RUN_TEST(test_minus_zero_with_width);
    MU_RUN_TEST(test_zero_minus_with_width);
    MU_RUN_TEST(test_zero_with_plus_flag);
    MU_RUN_TEST(test_zero_with_space_flag);
    MU_RUN_TEST(test_plus_with_zero);
    MU_RUN_TEST(test_space_with_zero);
    MU_RUN_TEST(test_minus_with_zero);
    MU_RUN_TEST(test_zero_combinations_complex);
    MU_RUN_TEST(test_many_leading_zeros_format);
    MU_RUN_TEST(test_percent_zero_combinations);
    MU_RUN_TEST(test_zero_flag_order_variations);
    MU_RUN_TEST(test_zero_with_larger_numbers);
    MU_RUN_TEST(test_zero_with_negative_larger);
    MU_RUN_TEST(test_format_zero_edge_cases);
    MU_RUN_TEST(test_redundant_zero_flags);
    MU_RUN_TEST(test_redundant_plus_flags);
    MU_RUN_TEST(test_redundant_space_flags);
    MU_RUN_TEST(test_redundant_minus_flags);
    MU_RUN_TEST(test_complex_flag_redundancy);
    MU_RUN_TEST(test_all_flag_permutations_with_zero);
    MU_RUN_TEST(test_extreme_flag_redundancy);
    MU_RUN_TEST(test_flag_order_all_combinations_2);
    MU_RUN_TEST(test_flag_order_all_combinations_3);
    MU_RUN_TEST(test_flag_order_all_combinations_4);
    MU_RUN_TEST(test_redundant_flags_with_negative);
    MU_RUN_TEST(test_width_between_flags);
}

MU_TEST_SUITE(format_without_percent)
{
    MU_RUN_TEST(test_user_format_4_number_10);
    MU_RUN_TEST(test_user_format_minus4_number_minus10);
    MU_RUN_TEST(test_user_format_plus_number_10);
    MU_RUN_TEST(test_user_format_space_number_10);
    MU_RUN_TEST(test_no_percent_width_4);
    MU_RUN_TEST(test_no_percent_left_align_4);
    MU_RUN_TEST(test_no_percent_plus_flag);
    MU_RUN_TEST(test_no_percent_space_flag);
    MU_RUN_TEST(test_no_percent_zero_padding);
    MU_RUN_TEST(test_no_percent_left_align_width);
    MU_RUN_TEST(test_no_percent_plus_width);
    MU_RUN_TEST(test_no_percent_space_width);
    MU_RUN_TEST(test_no_percent_combo_plus_zero);
    MU_RUN_TEST(test_no_percent_combo_minus_width);
    MU_RUN_TEST(test_no_percent_negative_with_plus);
    MU_RUN_TEST(test_no_percent_negative_with_space);
    MU_RUN_TEST(test_with_percent_width_4);
    MU_RUN_TEST(test_with_percent_left_align_4);
    MU_RUN_TEST(test_with_percent_plus_flag);
    MU_RUN_TEST(test_with_percent_space_flag);
    MU_RUN_TEST(test_percent_only_format);
    MU_RUN_TEST(test_percent_only_plus_flag);
    MU_RUN_TEST(test_mixed_percent_usage_1);
    MU_RUN_TEST(test_mixed_percent_usage_2);
    MU_RUN_TEST(test_no_percent_all_flags);
    MU_RUN_TEST(test_no_percent_large_width);
    MU_RUN_TEST(test_no_percent_exact_width);
}

MU_TEST_SUITE(zero_edge_cases)
{
    MU_RUN_TEST(test_zero_leading_zeros_1);
    MU_RUN_TEST(test_zero_leading_zeros_2);
    MU_RUN_TEST(test_zero_leading_zeros_3);
    MU_RUN_TEST(test_zero_leading_zeros_4);
    MU_RUN_TEST(test_zero_leading_zeros_many);
    MU_RUN_TEST(test_zero_negative_simple);
    MU_RUN_TEST(test_zero_negative_with_zeros_1);
    MU_RUN_TEST(test_zero_negative_with_zeros_2);
    MU_RUN_TEST(test_zero_negative_with_zeros_3);
    MU_RUN_TEST(test_zero_negative_many_zeros);
    MU_RUN_TEST(test_zero_with_plus_sign);
    MU_RUN_TEST(test_zero_leading_zeros_plus);
    MU_RUN_TEST(test_zero_negative_plus_format);
    MU_RUN_TEST(test_zero_negative_zeros_plus_format);
    MU_RUN_TEST(test_zero_width_padding);
    MU_RUN_TEST(test_zero_leading_zeros_width);
    MU_RUN_TEST(test_zero_negative_width);
    MU_RUN_TEST(test_zero_zero_padding);
    MU_RUN_TEST(test_zero_leading_zeros_zero_padding);
    MU_RUN_TEST(test_zero_negative_zero_padding);
    MU_RUN_TEST(test_zero_left_align);
    MU_RUN_TEST(test_zero_leading_zeros_left_align);
    MU_RUN_TEST(test_zero_negative_left_align);
    MU_RUN_TEST(test_zero_space_flag);
    MU_RUN_TEST(test_zero_leading_zeros_space);
    MU_RUN_TEST(test_zero_negative_space);
    MU_RUN_TEST(test_zero_all_formats);
    MU_RUN_TEST(test_zero_leading_zeros_all_formats);
    MU_RUN_TEST(test_zero_negative_all_formats);
    MU_RUN_TEST(test_repeated_zeros_hex_input_single);
    MU_RUN_TEST(test_repeated_zeros_hex_input_double);
    MU_RUN_TEST(test_repeated_zeros_hex_input_triple);
    MU_RUN_TEST(test_repeated_zeros_hex_input_quad);
    MU_RUN_TEST(test_repeated_zeros_hex_input_eight);
    MU_RUN_TEST(test_repeated_zeros_hex_input_16);
    MU_RUN_TEST(test_repeated_zeros_hex_input_32);
    MU_RUN_TEST(test_negative_zero_single);
    MU_RUN_TEST(test_negative_zero_double);
    MU_RUN_TEST(test_negative_zero_triple);
    MU_RUN_TEST(test_negative_zero_quad);
    MU_RUN_TEST(test_negative_zero_eight);
    MU_RUN_TEST(test_format_single_zero);
    MU_RUN_TEST(test_format_double_zero);
    MU_RUN_TEST(test_format_triple_zero);
    MU_RUN_TEST(test_format_quad_zero);
    MU_RUN_TEST(test_zero_with_format_double_zero);
    MU_RUN_TEST(test_zero_with_format_triple_zero);
    MU_RUN_TEST(test_zero_padded_width_01);
    MU_RUN_TEST(test_zero_padded_width_05);
    MU_RUN_TEST(test_zero_padded_width_010);
    MU_RUN_TEST(test_hex_FF_format_05);
    MU_RUN_TEST(test_hex_neg_A_format_05);
    MU_RUN_TEST(test_plus_single_zero);
    MU_RUN_TEST(test_plus_double_zero);
    MU_RUN_TEST(test_plus_zero_padded_05);
    MU_RUN_TEST(test_zero_with_plus_double_zero);
    MU_RUN_TEST(test_negative_with_plus_05);
    MU_RUN_TEST(test_minus_single_zero);
    MU_RUN_TEST(test_minus_width_5);
    MU_RUN_TEST(test_hex_ABC_minus_10);
    MU_RUN_TEST(test_space_single_zero);
    MU_RUN_TEST(test_space_zero_padded_05);
    MU_RUN_TEST(test_negative_with_space_05);
    MU_RUN_TEST(test_plus_minus_width_5);
    MU_RUN_TEST(test_plus_minus_zero_padded_010);
    MU_RUN_TEST(test_zero_with_plus_minus_010);
    MU_RUN_TEST(test_negative_hex_A_plus_minus_010);
    MU_RUN_TEST(test_leading_zeros_hex_format_010);
    MU_RUN_TEST(test_leading_zeros_hex_A_format_05);
    MU_RUN_TEST(test_negative_leading_zeros_plus_010);
    MU_RUN_TEST(test_zero_zero_format);
    MU_RUN_TEST(test_quad_zero_quad_zero_format);
    MU_RUN_TEST(test_double_zero_plus_double_zero);
    MU_RUN_TEST(test_triple_zero_minus_double_zero);
    MU_RUN_TEST(test_negative_triple_zero_plus_triple_zero);
    MU_RUN_TEST(test_format_no_percent_single_zero);
    MU_RUN_TEST(test_format_no_percent_double_zero);
    MU_RUN_TEST(test_format_no_percent_05);
    MU_RUN_TEST(test_format_no_percent_plus_05);
    MU_RUN_TEST(test_format_no_percent_minus_10);
    MU_RUN_TEST(test_format_no_percent_space_05);
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
    MU_RUN_SUITE(bit_128_extremes);
    MU_RUN_SUITE(bit_128_powers);
    MU_RUN_SUITE(bit_128_special_numbers);
    MU_RUN_SUITE(bit_128_patterns);
    MU_RUN_SUITE(bit_128_formatting);
    MU_RUN_SUITE(bit_128_large_numbers);
    MU_RUN_SUITE(bit_128_negation);
    MU_RUN_SUITE(bit_128_combinations);
    MU_RUN_SUITE(empty_and_null_cases);
    MU_RUN_SUITE(zero_flag_edge_cases);
    MU_RUN_SUITE(format_without_percent);
    MU_RUN_SUITE(zero_edge_cases);
    MU_REPORT();
    return MU_EXIT_CODE;
}
