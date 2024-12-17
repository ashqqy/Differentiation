#ifndef SIMPLIFIER_H
#define SIMPLIFIER_H

#include <math.h>

const double delta = pow (10, -7);

int CompareDoubles (double num1, double num2);
void Simplify (tree_node_t* node);

void NumOp(tree_node_t* op_node, double expression);
void ZeroAddSub (tree_node_t* op_node, tree_node_t* non_zero_node);
void OneMulDivDeg (tree_node_t* op_node, tree_node_t* non_one_node);
void ZeroMulDiv (tree_node_t* op_node, tree_node_t* zero_node);


#endif // SIMPLIFIER_H