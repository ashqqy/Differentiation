#include <math.h>

#include "CustomAssert.h"
#include "Tree.h"
#include "Simplifier.h"

#define left_(struct_field) node->left->data.struct_field
#define right_(struct_field) node->right->data.struct_field

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
            if ((node->data.content.function == LN) && (left_ (type) == CONST) 
                && (left_ (content.constant) == EXP))
                NumOp (node, 1);
            else
                Simplify (node->left);
            return;
        }

        case OP:
        {
            OperationSimplify (node);
            return;
        }

        case SP_SYMB:
        default: 
            return;
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

void OperationSimplify (tree_node_t* node)
{
    CustomAssert (node != NULL);

    switch (node->data.content.operation)
    {
        case ADD:
        {
            SimplifyCaseAdd (node);
            break;
        }
        case SUB:
        {   
            SimplifyCaseSub (node);                
            break;
        }
        case MUL:
        {
            SimplifyCaseMul (node);                
            break;
        }
        case DIV:
        {
            SimplifyCaseDiv (node);
            break;
        }
        case DEG:
        {
            SimplifyCaseDeg (node);
            break;
        }
        default:
            break;
    }
}

void SimplifyCaseAdd (tree_node_t* node)
{
    CustomAssert (node != NULL);

    // число+число
    if ((left_ (type) == NUM) && (right_ (type) == NUM))
        NumOp (node, left_ (content.number) + right_ (content.number));
    // выражение + ноль
    else if ((left_ (type) != NUM) && (right_ (type) == NUM) && 
             (CompareDoubles(right_ (content.number), 0) == 0))
    {
        ZeroAddSub (node, node->left);
        Simplify (node);
    }
    // ноль + выражение
    else if ((right_ (type) != NUM) && (left_ (type) == NUM) && 
             (CompareDoubles (left_ (content.number), 0) == 0))
    {
        ZeroAddSub (node, node->right);
        Simplify (node);
    }
}

void SimplifyCaseSub (tree_node_t* node)
{
    CustomAssert (node);

    // число-число
    if ((left_ (type) == NUM) && right_ (type) == NUM)
        NumOp (node, left_ (content.number) - right_ (content.number));
    // выражение - 0
    else if ((left_ (type) != NUM) && (right_ (type) == NUM) && 
             (CompareDoubles(right_ (content.number), 0) == 0))
    {
        ZeroAddSub (node, node->left);
        Simplify (node);
    }
    // 0 - выражение
    else if ((right_ (type) != NUM) && (left_ (type) == NUM) && 
        (CompareDoubles (left_ (content.number), 0) == 0))
    {
        node->data.content.operation = MUL;
        left_ (content.number) = -1;
        Simplify (node->right);
    }
}
void SimplifyCaseMul (tree_node_t* node)
{
    CustomAssert (node != NULL);

    // число*число
    if ((left_ (type) == NUM) && (right_ (type) == NUM))
        NumOp (node, left_ (content.number) * right_ (content.number));
    // выражение * 0
    else if ((left_ (type) != NUM) && (right_ (type) == NUM) && 
             (CompareDoubles(right_ (content.number), 0) == 0))
    {
        ZeroMulDiv (node, node->right);
        Simplify (node);
    }
    // 0 * выражение
    else if ((right_ (type) != NUM) && (left_ (type) == NUM) && 
        (CompareDoubles (left_ (content.number), 0) == 0))
    {
        ZeroMulDiv (node, node->left);
        Simplify (node);
    }
    // выражение * 1
    else if ((left_ (type) != NUM) && (right_ (type) == NUM) && 
             (CompareDoubles(right_ (content.number), 1) == 0))
    {
        OneMulDivDeg (node, node->left);
        Simplify (node);
    }
    // 1 * выражение
    else if ((right_ (type) != NUM) && (left_ (type) == NUM) && 
        (CompareDoubles (left_ (content.number), 1) == 0))
    {
        OneMulDivDeg (node, node->right);
        Simplify (node);
    }
}

void SimplifyCaseDiv (tree_node_t* node)
{
    CustomAssert (node != NULL);

    // число / число
    if ((left_ (type) == NUM) && (right_ (type) == NUM))
        NumOp (node, left_ (content.number) / right_ (content.number));
    // 0 / выражение
    else if ((right_ (type) != NUM) && (left_ (type) == NUM) && 
        (CompareDoubles (left_ (content.number), 0) == 0))
    {
        ZeroMulDiv (node, node->left);
        Simplify (node);
    }
    // выражение / 1
    else if ((left_ (type) != NUM) && (right_ (type) == NUM) && 
             (CompareDoubles(right_ (content.number), 1) == 0))
    {
        OneMulDivDeg (node, node->left);
        Simplify (node);
    }
}

void SimplifyCaseDeg (tree_node_t* node)
{
    CustomAssert (node != NULL);

    Simplify (node->right);

    // число ^ число
    if ((left_ (type) == NUM) && (right_ (type) == NUM))
        NumOp (node, pow (left_ (content.number), right_ (content.number)));
    // 1 ^ выражение
    else if ((right_ (type) != NUM) && (left_ (type) == NUM) && 
        (CompareDoubles (left_ (content.number), 1) == 0))
        NumOp (node, 1);
    // выражение ^ 1
    else if ((left_ (type) != NUM) && (right_ (type) == NUM) && 
        (CompareDoubles (right_ (content.number), 1) == 0))
    {
        OneMulDivDeg (node, node->left);
        Simplify (node);
    }
}
//--------------------------------------------------------------------------

#undef left_
#undef right_

//--------------------------------------------------------------------------

