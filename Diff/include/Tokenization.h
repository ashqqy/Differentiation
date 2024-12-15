#ifndef TOKENIZATION_H
#define TOKENIZATION_H

#include "Tree.h"

const int TOKEN_ARRAY_SIZE = 100;

struct reserved_name_t
{
    const char* name;
    tree_data_t data;
};

int FindReservedName (const char* name, tree_data_t* data);
tree_node_t** Tokenization (char* buffer, size_t buffer_size, int* ptr);
void TokenArrayDestroy (tree_node_t** token_array);
[[noreturn]] void SyntaxError (const char* message); // FIXME WTF

#endif // TOKENIZATION_H