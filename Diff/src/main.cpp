#include <stdio.h>
#include <assert.h>

#include "Tree.h"
#include "Diff.h"
#include "Common.h"

//--------------------------------------------------------------------------

int main ()
{
    // Инициализируем дерево
    tree_t tree = {};
    node_t* root = TreeInit (&tree);
    if (root == NULL)
        TreeErrCheck$ (&tree, INIT_ERR);

    node_t* node1 = NodeCreate (&tree, NUM, 1, tree.root, NULL, NULL);
    TreeDump (&tree);

    // int primer = GetG ();
    // printf ("primer = %d\n", primer);
}

//--------------------------------------------------------------------------
