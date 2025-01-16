#!/bin/sh

total_tests=0
failed_tests=0

test_functional() {
    test_name="$1"
    command="$2"
    your_shell="../src/42sh"
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

test_functional "Ignore comments" "echo hello # comment"
test_functional "Ignore comments" "echo hello # comment"
test_functional "Ignore comments" "echo hello\n # echo hello comment"

echo
echo "==== Test Summary for $(basename "$0") ===="
echo "Total tests: $total_tests"
echo "Failed tests: $failed_tests"


if [ $failed_tests -gt 0 ]; then
    exit 1
else
    exit 0
fi
