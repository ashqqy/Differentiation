#ifndef TREE_H
#define TREE_H

#include <stdio.h>
    
//--------------------------------------------------------------------------

enum tree_data_type_t
{
    NUM = 1,
    VAR = 2,
    OP  = 3
};

enum operations_t
{
    ADD = 43,
    SUB = 45,
    MUL = 42,
    DIV = 47
};

struct tree_data_t
{
    union
    {
        char variable;
        double number;
        operations_t operation;
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

char OperationDescribe (operations_t operation);

tree_node_t* NodeCreate (tree_data_t data);
tree_node_t* NodeLink (tree_node_t* node, tree_node_t** node_to_link_to);
tree_node_t* NodeEditData (tree_node_t* node, tree_data_t new_data);
void TreeDestroy (tree_node_t* node);

void TreeDump (tree_node_t* root_node);
void TreeNodeDescrDump (FILE* dump_file, tree_node_t* node);
void TreeNodeLinkDump (FILE* dump_file, tree_node_t* node);

//--------------------------------------------------------------------------

#endif //TREE_H
