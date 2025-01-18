#!/bin/sh

# Obtain the test status
. ./test_status.sh

# Arguments:
# $1: Path to your shell
# $2: Test name
# $3: Command to run
your_shell="$1"
ref_shell="bash"
test_name="$2"
command="$3"

your_stdout=$(mktemp)
your_stderr=$(mktemp)
"$your_shell" -c "$command" >"$your_stdout" 2>"$your_stderr"
your_exit_code=$?

ref_stdout=$(mktemp)
ref_stderr=$(mktemp)
"$ref_shell" --posix -c "$command" >"$ref_stdout" 2>"$ref_stderr"
ref_exit_code=$?

your_output=$(cat "$your_stdout")
your_error=$(cat "$your_stderr")
ref_output=$(cat "$ref_stdout")
ref_error=$(cat "$ref_stderr")

rm -f "$your_stdout" "$your_stderr" "$ref_stdout" "$ref_stderr"

if [ "$your_output" = "$ref_output" ] && [ "$your_exit_code" -eq "$ref_exit_code" ]; then
  if [ -n "$ref_error" ] && [ -z "$your_error" ]; then
    echo "❌ Test failed: Expected stderr but none was produced"
    echo "Test name: $test_name"
    echo "Command: $command"
    failed_tests=$((failed_tests + 1))
  fi
else
  echo "❌ Test failed: Outputs or exit codes differ"
  echo "Test name: $test_name"
  echo "Command: $command"
  echo "Your shell output: $your_output"
  echo "Your shell stderr: $your_error"
  echo "Your shell exit code: $your_exit_code"
  echo "Reference shell output: $ref_output"
  echo "Reference shell stderr: $ref_error"
  echo "Reference shell exit code: $ref_exit_code"
  FAIL_TEST=$((FAIL_TEST + 1))
fi

TOTAL_TEST=$((TOTAL_TEST + 1))
echo "TOTAL_TEST=$TOTAL_TEST" >test_status.sh
echo "FAIL_TEST=$FAIL_TEST" >>test_status.sh
