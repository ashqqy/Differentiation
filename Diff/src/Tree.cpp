#include <stdlib.h>
#include <assert.h>

#include "Tree.h"
#include "Stack.h"
#include "Common.h"
    
//--------------------------------------------------------------------------

node_t* TreeInit (tree_t* tree, type_t type, node_data_t data)
{
    if (tree == NULL)
        return NULL;
    
    node_t* root = (node_t*) calloc (1, sizeof(node_t));

    if (root != NULL)
    {
        tree->n_nodes = 1;
        tree->root    = root;
        root->type    = type;
        root->data    = data;
    }

    TREE_VERIFY(tree);

    return root;
}

node_t* NodeCreate (tree_t* tree, type_t type, node_data_t data, node_t* parent, node_t* left, node_t* right)
{
    TREE_VERIFY(tree);

    node_t* node = (node_t*) calloc (1, sizeof(node_t));
    if (node == NULL)
        return NULL;

    tree->n_nodes += 1;
    node->parent  = parent;
    node->type    = type;
    node->data    = data;
    node->left    = left;
    node->right   = right;

    TREE_VERIFY(tree);

    return node;
}

// DANGER FUNC
// node_t* NodeEdit (node_t* node, type_t new_type, node_data_t new_data, node_t* new_parent, node_t* new_left, node_t* new_right)
// {
//     if (node == NULL)
//         return NULL;
    
//     node->parent = new_parent;
//     node->type   = new_type;
//     node->data   = new_data;
//     node->left   = new_left;
//     node->right  = new_right;

//     return node;
// }

node_t* NodeDataEdit (tree_t* tree, node_t* node, type_t new_type, node_data_t new_data)
{
    TREE_VERIFY(tree);

    if (node == NULL)
        return NULL;
    
    node->type = new_type;
    node->data = new_data;

    TREE_VERIFY(tree);

    return node;
}

//--------------------------------------------------------------------------

void TreeVerify (tree_t* tree)
{
    tree_error_t tree_err = TREE_OK;
    if (tree->root == NULL) 
    {
        tree_err = UNINITIALIZED_TREE;
    }

    NodeVerify (tree->root, &tree_err);

    TreeErrCheck$ (tree, tree_err);
}

tree_error_t NodeVerify (node_t* node, tree_error_t* tree_err)
{
    if (*tree_err    != TREE_OK)                                    return *tree_err;
    if (node         == NULL)                                       return *tree_err;
    if (node->parent == NULL)                                       return NODE_WITHOUT_PARENT;
    if (node->parent->left != node && node->parent->right != node)  return NODE_BAD_PARENT;
    if (node->right  != NULL)
        if (node->right->parent != node)                            return NODE_BAD_RCHILD;
    if (node->left   != NULL)
        if (node->left->parent  != node)                            return NODE_BAD_LCHILD;

    *tree_err = NodeVerify (node->left,  tree_err);
    *tree_err = NodeVerify (node->right, tree_err);

    return *tree_err;
}

const char* TreeErrorDescr (tree_error_t tree_err)
{
    #define DESCR$_(tree_err) case tree_err: return #tree_err;

    switch (tree_err)
    {
        DESCR$_ (TREE_OK);
        DESCR$_ (INIT_ERR);
        DESCR$_ (UNINITIALIZED_TREE);
        DESCR$_ (NODE_NULL_PTR);
        DESCR$_ (NODE_WITHOUT_PARENT);
        DESCR$_ (NODE_BAD_PARENT);
        DESCR$_ (NODE_BAD_RCHILD);
        DESCR$_ (NODE_BAD_LCHILD);
        default: return "Unknown error";
    }

    #undef DESCR$_
}

//--------------------------------------------------------------------------

void TreeDump (tree_t* tree)
{
    assert (tree != NULL);

    FILE* dump_file = fopen ("./Dump/dump.dot", "w");

    fprintf (dump_file, "digraph G\n");
    fprintf (dump_file, "{\n");
    fprintf (dump_file, "node[shape=\"record\", style=\"rounded, filled\"];\n\n");

    // определяем узлы 
    TreeNodeDescrDump (dump_file, tree->root);
    fprintf (dump_file, "\n");

    // соединяем узлы
    TreeNodeLinkDump  (dump_file, tree->root);

    fprintf (dump_file, "}\n");

    fclose (dump_file);

    const char* command = "dot ./dump/dump.dot -Tpng -o ./dump/dump.png";
    // const char command[81] = "\"C:/Program Files/Graphviz/bin/dot.exe\" ./dump/dump.dot -Tpng -o ./dump/dump.png";
    system(command);
}

void TreeNodeDescrDump (FILE* dump_file, node_t* node)
{
    assert (dump_file != NULL);

    if (node == NULL)
        return;

    fprintf (dump_file, "p%p[label = \"{ <par>parent = %p | <ptr>ptr = %p | <d>%d| { <l>left = %p|<r>right = %p} }\"];\n", 
                                                           node, node->parent, node, node->data, node->left, node->right);
    
    TreeNodeDescrDump (dump_file, node->left);
    TreeNodeDescrDump (dump_file, node->right);

    return;
}

void TreeNodeLinkDump (FILE* dump_file, node_t* node)
{   
    assert (dump_file != NULL);

    if (node->left  != NULL) 
    {
        fprintf (dump_file, "p%p:<l> -> p%p\n", node, node->left);
        TreeNodeLinkDump (dump_file, node->left);
    }
    if (node->right != NULL) 
    {
        fprintf (dump_file, "p%p:<r> -> p%p\n", node, node->right);
        TreeNodeLinkDump (dump_file, node->right);
    }

    return;
}

//--------------------------------------------------------------------------
