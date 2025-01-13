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

Test(test_create_hash_table, create_hash_table)
{
    struct HashMap *ht = create_hash_table();
    cr_assert_not_null(ht);
    free_hash_map(ht);
}

Test(test_insert, insert)
{
    struct HashMap *ht = create_hash_table();
    insertVariable(ht, "test", "value");
    struct Variable var = getVariable(ht, "test");
    cr_assert_str_eq(var.name, "test");
    cr_assert_str_eq(var.value, "value");
    free_hash_map(ht);
}

Test(test_remove, removeVariable)
{
    struct HashMap *ht = create_hash_table();
    insertVariable(ht, "test", "value");
    removeVariable(ht, "test");
    struct Variable var = getVariable(ht, "test");
    cr_assert_null(var.name);
    cr_assert_null(var.value);
    free_hash_map(ht);
}

Test(test_get, getVariable)
{
    struct HashMap *ht = create_hash_table();
    insertVariable(ht, "test", "value");
    struct Variable var = getVariable(ht, "test");
    cr_assert_str_eq(var.name, "test");
    cr_assert_str_eq(var.value, "value");
    free_hash_map(ht);
}

Test(test_update, updateVariable)
{
    struct HashMap *ht = create_hash_table();
    insertVariable(ht, "test", "value");
    updateVariable(ht, "test", "new_value");
    struct Variable var = getVariable(ht, "test");
    cr_assert_str_eq(var.name, "test");
    cr_assert_str_eq(var.value, "new_value");
    free_hash_map(ht);
}
