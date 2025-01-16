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

test_functional "Simple ls" "ls"
test_functional "Simple ls with flags" "ls -l"
test_functional "Test echo with key words" "echo true false if then else elif fi"
test_functional "Echo with big args" "echo helloooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
test_functional "Echo with multiples args" "echo Hello the world"
test_functional "Echo long spaces" "echo Hello     the     world"
test_functional "Echo lots of newlines" "echo test




echo encore"

# Test with binary
test_functional "Simple echo" "/bin/echo Hello world"
test_functional "Simple echo with newline" "/bin/echo -n Hello world"
test_functional "Simple true" "/bin/true"
test_functional "Simple false" "/bin/false"

echo
echo "==== Test Summary for $(basename "$0") ===="
echo "Total tests: $total_tests"
echo "Failed tests: $failed_tests"

# Test with variables

test_functional "Simple echo with variable" "echo \$PWD"
test_functional "Simple echo with variable" "echo \$@"
test_functional "Simple echo with variable" "echo \$?"
test_functional "Simple echo with variable" "echo \$1"
toto=pouet
test_functional "Simple echo with variable" "echo \$toto"

# Test with for
test_functional "Simple for" "for i in 1 2 3; do echo \$i; done"
test_functional "Simple for" "for i in 1; do echo \$i; done"
test_functional "Bad for" "for i in 1 2; do echo \$i"
# TODO: fix test_functional "Bad for" "for i in 1 2; do echo \$i; done; done"
test_functional "Bad for" "for i in 1 2; do echo \$i done"


if [ $failed_tests -gt 0 ]; then
    exit 1
else
    exit 0
fi
