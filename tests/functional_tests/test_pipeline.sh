#!/bin/bash

total_tests=0
failed_tests=0

function test_functional() {
    local test_name="$1"
    local command="$2"
    local your_shell="../src/42sh"
    local ref_shell="/bin/bash"

    ((total_tests++))

    local your_stdout your_stderr your_exit_code
    your_stdout=$(mktemp)
    your_stderr=$(mktemp)
    "$your_shell" -c "$command" >"$your_stdout" 2>"$your_stderr"
    your_exit_code=$?

    local ref_stdout ref_stderr ref_exit_code
    ref_stdout=$(mktemp)
    ref_stderr=$(mktemp)
    "$ref_shell" --posix -c "$command" >"$ref_stdout" 2>"$ref_stderr"
    ref_exit_code=$?

    local your_output=$(<"$your_stdout")
    local your_error=$(<"$your_stderr")
    local ref_output=$(<"$ref_stdout")
    local ref_error=$(<"$ref_stderr")

    rm -f "$your_stdout" "$your_stderr" "$ref_stdout" "$ref_stderr"

    if [[ "$your_output" == "$ref_output" && "$your_exit_code" == "$ref_exit_code" ]]; then
        if [[ -n "$ref_error" && -z "$your_error" ]]; then
            echo "❌ Test failed: Expected stderr but none was produced"
            echo "Test: $test_name"
            ((failed_tests++))
        fi
    else
        echo "❌ Test failed: Outputs or exit codes differ for test: $test_name"
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

test_functional "Simple pipeline" "echo hello | cat"
test_functional "Multiple pipeline" "echo hello | cat | wc -c"
test_functional "Complicated pipeline" "echo hello | tr a-z A-Z | rev"
test_functional "Test with if" "echo hello | if grep hello; then echo found; fi"
test_functional "Test with a fail command" "echo hello | non_existent_command | cat"

echo
echo "==== Test Summary for $(basename "$0") ===="
echo "Total tests: $total_tests"
echo "Failed tests: $failed_tests"


if [[ $failed_tests -gt 0 ]]; then
    exit 1
else
    exit 0
fi