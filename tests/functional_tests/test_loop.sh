#!/bin/sh

BIN=$1
test="./functional_tests/func_test_run.sh"

# Test for loop
$test $BIN "For loop" 'for i in 1 2 3; do echo $i; done'

# Test break
$test $BIN "for break" 'for i in 1 2 3; do echo coucou; break; echo toto fi; echo $i; done'
# Test continue
$test $BIN "for continue" 'for i in 1 2 3; do echo $i; continue; echo toto fi; echo $i; done'

# Test while loop
$test $BIN "While loop" 'while false; do echo "toto"; done'
$test $BIN "While loop with variable" 'a=true;while $a; do echo Yann;a=false; done'

# Test break
$test $BIN "While break" 'while true; do echo coucou; break; echo toto fi; done'

# Test until loop
$test $BIN "Until loop" 'until true; do echo toto; done'
$test $BIN "Until loop with variable" 'a=false;while $a; do echo Noah;a=true; done'

# Test break
$test $BIN "Until break" 'until false; do echo coucou; break; echo toto fi; done'
