#!/bin/sh

BIN="$1"
test="./functional_tests/func_test_run.sh"

$test $BIN "Simple pipeline" "echo hello | cat"
$test $BIN "Multiple pipeline" "echo hello | cat | wc -c"
$test $BIN "Complicated pipeline" "echo hello | tr a-z A-Z | rev"
$test $BIN "Test with if" "echo hello | if grep hello; then echo found; fi"
$test $BIN "Test with a fail command" "echo hello | non_existent_command | cat"
