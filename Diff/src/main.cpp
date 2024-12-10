#include <stdio.h>
#include <assert.h>

#include "Tree.h"
#include "Parce.h"
#include "Diff.h"
#include "Common.h"

//--------------------------------------------------------------------------

int main ()
{
    tree_node_t* root_node = GetG ();
    tree_node_t* diff_root_node = Diff (root_node);
    TreeDump (diff_root_node);
}

//--------------------------------------------------------------------------
