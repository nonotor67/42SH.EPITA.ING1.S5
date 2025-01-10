#!/bin/bash

function test_functional() {
    local test_name="$1"               # Nom du test
    local command="$2"                 # Commande à tester
    local your_shell="../src/42sh"     # Chemin vers shell
    local ref_shell="/bin/bash"        # Shell de référence

    echo "Running test: $test_name"

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
    fi
}


test_functional "Simple ls" "ls"
test_functional "Simple ls with flags" "ls -l"
test_functional "Echo with big args" "echo helloooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"
test_functional "Echo with multiples args" "echo Hello the world"
test_functional "Echo with quotes" "echo \"Hello the world\""

# Test multiple commands
test_functional "Multiple commands" "echo Hello; echo World"
test_functional "Multiple commands with quotes" "echo \"Hello the world\"; ls; cat Makefile"
test_functional "Multiple commands with quotes and flags" "echo \"Hello the world\"; ls -l; cat Makefile"

# Test unknown command
test_functional "Unknown command" "unknown_command"