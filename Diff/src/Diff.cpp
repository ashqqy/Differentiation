
#include "CustomAssert.h"
#include "Tree.h"
#include "Diff.h"

//--------------------------------------------------------------------------

#define _DIFF_L Diff (node->left)
#define _DIFF_R Diff (node->right)
#define _COPY_L SubtreeCopy (node->left)
#define _COPY_R SubtreeCopy (node->right)

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
        case CONST:
            return _NUM (0);

        case VAR:
            return _NUM (1);

        case OP:
        {
            switch (node->data.content.operation)
            {
                case ADD: 
                    return _ADD (_DIFF_L, _DIFF_R);

                case SUB:
                    return _SUB (_DIFF_L, _DIFF_R);

                case MUL:
                    return _ADD (_MUL(_DIFF_L, _COPY_R), 
                                 _MUL(_DIFF_R, _COPY_L));
 
                case DIV:
                    return _DIV (_SUB (_MUL (_DIFF_L, _COPY_R), 
                           _MUL (_DIFF_R, _COPY_L)), _MUL (_COPY_R, _COPY_R));

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
