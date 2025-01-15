#!/bin/bash

total_tests=0
failed_tests=0

function test_functional() {
    local test_name="$1"
    local command="$2"
    local your_shell="../src/42sh"
    local ref_shell="/bin/bash"

    echo "Running test: $test_name"
    ((total_tests++))

    local your_stdout your_stderr your_exit_code
    your_stdout=$(mktemp)
    your_stderr=$(mktemp)
    "$your_shell" -c "$command" >"$your_stdout" 2>"$your_stderr"
    your_exit_code=$?

    local ref_stdout ref_stderr ref_exit_code
    ref_stdout=$(mktemp)
    ref_stderr=$(mktemp)
    "$ref_shell" -c "$command" >"$ref_stdout" 2>"$ref_stderr"
    ref_exit_code=$?

    local your_output=$(<"$your_stdout")
    local your_error=$(<"$your_stderr")
    local ref_output=$(<"$ref_stdout")
    local ref_error=$(<"$ref_stderr")

    rm -f "$your_stdout" "$your_stderr" "$ref_stdout" "$ref_stderr"

    if [[ "$your_output" == "$ref_output" && "$your_exit_code" == "$ref_exit_code" ]]; then
        if [[ -n "$ref_error" && -z "$your_error" ]]; then
            echo "❌ Test failed: Expected stderr but none was produced"
            ((failed_tests++))
        else
            echo "✅ Test passed: Outputs and exit codes match"
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
        ((failed_tests++))
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


if [[ $failed_tests -gt 0 ]]; then
    exit 1
else
    exit 0
fi
