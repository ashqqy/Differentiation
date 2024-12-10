#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "Common.h"
#include "CustomAssert.h"
#include "Tree.h"

//------------------------------------------------------

tree_node_t* NodeCreate (tree_data_t data)
{
    tree_node_t* node = (tree_node_t*) calloc (1, sizeof (tree_node_t));
    if (node == NULL)
        return NULL;

    node->data   = data;
    node->left   = NULL;
    node->right  = NULL;

    return node;
}
//------------------------------------------------------

tree_node_t* NodeLink (tree_node_t* node, tree_node_t** node_to_link_to)
{
    assert (node            != NULL);
    assert (node_to_link_to != NULL);

    *node_to_link_to = node;

    return node;
}

//------------------------------------------------------

tree_node_t* NodeEditData (tree_node_t* node, tree_data_t new_data)
{
    assert (node != NULL);

    node->data = new_data;

    return node;
}

//------------------------------------------------------

void TreeDestroy (tree_node_t* node)
{
    if (node == NULL)
        return;

    TreeDestroy (node->left);
    TreeDestroy (node->right);

    free (node); node = NULL;
}

//------------------------------------------------------

void TreeDump (tree_node_t* root_node)
{
    assert (root_node != NULL);

    FILE* dump_file = fopen ("./Dump/dump.dot", "w");

    fprintf (dump_file, "digraph G\n");
    fprintf (dump_file, "{\n");
    fprintf (dump_file, "node[shape=\"record\", style=\"rounded, filled\"];\n\n");

    // определяем узлы 
    if (root_node != NULL)
        TreeNodeDescrDump (dump_file, root_node);
    fprintf (dump_file, "\n");

    // соединяем узлы
    if (root_node != NULL)
        TreeNodeLinkDump (dump_file, root_node);

    fprintf (dump_file, "}\n");

    fclose (dump_file);

    const char command[81] = "dot ./dump/dump.dot -Tpng -o ./dump/dump.png"; // linux
    // const char command[81] = "\"C:/Program Files/Graphviz/bin/dot.exe\" ./dump/dump.dot -Tpng -o ./dump/dump.png"; // windows
    system(command);
}

void TreeNodeDescrDump (FILE* dump_file, tree_node_t* node)
{
    assert (dump_file != NULL);

    if (node == NULL)
        return;

    if      (node->data.type == NUM)
        fprintf (dump_file, "p%p[label = \"{ <ptr> %p |<type> %s | <data>%lf| { <l>left|<r>right } }\"];\n", 
                            node, node, "NUM", node->data.content.number);
    else if (node->data.type == OP)
        fprintf (dump_file, "p%p[label = \"{ <ptr> %p |<type> %s | <data>%c| { <l>left|<r>right } }\"];\n", 
                            node, node, "OP", node->data.content.operation);
    else if (node->data.type == VAR)
        fprintf (dump_file, "p%p[label = \"{ <ptr> %p |<type> %s | <data>%c| { <l>left|<r>right } }\"];\n", 
                            node, node, "VAR", node->data.content.variable);
    
    if (node->left  != NULL) TreeNodeDescrDump (dump_file, node->left);
    if (node->right != NULL) TreeNodeDescrDump (dump_file, node->right);
}

void TreeNodeLinkDump (FILE* dump_file, tree_node_t* node)
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
}

//------------------------------------------------------