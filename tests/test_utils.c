#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <utils/utils.h>

Test(test_malloc_not_null, xmalloc)
{
    void *ptr = xmalloc(10);
    cr_assert_not_null(ptr);
    free(ptr);
}

Test(test_calloc_not_null, xcalloc)
{
    void *ptr = xcalloc(10, sizeof(int));
    cr_assert_not_null(ptr);
    free(ptr);
}

Test(test_realloc_not_null, xrealloc)
{
    void *ptr = xmalloc(10);
    void *new_ptr = xrealloc(ptr, 20);
    cr_assert_not_null(new_ptr);
    free(new_ptr);
}

Test(test_hash, hash)
{
    cr_assert_eq(hash("test"), 72);
}

Test(test_create_variable, create_variable)
{
    struct Variable *var = create_variable("test", "value");
    cr_assert_str_eq(var->name, "test");
    cr_assert_str_eq(var->value, "value");
    free_variable(var);
}

Test(test_free_variable, free_variable)
{
    struct Variable *var = create_variable("test", "value");
    free_variable(var);
    cr_assert_not_null(var);
}

Test(test_insert, insert)
{
    insertVariable("test", "value");
    struct Variable var = getVariable("test");
    cr_assert_str_eq(var.name, "test");
    cr_assert_str_eq(var.value, "value");
    free_hash_map();
}

Test(test_get, getVariable)
{
    insertVariable("test", "value");
    struct Variable var = getVariable("test");
    cr_assert_str_eq(var.name, "test");
    cr_assert_str_eq(var.value, "value");
    free_hash_map();
}

Test(test_update, updateVariable)
{
    insertVariable("test", "value");
    updateVariable("test", "new_value");
    struct Variable var = getVariable("test");
    cr_assert_str_eq(var.name, "test");
    cr_assert_str_eq(var.value, "new_value");
    free_hash_map();
}
