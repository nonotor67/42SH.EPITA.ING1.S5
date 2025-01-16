#!/bin/sh

if [ "$COVERAGE" = "yes" ] ; then
  # Run unit tests
  ./unit_tests
  # Run functional tests
  ./functional_tests/func_tests.sh $BIN_PATH
else
  # Only test functionnal tests
  ./functional_tests/func_tests.sh $BIN_PATH
fi

if [ -n "$OUTPUT_FILE" ] ; then
  echo "Output file: $OUTPUT_FILE"
fi
