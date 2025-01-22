#!/bin/sh

path_to_tests="/functional_tests"
total_tests=0
failed_tests=0

BIN=$1
printf "TOTAL_TEST=0\nFAIL_TEST=0" > test_status.sh

run_test_script() {
    script="$1"
    echo "Executing test script: $script"
    ./"$script" $BIN
}

# Exécuter chaque script de test
run_test_script "$path_to_tests/test_builtin.sh"
run_test_script "$path_to_tests/test_command.sh"
run_test_script "$path_to_tests/test_condition.sh"
run_test_script "$path_to_tests/test_comments.sh"
run_test_script "$path_to_tests/test_redirection.sh"
run_test_script "$path_to_tests/test_script.sh"
run_test_script "$path_to_tests/test_pipeline.sh"
run_test_script "$path_to_tests/test_script_stdin.sh"
run_test_script "$path_to_tests/test_fails.sh"
run_test_script "$path_to_tests/test_loop.sh"

