#ifndef PARCE_H
#define PARCE_H

#include "Tree.h"

//--------------------------------------------------------------------------

tree_node_t* GetG (tree_node_t** token_array, int* shift);
tree_node_t* GetPlus (tree_node_t** token_array, int* shift);
tree_node_t* GetMult (tree_node_t** token_array, int* shift);
tree_node_t* GetDegree (tree_node_t** token_array, int* shift);
tree_node_t* GetFunc (tree_node_t** token_array, int* shift);
tree_node_t* GetBracket (tree_node_t** token_array, int* shift);
tree_node_t* GetConst (tree_node_t** token_array, int* shift);
tree_node_t* GetVariable (tree_node_t** token_array, int* shift);
tree_node_t* GetNumber (tree_node_t** token_array, int* shift);

//--------------------------------------------------------------------------

#endif //PARCE_H