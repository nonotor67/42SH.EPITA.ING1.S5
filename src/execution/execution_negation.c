#include "execution_negation.h"

int execute_negation(struct ast *node)
{
    return !execution(node->left);
}
