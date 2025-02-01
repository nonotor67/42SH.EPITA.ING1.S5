#!/bin/sh

BIN=$1
test="./functional_tests/func_test_run.sh"

# Test command block
$test $BIN "Simple command block" "{ echo Hello; echo World; }"
$test $BIN "Test with pipe" "{ echo Hello; echo World; } | cat -e"
$test $BIN "Test with pipe" "{ echo a; echo b; } | tr b h"

# Test command block with if
$test $BIN "Test with if" "{ if true; then echo true; fi }"
$test $BIN "Test with if" "{ if false; then echo true; else echo false; fi }"

# Test command block with while
$test $BIN "Test with while" "{ while true; do echo true; break; done }"
$test $BIN "Test with while" "{ while false; do echo true; done }"

# Test command block with for
$test $BIN "Test with for" "{ for i in 1 2 3; do echo \$i; done; }"
$test $BIN "Test with for" "{ for i in 1 2 3; do echo \$i; done | cat -e; }"

# Test command block with and
$test $BIN "Test with and" "{ echo a && echo b; }"
$test $BIN "Test with and" "{ echo a && false; }"

# Test command block with or
$test $BIN "Test with or" "{ echo a || echo b; }"
$test $BIN "Test with or" "{ false || echo b; }"

# Test command block with negation
$test $BIN "Test with negation" "{ ! false; }"
$test $BIN "Test with negation" "{ ! true; }"

# Test command block with parenthesis
$test $BIN "Test with parenthesis" "{ (echo a); }"
$test $BIN "Test with parenthesis" "{ (echo a && echo b); }"

# Test command block with redirection
$test $BIN "Test with redirection" "{ echo a > file; }"
$test $BIN "Test with redirection" "{ echo a > file; cat file; }"
rm -f file
