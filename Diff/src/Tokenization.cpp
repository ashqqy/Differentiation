#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Tokenization.h"
#include "CustomAssert.h"
#include "MyAllocation.h"

#define _TOKEN_INIT(data_type, union_type, union_data)                                \
    token_array[token_array_shift] = (tree_node_t*) calloc (1, sizeof (tree_node_t)); \
    token_array[token_array_shift]->data.type = data_type;                            \
    token_array[token_array_shift]->data.content.union_type = union_data;             \
    token_array_shift += 1


//--------------------------------------------------------------------------

reserved_name_t reserved_names[] = 
{
    {.name = "e",   .data = {.content = {.constant = EXP}, .type = CONST}},
    {.name = "pi",  .data = {.content = {.constant = PI},  .type = CONST}},
    {.name = "sin", .data = {.content = {.function = SIN}, .type = FUNC}},
    {.name = "cos", .data = {.content = {.function = COS}, .type = FUNC}},
    {.name = "tg",  .data = {.content = {.function = TG},  .type = FUNC}},
    {.name = "ctg", .data = {.content = {.function = CTG}, .type = FUNC}},
    {.name = "ln",  .data = {.content = {.function = LN},  .type = FUNC}},
};

int FindReservedName (char* name, tree_data_t* data)
{
    CustomAssert (reserved_names != NULL);

    for (size_t i = 0; i < sizeof (reserved_names) / sizeof (reserved_names[0]); ++i)
    {
        if (strcmp (name, reserved_names[i].name) == 0)
        {
            *data = reserved_names[i].data;
            return 0;
        }
    }

    return -1;
}

//--------------------------------------------------------------------------

tree_node_t** Tokenization (char* buffer, size_t buffer_size, int* shift)
{
    // TODO resize
    tree_node_t** token_array = (tree_node_t**) calloc (TOKEN_ARRAY_SIZE, sizeof (tree_node_t*));
    int token_array_shift = 0;

    while (buffer[*shift] != '\n' && (size_t) *shift < buffer_size)
    {
        // ебейшая проверка на то, что либо '-' у числа в самом начале строки либо после открывающей скобки
        if (('0' <= buffer[*shift] && buffer[*shift] <= '9') || (buffer[*shift] == '-' && 
        (token_array_shift == 0 || (token_array[token_array_shift - 1]->data.type == BRACKET && 
        token_array[token_array_shift - 1]->data.content.bracket == BRACKET_OP))))
        {
            int n = 0;
            double readen_number = 0;
            sscanf (buffer + *shift, "%lf%n", &readen_number, &n);

            _TOKEN_INIT (NUM, number, readen_number);

            *shift += n;
        }

        else if (('a' <= buffer[*shift] && buffer[*shift] <= 'z') || ('A' <= buffer[*shift] && buffer[*shift] <= 'Z'))
        {
            tree_data_t token_data = {};
            char name[10] = "";
            int n = 0;
            sscanf (buffer + *shift, "%[a-zA-Z]%n", name, &n);

            if (FindReservedName (name, &token_data))
            {
                if (n == 1)
                {
                    _TOKEN_INIT (VAR, variable, buffer[*shift]);
                    *shift += n;
                }

                else 
                {
                    SyntaxError (buffer[*shift]);
                }
            }

            else
            {
                if (token_data.type == CONST)
                {
                    _TOKEN_INIT (token_data.type, constant, token_data.content.constant);
                }
                else if (token_data.type == FUNC)
                {
                    _TOKEN_INIT (token_data.type, function, token_data.content.function);
                }

                *shift += n;
            }
        }

        else if (buffer[*shift] == '(' || buffer[*shift] == ')')
        {
            _TOKEN_INIT (BRACKET, operation, (operation_t) buffer[*shift]);
            *shift += 1;
        }

        else if (buffer[*shift] == '+' || buffer[*shift] == '-' || buffer[*shift] == '/' || 
                 buffer[*shift] == '*' || buffer[*shift] == '^')
        {
            _TOKEN_INIT (OP, operation, (operation_t) buffer[*shift]);
            *shift += 1;
        }

        else if (buffer[*shift] == ' ' || buffer[*shift] == '\r')
        {
            *shift += 1;
        }

        else 
        {
            SyntaxError (buffer[*shift]);
        }
    }

    // токен конца выражения
    _TOKEN_INIT (CONST, variable, '$');

    token_array = (tree_node_t**) MyRecalloc (token_array, (size_t) token_array_shift + 1, sizeof (tree_node_t*), TOKEN_ARRAY_SIZE, 0);
    return token_array;
}

//--------------------------------------------------------------------------

[[noreturn]] void SyntaxError (char symb)
{
    printf ("Syntax error: %c\n", symb);
    assert(0);
}

//--------------------------------------------------------------------------
