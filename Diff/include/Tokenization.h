#ifndef TOKENIZATION_H
#define TOKENIZATION_H

#include "Tree.h"

const int TOKEN_ARRAY_SIZE = 100;

struct reserved_name_t
{
    const char* name;
    tree_data_t data;
};

const reserved_name_t reserved_names[] = 
{
    {.name = "e",   .data = {.content = {.constant = EXP}, .type = CONST}},
    {.name = "pi",  .data = {.content = {.constant = PI},  .type = CONST}},
    {.name = "sin", .data = {.content = {.function = SIN}, .type = FUNC}},
    {.name = "cos", .data = {.content = {.function = COS}, .type = FUNC}},
    {.name = "tg",  .data = {.content = {.function = TG},  .type = FUNC}},
    {.name = "ctg", .data = {.content = {.function = CTG}, .type = FUNC}},
    {.name = "ln",  .data = {.content = {.function = LN},  .type = FUNC}},
};

int FindReservedDataByName (const char* name, tree_data_t* data);
const char* FindReservedNameByData (tree_data_t* data);

tree_node_t** Tokenization (char* buffer, size_t buffer_size, int* ptr);
void TokenArrayDestroy (tree_node_t** token_array);
[[noreturn]] void SyntaxError (const char* message);

#endif // TOKENIZATION_H