#!/bin/bash

path_to_tests="/functional_tests"
total_tests=0
failed_tests=0

function run_test_script() {
    local script="$1"
    echo "Executing test script: $script"
    ./$script
    local script_exit_code=$?
    if [[ $script_exit_code -ne 0 ]]; then
        ((failed_tests++))
    fi
    ((total_tests++))
}

# Exécuter chaque script de test
run_test_script "$path_to_tests/test_builtin.sh"
run_test_script "$path_to_tests/test_command.sh"
run_test_script "$path_to_tests/test_condition.sh"
run_test_script "$path_to_tests/test_comments.sh"

# Résumé global
echo
echo "==== Global Test Summary ===="
echo "Total scripts: $total_tests"
echo "Failed scripts: $failed_tests"

if [[ $failed_tests -gt 0 ]]; then
    exit 1
else
    exit 0
fi
