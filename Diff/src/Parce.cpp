#include <stdlib.h>
#include <assert.h>

#include "CustomAssert.h"
#include "Tree.h"
#include "Parce.h"
#include "Tokenization.h"

//--------------------------------------------------------------------------

#define token_data_(struct_field) token_array[*shift]->data.struct_field

//--------------------------------------------------------------------------

tree_node_t* GetG (tree_node_t** token_array, int* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* root_node = GetPlus (token_array, shift);

    if ((token_data_ (type) != SP_SYMB) || (token_data_ (content.special_symb) != EXPRESSION_END))
        SyntaxError ("GetG");

    free (token_array[*shift]); token_array[*shift] = NULL;
    *shift += 1;
    return root_node;
}

tree_node_t* GetPlus (tree_node_t** token_array, int* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* first_node = GetMult (token_array, shift);
    while ((token_data_ (type) == OP) && (token_data_ (content.operation) == ADD || token_data_ (content.operation) == SUB))
    {   
        tree_node_t* parent_node = token_array[*shift];

        *shift += 1;
        tree_node_t* second_node = GetMult (token_array, shift);
        
        NodeLink (first_node, &parent_node->left);
        NodeLink (second_node, &parent_node->right);

        first_node = parent_node;
    }
    return first_node;
}

tree_node_t* GetMult (tree_node_t** token_array, int* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* first_node = GetDegree (token_array, shift);

    while ((token_data_ (type) == OP) && (token_data_ (content.operation) == MUL || 
            token_data_ (content.operation) == DIV))
    {
        tree_node_t* parent_node = token_array[*shift];

        *shift += 1;
        tree_node_t* second_node = GetDegree (token_array, shift);

        NodeLink (first_node,  &parent_node->left);
        NodeLink (second_node, &parent_node->right);
        
        first_node = parent_node;
    }
    return first_node;
}

tree_node_t* GetDegree (tree_node_t** token_array, int* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* first_node = GetFunc (token_array, shift);

    while ((token_data_ (type) == OP) && (token_data_ (content.operation) == DEG))
    {
        tree_node_t* parent_node = token_array[*shift];

        *shift += 1;
        tree_node_t* second_node = GetFunc (token_array, shift);

        NodeLink (first_node,  &parent_node->left);
        NodeLink (second_node, &parent_node->right);

        first_node = parent_node;
    }

    return first_node;
}

tree_node_t* GetFunc (tree_node_t** token_array, int* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    if (token_data_ (type) == FUNC)
    {
        tree_node_t* parent_node = token_array[*shift];
        *shift += 1;

        if ((token_data_ (type) == SP_SYMB) && 
            (token_data_ (content.special_symb) == BRACKET_OP))
        {
            tree_node_t* func_arg = GetBracket (token_array, shift);
            NodeLink (func_arg, &parent_node->left);
        }
        else
            SyntaxError ("func without open bracket");
        
        return parent_node;
    }

    return GetBracket (token_array, shift);
}

tree_node_t* GetBracket (tree_node_t** token_array, int* const shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    if ((token_data_ (type) == SP_SYMB) && 
       (token_data_ (content.special_symb) == BRACKET_OP))
    {
        free (token_array[*shift]); token_array[*shift] = NULL;

        (*shift)++; // почему нельзя ++
        tree_node_t* node = GetPlus (token_array, shift);
        if ((token_data_ (type) != SP_SYMB) || 
            (token_data_ (content.special_symb) != BRACKET_CL))
            SyntaxError ("GetBracket"); // TODO

        free (token_array[*shift]); token_array[*shift] = NULL;

        *shift += 1;
        return node;
    }

    else if (token_data_ (type) == VAR)
        return GetVariable (token_array, shift);
    else if (token_data_ (type) == CONST)
        return GetConst (token_array, shift);
    else
        return GetNumber (token_array, shift);
}

tree_node_t* GetConst (tree_node_t** token_array, int* shift)

{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);  

    tree_node_t* node = NULL;
    if (token_data_ (type) == CONST)
        node = token_array[*shift];
    else 
        SyntaxError ("GetConst");

    *shift += 1;

    return node;
}

tree_node_t* GetVariable (tree_node_t** token_array, int* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* node = NULL;
    if (token_data_ (type) == VAR)
        node = token_array[*shift];
    else 
        SyntaxError ("GetVariable");

    *shift += 1;

    return node;
}

tree_node_t* GetNumber (tree_node_t** token_array, int* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* node = NULL;
    if (token_data_ (type) == NUM)
        node = token_array[*shift];
    else
        SyntaxError ("GetNumber");

    *shift += 1;

    return node;
}

//--------------------------------------------------------------------------

#undef token_data_

//--------------------------------------------------------------------------
