
#include "CustomAssert.h"
#include "Tree.h"
#include "Diff.h"

//--------------------------------------------------------------------------

#define _NUM(num) do            \
{                               \
    value.type           = NUM; \
    value.content.number = num; \
    return NodeCreate (value);  \
} while (0)

#define _ADD(left_node, right_node) do             \
{                                                  \
    value.type              = OP;                  \
    value.content.operation = ADD;                 \
    tree_node_t* parent_node = NodeCreate (value); \
    NodeLink (left_node, &parent_node->left);      \
    NodeLink (right_node, &parent_node->right);    \
    return parent_node;                            \
} while (0)


//--------------------------------------------------------------------------

tree_node_t* Diff (tree_node_t* node)
{
    CustomAssert (node != NULL);
    tree_data_t value;

    if (node->data.type == NUM)
        _NUM (0);

    else if (node->data.type == VAR)
        _NUM (1);

    else if (node->data.type == OP)
    {
        switch (node->data.content.operation)
        {
            case ADD: 
                _ADD (Diff (node->left), Diff (node->right));
            
            case SUB:
            case MUL:
            case DIV:
            default:
                break;
        }
    }

    return NULL;
}

//--------------------------------------------------------------------------
