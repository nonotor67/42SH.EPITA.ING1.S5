#!/bin/sh

BIN=$1
test="./functional_tests/func_test_run.sh"

$test $BIN "Simple echo test" "echo Hello world"
$test $BIN "Simple echo test with newline" "echo -n Hello world"
$test $BIN "Complex echo test" "echo -eEe \\\\"
$test $BIN "Complex echo test" "echo -ne '\\'"
$test $BIN "Complex echo test" "echo -nE '\\'"
$test $BIN "Complex echo test" "echo -nEe '\\\\'"
$test $BIN "Complex echo test" "echo -nEeeeee '\\\\\\\\\\\'"
$test $BIN "Complex echo test" "echo -eeeeeeEEEE '\\\\'"
$test $BIN "Simple true" "true"
$test $BIN "Simple false" "false"

# Test continue and break outside of loop
$test $BIN "continue outside of loop" "continue 10"
$test $BIN "break outside of loop" "break"

# Test dot builtin

$test $BIN "Simple dot test" ". ./functional_tests/dot_script/simple_test.sh"
$test $BIN "Simple variable test" "a=hello;. ./functional_tests/dot_script/variable_test.sh;echo $a; echo $b"
$test $BIN "Test with error" "cd functional_tests/dot_script/; . simple_test.sh"
$test $BIN "None existing file" ". ./functional_tests/dot_script/bob.sh"

# Test cd
$test $BIN "Simple cd" "cd /;echo $PWD;ls"
$test $BIN "Simple cd with path" "cd /usr;echo $PWD;ls"
# Test error
$test $BIN "Simple cd with error" "cd unexpected_dir;echo $PWD;ls"
# Test with -
$test $BIN "Simple cd with -" "cd /usr;cd -"

# Test exit
$test $BIN "Simple exit" "exit 0"
$test $BIN "Simple exit with code" "exit 1"
$test $BIN "Simple exit with code" "exit 255"
$test $BIN "Simple exit with code" "exit"
$test $BIN "Test with other command" "exit 0;echo Hello"
$test $BIN "Test with other command" "exit 1;echo Hello"
$test $BIN "Test with other command" "false; exit"

export truc=1
$test $BIN "Simple export" "echo \$truc"
unset truc

# Test export
$test $BIN "Simple export" "export truc=1;echo \$truc; unset truc"
$test $BIN "Simple export" "export truc=1;echo \$truc; unset truc; echo \$truc"
$test $BIN "Simple export" "export truc=1;echo \$truc; unset truc; echo \$truc; export truc=2; echo \$truc; unset truc; echo \$truc"
