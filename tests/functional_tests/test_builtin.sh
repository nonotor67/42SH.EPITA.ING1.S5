#!/bin/sh

BIN=$1
test="./functional_tests/func_test_run.sh"

$test $BIN "Simple echo test" "echo Hello world"
$test $BIN "Simple echo test with newline" "echo -n Hello world"
$test $BIN "Complex echo test" "echo -eEe \\\\"
$test $BIN "Complex echo test" "echo -ne '\\'"
$test $BIN "Complex echo test" "echo -nE '\\'"
$test $BIN "Complex echo test" "echo -nEe '\\\\'"
$test $BIN "Complex echo test" "echo -nEeeeee '\\\\\\\\\\\'"
$test $BIN "Complex echo test" "echo -eeeeeeEEEE '\\\\'"
$test $BIN "Simple true" "true"
$test $BIN "Simple false" "false"

# Test cd
$test $BIN "Simple cd" "cd /;echo $PWD;ls"
$test $BIN "Simple cd with path" "cd /usr;echo $PWD;ls"
# Test error
$test $BIN "Simple cd with error" "cd unexpected_dir;echo $PWD;ls"
# Test with -
$test $BIN "Simple cd with -" "cd /usr;cd -"