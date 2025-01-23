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

# Edge cases
$test $BIN "Echo with quotes" "echo \"Hello the world\""
$test $BIN "Echo empty" "echo"
$test $BIN "Echo empty with double quotes" "echo \"\""
$test $BIN "Echo empty with single quotes" "echo ''"
$test $BIN "Echo with quotes and spaces" "echo \"H e l l o   t h e  world\""
$test $BIN "triple single quotes" "echo '''"

# echo keywords
$test $BIN "Echo with if" "echo if"
$test $BIN "Echo with then, elif, fi, else" "echo then ; echo else ; echo elif ; echo fi"
$test $BIN "Echo with while" "echo while"
$test $BIN "Echo with do" "echo do"
$test $BIN "Echo with done" "echo done"
$test $BIN "Echo with for" "echo for"
$test $BIN "Echo with in" "echo in"
# $test $BIN "Echo with !" "echo !"

# Tests quoted keywords
$test $BIN "Echo with quoted if" "echo \"if\""
$test $BIN "if quoted" "\"if\" false; then echo \"if\"; fi"
$test $BIN "if not quoted" "if false; then echo \"if\"; fi"
$test $BIN "then quoted" "if false; 'then' echo \"then\"; fi"
$test $BIN "then not quoted" "if false; then echo \"then\"; fi"
$test $BIN "for quoted" "'for' i in 1 2 3; do echo \$i; done"
$test $BIN "for not quoted" "for i in 1 2 3; do echo \$i; done"
$test $BIN "do quoted" "for i in 1 2 3; 'do' echo \$i; done"
$test $BIN "do not quoted" "for i in 1 2 3; do echo \$i; done"
$test $BIN "done quoted" "for i in 1 2 3; do echo \$i; 'done'"
$test $BIN "done not quoted" "for i in 1 2 3; do echo \$i; done"

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

# Empty command block

$test $BIN "Empty command block" "{ }"