
#include "CustomAssert.h"
#include "Tree.h"
#include "Diff.h"

//--------------------------------------------------------------------------

#define _NUM(num) \
    NodeCreate (tree_data_t {.content = {.number = num}, .type = NUM})

#define _ADD(left_node, right_node) \
    NodeCreate (tree_data_t {.content = {.operation = ADD}, .type = OP}, left_node, right_node)

#define _SUB(left_node, right_node) \
    NodeCreate (tree_data_t {.content = {.operation = SUB}, .type = OP}, left_node, right_node)

#define _MUL(left_node, right_node) \
    NodeCreate (tree_data_t {.content = {.operation = MUL}, .type = OP}, left_node, right_node)

#define _DIV(left_node, right_node) \
    NodeCreate (tree_data_t {.content = {.operation = DIV}, .type = OP}, left_node, right_node)

//--------------------------------------------------------------------------

tree_node_t* Diff (tree_node_t* node)
{
    CustomAssert (node != NULL);

    switch (node->data.type)
    {
        case NUM:
            return _NUM (0);

        case VAR:
            return _NUM (1);

        case OP:
        {
            switch (node->data.content.operation)
            {
                case ADD: 
                    return _ADD (Diff (node->left), Diff (node->right));

                case SUB:
                    return _SUB (Diff (node->left), Diff (node->right));

                case MUL:
                    return _ADD (_MUL(Diff (node->left), SubtreeCopy (node->right)), 
                                 _MUL(Diff (node->right), SubtreeCopy (node->left)));
 
                case DIV:
                    return _DIV (_SUB (_MUL (Diff (node->left), SubtreeCopy (node->right)), 
                                 _MUL (Diff (node->right), SubtreeCopy (node->left))), 
                                 _MUL (SubtreeCopy (node->right), SubtreeCopy (node->right)));

                default:
                    break;
            }
        }

        default:
            break;
    }

    return NULL;
}

//--------------------------------------------------------------------------
