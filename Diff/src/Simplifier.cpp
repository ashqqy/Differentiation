#include <math.h>

#include "CustomAssert.h"
#include "Tree.h"
#include "Simplifier.h"

void NumOp(tree_node_t* op_node, double expression)
{
    NodeEditData (op_node, tree_data_t {.content = {.number = expression}, .type = NUM});
    TreeDestroy (op_node->left);
    TreeDestroy (op_node->right);
    op_node->left = NULL;
    op_node->right = NULL;
}

void ZeroAddSub (tree_node_t* op_node, tree_node_t* non_zero_node)
{
    NodeEditData (op_node, non_zero_node->data);
    tree_node_t* node_to_free_1 = op_node->left;
    tree_node_t* node_to_free_2 = op_node->right;
    op_node->left  = non_zero_node->left;
    op_node->right = non_zero_node->right;
    free (node_to_free_1);
    free (node_to_free_2);
}

void OneMulDivDeg (tree_node_t* op_node, tree_node_t* non_one_node)
{
    NodeEditData (op_node, non_one_node->data);
    tree_node_t* node_to_free_1 = op_node->left;
    tree_node_t* node_to_free_2 = op_node->right;
    op_node->left  = non_one_node->left;
    op_node->right = non_one_node->right;
    free (node_to_free_1);
    free (node_to_free_2);
}

void ZeroMulDiv (tree_node_t* op_node, tree_node_t* zero_node)
{
    NodeEditData (op_node, zero_node->data);
    TreeDestroy (op_node->left);
    TreeDestroy (op_node->right);
    op_node->left  = NULL;
    op_node->right = NULL;
}


//--------------------------------------------------------------------------

void Simplify (tree_node_t* node)
{
    if (node == NULL)
        return;    

    Simplify (node->left);
    Simplify (node->right);

    switch (node->data.type)
    {
        case NUM:
            return;

        case CONST:
            return;
        
        case VAR:
            return;

        case FUNC:
        {
            if ((node->data.content.function == LN) && (node->left->data.type == CONST) 
                && (node->left->data.content.constant == EXP))
                NumOp (node, 1);
            else
                Simplify (node->left);
            return;
        }

        case OP:
        {
            switch (node->data.content.operation)
            {
                case ADD:
                {
                    // число+число
                    if ((node->left->data.type == NUM) && (node->right->data.type == NUM))
                        NumOp (node, node->left->data.content.number + node->right->data.content.number);

                    // выражение + ноль
                    else if ((node->left->data.type != NUM) && (node->right->data.type == NUM) && 
                             (CompareDoubles(node->right->data.content.number, 0) == 0))
                    {
                        ZeroAddSub (node, node->left);
                        Simplify (node);
                    }

                    // ноль + выражение
                    else if ((node->right->data.type != NUM) && (node->left->data.type == NUM) && 
                             (CompareDoubles (node->left->data.content.number, 0) == 0))
                    {
                        ZeroAddSub (node, node->right);
                        Simplify (node);
                    }

                    break;
                }

                case SUB:
                {
                    // число-число
                    if ((node->left->data.type == NUM) && node->right->data.type == NUM)
                        NumOp (node, node->left->data.content.number - node->right->data.content.number);

                    // выражение - 0
                    else if ((node->left->data.type != NUM) && (node->right->data.type == NUM) && 
                             (CompareDoubles(node->right->data.content.number, 0) == 0))
                    {
                        ZeroAddSub (node, node->left);
                        Simplify (node);
                    }

                    // 0 - выражение
                    else if ((node->right->data.type != NUM) && (node->left->data.type == NUM) && 
                        (CompareDoubles (node->left->data.content.number, 0) == 0))
                    {
                        node->data.content.operation = MUL;
                        node->left->data.content.number = -1;
                        Simplify (node->right);
                    }
                        
                    break;
                }

                case MUL:
                {
                    // число*число
                    if ((node->left->data.type == NUM) && (node->right->data.type == NUM))
                        NumOp (node, node->left->data.content.number * node->right->data.content.number);

                    // выражение * 0
                    else if ((node->left->data.type != NUM) && (node->right->data.type == NUM) && 
                             (CompareDoubles(node->right->data.content.number, 0) == 0))
                    {
                        ZeroMulDiv (node, node->right);
                        Simplify (node);
                    }

                    // 0 * выражение
                    else if ((node->right->data.type != NUM) && (node->left->data.type == NUM) && 
                        (CompareDoubles (node->left->data.content.number, 0) == 0))
                    {
                        ZeroMulDiv (node, node->left);
                        Simplify (node);
                    }

                    // выражение * 1
                    else if ((node->left->data.type != NUM) && (node->right->data.type == NUM) && 
                             (CompareDoubles(node->right->data.content.number, 1) == 0))
                    {
                        OneMulDivDeg (node, node->left);
                        Simplify (node);
                    }

                    // 1 * выражение
                    else if ((node->right->data.type != NUM) && (node->left->data.type == NUM) && 
                        (CompareDoubles (node->left->data.content.number, 1) == 0))
                    {
                        OneMulDivDeg (node, node->right);
                        Simplify (node);
                    }
                        
                    break;
                }
                case DIV:
                {
                    // число / число
                    if ((node->left->data.type == NUM) && (node->right->data.type == NUM))
                        NumOp (node, node->left->data.content.number / node->right->data.content.number);

                    // 0 / выражение
                    else if ((node->right->data.type != NUM) && (node->left->data.type == NUM) && 
                        (CompareDoubles (node->left->data.content.number, 0) == 0))
                    {
                        ZeroMulDiv (node, node->left);
                        Simplify (node);
                    }

                    // выражение / 1
                    else if ((node->left->data.type != NUM) && (node->right->data.type == NUM) && 
                             (CompareDoubles(node->right->data.content.number, 1) == 0))
                    {
                        OneMulDivDeg (node, node->left);
                        Simplify (node);
                    }

                    break;
                }
                case DEG:
                {
                    // число ^ число
                    if ((node->left->data.type == NUM) && (node->right->data.type == NUM))
                        NumOp (node, pow (node->left->data.content.number, node->right->data.content.number));

                    // 1 ^ выражение
                    else if ((node->right->data.type != NUM) && (node->left->data.type == NUM) && 
                        (CompareDoubles (node->left->data.content.number, 1) == 0))
                        NumOp (node, 1);

                    // выражение ^ 1
                    else if ((node->left->data.type != NUM) && (node->right->data.type == NUM) && 
                        (CompareDoubles (node->right->data.content.number, 1) == 0))
                    {
                        OneMulDivDeg (node, node->left);
                        Simplify (node);
                    }
                }
                default:
                    break;
            }
        }

        case SP_SYMB:
        default: 
            break;
    }
}

//--------------------------------------------------------------------------

int CompareDoubles (double num1, double num2)
{
    if (abs (num1 - num2) < delta)
        return 0;
    else 
        return -1;
}

//--------------------------------------------------------------------------
