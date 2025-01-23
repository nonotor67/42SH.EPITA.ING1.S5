#!/bin/sh

BIN=$1
test="./functional_tests/func_test_run.sh"

# Test simple function
$test $BIN "Simple function" 'toto() { echo "coucou"; }; toto'
$test $BIN "Simple function with arguments" 'toto() { echo $1; }; toto coucou; echo $1'

# Test multiple functions
$test $BIN "Multiple functions" 'toto() { echo "coucou"; }; titi() { echo "tata"; }; toto; titi'
# Test multiple functions with arguments
$test $BIN "Multiple functions with arguments" 'toto() { echo $1; }; titi() { echo $1; }; toto coucou; titi tata; echo $1'

# Test function in function
$test $BIN "Function in function" 'toto() { titi() { echo "coucou"; }; titi; }; toto'
$test $BIN "Function in function with arguments" 'toto() { titi() { echo $1; }; titi coucou; echo $1; }; toto tata; echo $1'