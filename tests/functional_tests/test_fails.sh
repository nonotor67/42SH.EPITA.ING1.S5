#!/bin/sh

BIN=$1
test="./functional_tests/func_test_run.sh"

$test $BIN "Bad for loop" 'for i in 1 2 3; do echo $i; done; done'
$test $BIN "Bad while loop" 'while false; do echo "toto"; done; done'
$test $BIN "Bad if condition" 'if true; then echo "toto"; fi; fi'
$test $BIN "Bad if condition (no then)" 'if true; fi'
$test $BIN "Bad for loop (no do)" 'for i in 1 2 3; echo $i; done'
