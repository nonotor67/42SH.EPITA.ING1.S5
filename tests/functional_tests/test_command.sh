#!/bin/sh

BIN=$1
test="./functional_tests/func_test_run.sh"

$test $BIN "Simple ls" "ls"
$test $BIN "Simple ls with flags" "ls -l"
$test $BIN "Test echo with key words" "echo true false if then else elif fi"
$test $BIN "Echo with big args" "echo helloooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
$test $BIN "Echo with multiples args" "echo Hello the world"
$test $BIN "Echo long spaces" "echo Hello     the     world"
$test $BIN "Echo lots of newlines" "echo test




echo encore"

# Test with binary
$test $BIN "Simple echo" "/bin/echo Hello world"
$test $BIN "Simple echo with newline" "/bin/echo -n Hello world"
$test $BIN "Simple true" "/bin/true"
$test $BIN "Simple false" "/bin/false"

# Test with variables

$test $BIN "Simple echo with variable" "echo \$PWD"
$test $BIN "Simple echo with variable" "echo \$@"
$test $BIN "Simple echo with variable" "echo \$?"
$test $BIN "Simple echo with variable" "echo \$1"
$test $BIN "Simple echo with variable" "echo \$toto"

# Test with for
$test $BIN "Simple for" "for i in 1 2 3; do echo \$i; done"
$test $BIN "Simple for" "for i in 1; do echo \$i; done"
$test $BIN "Bad for" "for i in 1 2; do echo \$i"
# TODO: fix test_functional "Bad for" "for i in 1 2; do echo \$i; done; done"
$test $BIN "Bad for" "for i in 1 2; do echo \$i done"

# Test variables

$test $BIN "Simple variable" "toto=42; echo \$toto"
$test $BIN "Simple \$?" "echo \$?"
$test $BIN "Complexe \$?" "if true; then false || echo \$?; fi"
$test $BIN "Complexe \$?" "if true; then true && echo \$?; fi"
$test $BIN "Complexe \$?" "if echo $?; then echo \$?; true; echo \$?; fi"
