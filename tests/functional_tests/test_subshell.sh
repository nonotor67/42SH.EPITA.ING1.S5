#!/bin/sh

BIN="$1"
test="./functional_tests/func_test_run.sh"

$test $BIN "Simple subshell" "(echo hello)"
$test $BIN "Multiple subshell" "(echo hello; echo world)"
$test $BIN "Complicated subshell" "(echo hello | tr a-z A-Z | rev)"

# Test for variable substitution
$test $BIN "Test with variable substitution" "a=sh; (a=42; echo -n $a);echo $a"
$test $BIN "Test with variable substitution" "a=sh; (a=42;b=yann; echo -n $a $b);echo $a $b"

# Test with multiple subshell
$test $BIN "Test with multiple subshell" '(echo hello; (echo world))'
$test $BIN "Test with multiple subshell" '(echo hello; (echo world; echo yann))'

# Test with multiple subshell and variable substitution
$test $BIN "Test with multiple subshell and variable substitution" 'a=sh; (a=42; echo -n $a);echo $a; (a=yann;echo $a; (echo $a;a=noah;echo $a;b=2);echo $a);echo $a;echo $b'

# Test subshell with cd
$test $BIN "Test subshell with cd" '(cd /; pwd); pwd'
$test $BIN "Test subshell with cd" '(cd /; pwd); pwd; (cd /usr; pwd); pwd'

# Test subshell with function
$test $BIN "Test subshell with function" '(coucou() { echo hello; }; coucou); coucou'
$test $BIN "Test subshell with function" '(coucou() { echo hello; }; coucou); coucou; (coucou() { echo world; }; coucou); coucou'

# Test subshell with function and variable substitution
$test $BIN "Test subshell with function and variable substitution" 'a=sh; (coucou() { echo hello; }; coucou); coucou; (coucou() { echo $a; }; coucou); coucou'
