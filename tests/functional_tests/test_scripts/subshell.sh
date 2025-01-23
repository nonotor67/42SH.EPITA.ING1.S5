
# Test subshell on multiple lines
(
    echo hello
)

a=42
(
    echo hello
    echo world
    if true; then
        echo $a
        a=43
    fi
    echo $a
)
echo $a

(
    coucou() {
        echo hello
    }
    coucou
)
coucou

# Test subshell with variable substitution
a=sh
(
    a=42
    echo -n $a
)
echo $a

# Test subshell in subshell
(
    echo hello
    (
        echo world
    )
)

