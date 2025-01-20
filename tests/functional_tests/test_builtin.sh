#!/bin/sh

BIN=$1
test="./functional_tests/func_test_run.sh"

$test $BIN "Simple echo test" "echo Hello world"
$test $BIN "Simple echo test with newline" "echo -n Hello world"
$test $BIN "Simple true" "true"
$test $BIN "Simple false" "false"

# Test dot builtin

$test $BIN "Simple dot test" ". ./functional_tests/dot_script/simple_test.sh"
$test $BIN "Simple variable test" "a=hello;. ./functional_tests/dot_script/variable_test.sh;echo $a; echo $b"
