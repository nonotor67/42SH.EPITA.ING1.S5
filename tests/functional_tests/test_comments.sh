#!/bin/sh

BIN=$1
test="./functional_tests/func_test_run.sh"

$test $BIN "Ignore comments" "echo hello # comment"
$test $BIN "Ignore comments" "echo hello # comment"
$test $BIN "Ignore comments" "echo hello\n # echo hello comment"
