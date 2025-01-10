#!/bin/bash
function test_functional() {
    local test_name="$1"               # Nom du test
    local command="$2"                 # Commande à tester
    local your_shell="../src/42sh"      # Chemin vers shell
    local ref_shell="/bin/bash"        # Shell de référence

    echo "Running test: $test_name"

    local your_output
    local your_exit_code
    your_output=$("$your_shell" -c "$command" 2>&1)
    your_exit_code=$?

    local ref_output
    local ref_exit_code
    ref_output=$("$ref_shell" -c "$command" 2>&1)
    ref_exit_code=$?

    if [[ "$your_output" == "$ref_output" && "$your_exit_code" == "$ref_exit_code" ]]; then
        echo "✅ Test passed: Outputs and exit codes match"
    else
        echo "❌ Test failed: Outputs or exit codes differ"
        echo "Command: $command"
        echo "Your shell output: $your_output"
        echo "Your shell exit code: $your_exit_code"
        echo "Reference shell output: $ref_output"
        echo "Reference shell exit code: $ref_exit_code"
    fi
}

test_functional "Ignore comments" "echo hello # comment"
test_functional "Ignore comments" "echo hello # comment"
test_functional "Ignore comments" "echo hello\n # echo hello comment"