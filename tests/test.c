#include "../src/test.h"

#include <criterion/criterion.h>

Test(addition_tests, test_add_positive_numbers)
{
  int result = add(3, 5);
  cr_assert_eq(result, 8, "Expected 3 + 5 to be 8, but got %d", result);
}

Test(addition_tests, test_add_negative_numbers)
{
  int result = add(-3, -5);
  cr_assert_eq(result, -8, "Expected -3 + -5 to be -8, but got %d", result);
}

Test(addition_tests, test_add_positive_and_negative_numbers)
{
  int result = add(10, -5);
  cr_assert_eq(result, 5, "Expected 10 + -5 to be 5, but got %d", result);
}

Test(addition_tests, test_add_zero)
{
  int result = add(0, 0);
  cr_assert_eq(result, 0, "Expected 0 + 0 to be 0, but got %d", result);
}

Test(addition_tests, test_add_large_numbers)
{
  int result = add(1000000, 2000000);
  cr_assert_eq(result, 3000000,
               "Expected 1000000 + 2000000 to be 3000000, but got %d", result);
}

Test(mul_tests, test_mul_positive_numbers)
{
  int result = mul(3, 5);
  cr_assert_eq(result, 15, "Expected 3 * 5 to be 15, but got %d", result);
}

Test(mul_tests, test_mul_negative_numbers)
{
  int result = mul(-3, -5);
  cr_assert_eq(result, 15, "Expected -3 * -5 to be 15, but got %d", result);
}

Test(mul_tests, test_mul_zero_numbers)
{
  int result = mul(0, -5);
  cr_assert_eq(result, 0, "Expected -3 * -5 to be 15, but got %d", result);
}
