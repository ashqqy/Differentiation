#ifndef PARCE_H
#define PARCE_H

#include "Tree.h"

//--------------------------------------------------------------------------

tree_node_t* GetG ();
tree_node_t* GetPlus ();
tree_node_t* GetMult ();
tree_node_t* GetBracket ();
tree_node_t* GetNumber ();
tree_node_t* GetVariable ();
void SyntaxError (char symb);

//--------------------------------------------------------------------------

#endif //PARCE_H