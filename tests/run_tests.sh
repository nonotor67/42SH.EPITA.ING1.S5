#!/bin/sh

if [ "$COVERAGE" = "yes" ] ; then
  # Run unit tests
  ./unit_tests || true
  # Run functional tests
  timeout 60s ./functional_tests/func_tests.sh $BIN_PATH
else
  # Only test functionnal tests
  timeout 60s ./functional_tests/func_tests.sh $BIN_PATH
fi

. ./test_status.sh

TEST_SUCCESS=$(( TOTAL_TEST - FAIL_TEST ))
PERCENTAGE=$(( TEST_SUCCESS * 100 / TOTAL_TEST ))
echo "Total tests: $TOTAL_TEST"
echo "Tests passed: $TEST_SUCCESS"
echo "Tests failed: $FAIL_TEST"
echo "Percentage of tests passed: $PERCENTAGE%"

if [ -n "$OUTPUT_FILE" ] ; then
  # Write the % of tests passed
  echo $PERCENTAGE > $OUTPUT_FILE
fi

rm -f test_status.sh

if [ $FAIL_TEST -gt 0 ] && [ $PIPELINE  = "yes" ]; then
  exit 1
else
  exit 0
fi