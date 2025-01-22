#!/bin/sh

BIN=$1
test="./functional_tests/func_test_run.sh"

# Test command block
$test $BIN "Simple command block" "{ echo Hello; echo World; }"
$test $BIN "Test with pipe" "{ echo Hello; echo World; } | cat -e"
$test $BIN "Test with pipe" "{ echo a; echo b; } | tr b h"

# Test command block with if
# $test $BIN "Test with if" "{ if true; then echo true; fi }"
