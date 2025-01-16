#!/bin/bash

# Test simple redirections
failed_tests=0

bash --posix -c 'echo hello > actual'
../src/42sh -c 'echo hello > expected'

if ! diff actual expected; then
    echo "❌ Test failed: Outputs differ"
    ((failed_tests++))
fi

# Test appending to a file
bash --posix -c 'echo hello >> actual'
../src/42sh -c 'echo hello >> expected'

if ! diff actual expected; then
    echo "❌ Test failed: Outputs differ"
    ((failed_tests++))
fi

# Overwrite a file
bash --posix -c 'echo hello > actual'
../src/42sh -c 'echo hello > expected'

if ! diff actual expected; then
    echo "❌ Test failed: Outputs differ"
    ((failed_tests++))
fi

rm -f actual expected

# Test input redirection
expected=$(bash --posix -c 'cat < input.txt')
actual=$(../src/42sh -c 'cat < input.txt')

if [[ "$actual" != "$expected" ]]; then
    echo "❌ Test failed: Outputs differ"
    ((failed_tests++))
fi

# Test >& redirection
bash --posix -c 'echo hello >&2' 2> stderr_bash.txt
../src/42sh -c 'echo hello >&2' 2> stderr_42sh.txt

if ! diff stderr_bash.txt stderr_42sh.txt; then
    echo "❌ Test failed: Outputs differ"
    ((failed_tests++))
fi

rm -f stderr_bash.txt stderr_42sh.txt

# Test multiple redirections
bash --posix -c 'echo hello > actual 2> stderr_bash.txt'
../src/42sh -c 'echo hello > expected 2> stderr_42sh.txt'

if ! diff actual expected; then
    echo "❌ Test failed: Outputs differ"
    ((failed_tests++))
fi

if ! diff stderr_bash.txt stderr_42sh.txt; then
    echo "❌ Test failed: Outputs differ"
    ((failed_tests++))
fi

rm -f actual expected stderr_bash.txt stderr_42sh.txt

if [[ $failed_tests -gt 0 ]]; then
  exit 1
fi
