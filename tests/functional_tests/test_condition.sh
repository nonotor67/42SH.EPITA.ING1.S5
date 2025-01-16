#!/bin/sh

total_tests=0
failed_tests=0

BIN=$1
# Obtain the test status
. ./test_status.sh

test_functional() {
    test_name="$1"
    command="$2"
    your_shell="$BIN"
    ref_shell="bash"

    total_tests=$((total_tests + 1))

    your_stdout=$(mktemp)
    your_stderr=$(mktemp)
    "$your_shell" -c "$command" >"$your_stdout" 2>"$your_stderr"
    your_exit_code=$?

    ref_stdout=$(mktemp)
    ref_stderr=$(mktemp)
    "$ref_shell" -c "$command" >"$ref_stdout" 2>"$ref_stderr"
    ref_exit_code=$?

    your_output=$(cat "$your_stdout")
    your_error=$(cat "$your_stderr")
    ref_output=$(cat "$ref_stdout")
    ref_error=$(cat "$ref_stderr")

    rm -f "$your_stdout" "$your_stderr" "$ref_stdout" "$ref_stderr"

    if [ "$your_output" = "$ref_output" ] && [ "$your_exit_code" -eq "$ref_exit_code" ]; then
        if [ -n "$ref_error" ] && [ -z "$your_error" ]; then
            echo "❌ Test failed: Expected stderr but none was produced"
            failed_tests=$((failed_tests + 1))
        fi
    else
        echo "❌ Test failed: Outputs or exit codes differ"
        echo "Command: $command"
        echo "Your shell output: $your_output"
        echo "Your shell stderr: $your_error"
        echo "Your shell exit code: $your_exit_code"
        echo "Reference shell output: $ref_output"
        echo "Reference shell stderr: $ref_error"
        echo "Reference shell exit code: $ref_exit_code"
        failed_tests=$((failed_tests + 1))
    fi
}

test_functional "Simple condition" "if true; then echo \"true\"; fi"
test_functional "Simple condition with else" "if false; then echo \"true\"; else echo \"false\"; fi"
test_functional "Simple condition with elif" "if false; then echo \"true\"; elif true; then echo \"elif\"; fi"

# Test for the case where the condition is false
test_functional "Simple condition with false" "if false; then echo true; fi"
test_functional "Simple condition with else and false" "if false; then echo \"true\"; else echo \"false\"; fi"
test_functional "Commands list with conditions" "echo toto; if true; then echo tkt; fi"

# Test for binary
test_functional "Simple condition with binary" "if true; then /bin/echo \"true\"; fi"
test_functional "Simple condition with else and binary" "if false; then /bin/echo \"true\"; else /bin/echo \"false\"; fi"

# Test bad if
test_functional "Bad if 1 " "if; then echo true; fi"
test_functional "Bad if 2 " "if; then echo true; else echo false; fi"
test_functional "Bad if 3 " "if echo encore; fi"
test_functional "Bad if 4 " "if echo encore; then echo true;"
test_functional "Bad if 5 " "if echo encore; then echo true; else echo false"
test_functional "Bad if 6 " "if echo encore; then echo true; elif echo encore; fi"
test_functional "Bad if 7 " "if echo test
then
fi"
test_functional "ACU" "if true then echo encore; fi"

echo
echo "==== Test Summary for $(basename "$0") ===="
echo "Total tests: $total_tests"
echo "Failed tests: $failed_tests"

TOTAL_TEST=$((TOTAL_TEST + total_tests))
FAIL_TEST=$((FAIL_TEST + failed_tests))
echo "TOTAL_TEST=$TOTAL_TEST" >test_status.sh
echo "FAIL_TEST=$FAIL_TEST" >>test_status.sh

if [ $failed_tests -gt 0 ]; then
    exit 1
else
    exit 0
fi
