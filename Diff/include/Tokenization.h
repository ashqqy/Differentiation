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
    {.name = "VelikayaConstantaRavnayaEdinice",  .data = {.content = {.constant = A},  .type = CONST}},
    {.name = "SmallPenis",  .data = {.content = {.constant = B},  .type = CONST}},
    {.name = "BigBebra",  .data = {.content = {.constant = C},  .type = CONST}},
    {.name = "NolbTolbkoBukvami",  .data = {.content = {.constant = D},  .type = CONST}},
    {.name = "eVstepeniPI",  .data = {.content = {.constant = E},  .type = CONST}},
    {.name = "maloy",  .data = {.content = {.constant = F},  .type = CONST}},
    {.name = "bolshoy",  .data = {.content = {.constant = G},  .type = CONST}},
    {.name = "sredniy",  .data = {.content = {.constant = H},  .type = CONST}},
    {.name = "UniqueConstant",  .data = {.content = {.constant = I},  .type = CONST}},
    {.name = "AYAYAYA",  .data = {.content = {.constant = J},  .type = CONST}},
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