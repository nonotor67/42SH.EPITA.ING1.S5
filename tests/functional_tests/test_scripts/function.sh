
foo() {
  bar() {
    echo foobar
  }
}
# defines bar
foo
# prints foobar
bar

# Redefine bar
bar() {
  echo bar
}

big_function() {
  echo big_function
  small_function() {
    echo $1 $2
  }
  if $1; then
    small_function $1 connard
  else
    big_function true
  fi
  for i in 1 2 3; do
    echo $i
  done
}

big_function false
