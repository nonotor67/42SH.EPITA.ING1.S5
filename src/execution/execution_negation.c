#include "execution_negation.h"

int execute_negation(struct ast *node)
{
    return !execute_node(node->left);
}
