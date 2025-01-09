
RESULT=$(./src/42sh -c "echo -n 'Hello World!'")
if [ "$RESULT" = "Hello World!" ]; then
    echo "Functional test passed"
else
    echo "Functional test failed"
fi