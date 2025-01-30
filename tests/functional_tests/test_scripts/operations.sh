
# Simple &&
echo "Hello" && echo "World"

# Simple ||

echo "Hello" || echo "World"

# Test lazy evaluation

echo "Hello" && false || echo "World"
echo "Hello" && true || echo "World"

# Test negation

! true
! false

# Test negation with &&

! true && echo "Hello"
! false && echo "Hello"

# Test negation with ||

! true || echo "Hello"
! false || echo "Hello"

# Test negation in if

if ! false; then
    echo "Hello"
fi

if ! true; then
    echo "Hello"
else
    echo "World"
fi

# Test negation in while

a=false
while ! $a; do
    echo "Hello"
    a=true
done

# Test negation in until

a=true
until ! $a; do
    echo "Hello"
    a=false
done
