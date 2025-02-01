#!/bin/sh

BIN=$1
test="./functional_tests/func_test_run.sh"

$test $BIN "Simple condition" "if true; then echo \"true\"; fi"
$test $BIN "Simple condition with else" "if false; then echo \"true\"; else echo \"false\"; fi"
$test $BIN "Simple condition with elif" "if false; then echo \"true\"; elif true; then echo \"elif\"; fi"

# Test for the case where the condition is false
$test $BIN "Simple condition with false" "if false; then echo true; fi"
$test $BIN "Simple condition with else and false" "if false; then echo \"true\"; else echo \"false\"; fi"
$test $BIN "Commands list with conditions" "echo toto; if true; then echo tkt; fi"

# Test for binary
$test $BIN "Simple condition with binary" "if true; then /bin/echo \"true\"; fi"
$test $BIN "Simple condition with else and binary" "if false; then /bin/echo \"true\"; else /bin/echo \"false\"; fi"

# Test bad if
$test $BIN "Bad if 1 " "if; then echo true; fi"
$test $BIN "Bad if 2 " "if; then echo true; else echo false; fi"
$test $BIN "Bad if 3 " "if echo encore; fi"
$test $BIN "Bad if 4 " "if echo encore; then echo true;"
$test $BIN "Bad if 5 " "if echo encore; then echo true; else echo false"
$test $BIN "Bad if 6 " "if echo encore; then echo true; elif echo encore; fi"
$test $BIN "Bad if 7 " "if echo test
then
fi"
$test $BIN "ACU" "if true then echo encore; fi"

# Test embedded if
$test $BIN "Embedded if" "if true; then if true; then echo \"true\"; fi; fi"
$test $BIN "Embedded if with else" "if true; then if false; then echo \"true\"; else echo \"false\"; fi; fi"
$test $BIN "Embedded if condition" "if if true; then echo \"true\"; fi; then echo \"true\"; fi"
$test $BIN "Embedded if recursive if" "if true; then if true; then if true; then echo \"true\"; fi; fi; fi"
$test $BIN "Embedded if recursive condition" "if if if if true; then echo \"true\"; fi; then echo \"true\"; fi; then echo \"true\"; fi"