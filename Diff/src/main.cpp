#include <stdio.h>
#include <assert.h>

#include "Tree.h"
#include "Diff.h"
#include "Common.h"

//--------------------------------------------------------------------------

int main ()
{
    tree_node_t* root_node = GetG ();
    TreeDump (root_node);
}

//--------------------------------------------------------------------------
