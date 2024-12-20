#ifndef TREE_H
#define TREE_H

#include <stdio.h>
    
//--------------------------------------------------------------------------

enum tree_data_type_t
{
    NUM     = 1,
    CONST   = 2,
    VAR     = 3,
    OP      = 4,
    FUNC    = 5,
    SP_SYMB = 6
};

enum special_symb_t
{
    EXPRESSION_END = 39,
    BRACKET_OP     = 40,
    BRACKET_CL     = 41
};

enum math_constant_t
{
    EXP = 1,
    PI  = 2,
};

enum math_function_t
{
    SIN = 1,
    COS = 2,
    TG  = 3,
    CTG = 4,
    LN  = 5
};

enum operation_t
{
    ADD = 43,
    SUB = 45,
    MUL = 42,
    DIV = 47,
    DEG = 94,
};

struct tree_data_t
{
    union
    {
        char variable;
        double number;
        math_constant_t constant;
        math_function_t function;
        operation_t operation;
        special_symb_t special_symb;
    } content;

    tree_data_type_t type;
};

struct tree_node_t
{
    tree_data_t data;
    tree_node_t* left;
    tree_node_t* right;
};

//--------------------------------------------------------------------------

tree_node_t* NodeCreate (tree_data_t data, tree_node_t* left_node = NULL, tree_node_t* right_node = NULL);
tree_node_t* NodeLink (tree_node_t* node, tree_node_t** node_to_link_to);
tree_node_t* NodeEditData (tree_node_t* node, tree_data_t new_data);
tree_node_t* SubtreeCopy (tree_node_t* node);
void TreeDestroy (tree_node_t* node);

void TreeDump (tree_node_t* root_node);
void TreeNodeDescrDump (FILE* dump_file, tree_node_t* node);
void TreeNodeLinkDump (FILE* dump_file, tree_node_t* node);

const char* DescribeMathFunc (math_function_t func_enum);
const char* DescribeMathConst (math_constant_t const_enum);


//--------------------------------------------------------------------------

#endif //TREE_H
