#!/bin/sh

# Test simple redirections
failed_tests=0
BIN=$1

bash --posix -c 'echo hello > actual'
$BIN -c 'echo hello > expected'

if ! diff actual expected; then
    echo "❌ Test failed: Outputs differ"
    failed_tests=$((failed_tests + 1))
fi

# Test appending to a file
bash --posix -c 'echo hello >> actual'
$BIN -c 'echo hello >> expected'

if ! diff actual expected; then
    echo "❌ Test failed: Outputs differ"
    failed_tests=$((failed_tests + 1))
fi

# Overwrite a file
bash --posix -c 'echo hello > actual'
$BIN -c 'echo hello > expected'

if ! diff actual expected; then
    echo "❌ Test failed: Outputs differ"
    failed_tests=$((failed_tests + 1))
fi

rm -f actual expected

# Test input redirection
expected=$(bash --posix -c 'cat < input.txt')
actual=$($BIN -c 'cat < input.txt')

if [ "$actual" != "$expected" ]; then
    echo "❌ Test failed: Outputs differ"
    failed_tests=$((failed_tests + 1))
fi

# Test >& redirection
bash --posix -c 'echo hello >&2' 2> stderr_bash.txt
$BIN -c 'echo hello >&2' 2> stderr_42sh.txt

if ! diff stderr_bash.txt stderr_42sh.txt; then
    echo "❌ Test failed: Outputs differ"
    failed_tests=$((failed_tests + 1))
fi

rm -f stderr_bash.txt stderr_42sh.txt

# Test multiple redirections
bash --posix -c 'echo hello > actual 2> stderr_bash.txt'
$BIN -c 'echo hello > expected 2> stderr_42sh.txt'

if ! diff actual expected; then
    echo "❌ Test failed: Outputs differ"
    failed_tests=$((failed_tests + 1))
fi

if ! diff stderr_bash.txt stderr_42sh.txt; then
    echo "❌ Test failed: Outputs differ"
    failed_tests=$((failed_tests + 1))
fi

rm -f actual expected stderr_bash.txt stderr_42sh.txt

if [ $failed_tests -gt 0 ]; then
  echo "❌ $failed_tests tests failed"
  exit 1
fi
