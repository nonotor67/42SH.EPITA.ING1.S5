#!/bin/sh

BIN="$1"
test="./functional_tests/func_test_run.sh"

$test $BIN "Escape single quote" "echo 'hello\'"
$test $BIN "Escape double quote" 'echo "hello\"world"'
$test $BIN "Escape backslash" 'echo "hello\\"'
$test $BIN "Escape backslash 2" 'echo "hello\\\\world"'
$test $BIN "Escape new line" 'echo "hello\
world"'
$test $BIN "No escape new line" 'echo "hello
world"'
$test $BIN "No escape tab" 'echo "cou\"cou\"c est moi"'