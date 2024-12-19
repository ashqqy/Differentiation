#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Tree.h"
#include "Common.h"
#include "Tokenization.h"
#include "CustomAssert.h"
#include "MyAllocation.h"

#define TOKEN_INIT_(data_type, union_type, union_data)                                \
    token_array[token_array_shift] = (tree_node_t*) calloc (1, sizeof (tree_node_t)); \
    token_array[token_array_shift]->data.type = data_type;                            \
    token_array[token_array_shift]->data.content.union_type = union_data;             \
    token_array_shift += 1

//--------------------------------------------------------------------------

int FindReservedDataByName (const char* name, tree_data_t* data)
{
    CustomAssert (name != NULL);
    CustomAssert (data != NULL);

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

const char* FindReservedNameByData (tree_data_t* data)
{
    CustomAssert (data != NULL);

    for (size_t i = 0; i < sizeof (reserved_names) / sizeof (reserved_names[0]); ++i)
    {
        if ((data->type == reserved_names[i].data.type && data->type == CONST && 
            data->content.constant == reserved_names[i].data.content.constant))
            return reserved_names[i].name;
        
        if ((data->type == reserved_names[i].data.type && data->type == FUNC &&
            data->content.function == reserved_names[i].data.content.function))
            return reserved_names[i].name;
            
    }

    return "Not found";
}

//--------------------------------------------------------------------------

tree_node_t** Tokenization (char* buffer, size_t buffer_size, int* shift)
{
    CustomAssert (buffer != NULL);
    CustomAssert (shift  != NULL);
    // TODO resize
    tree_node_t** token_array = (tree_node_t**) calloc (TOKEN_ARRAY_SIZE, sizeof (tree_node_t*));
    int token_array_shift = 0;

    while (buffer[*shift] != '\n' && (size_t) *shift < buffer_size)
    {
        // ебейшая проверка на то, что либо '-' у числа в самом начале строки либо после открывающей скобки
        if (isdigit(buffer[*shift]) || (buffer[*shift] == '-' && 
        (token_array_shift == 0 || (token_array[token_array_shift - 1]->data.type == SP_SYMB && 
        token_array[token_array_shift - 1]->data.content.special_symb == BRACKET_OP)))) // TODO
        {
            int n = 0;
            double readen_number = 0;
            sscanf (buffer + *shift, "%lf%n", &readen_number, &n);

            TOKEN_INIT_ (NUM, number, readen_number);

            *shift += n;
        }

        else if (isalpha(buffer[*shift]))
        {
            tree_data_t token_data = {};
            char name[100] = "";
            int n = 0;
            sscanf (buffer + *shift, "%[a-zA-Z]%n", name, &n);

            if (FindReservedDataByName (name, &token_data))
            {
                if (n == 1)
                {
                    TOKEN_INIT_ (VAR, variable, buffer[*shift]);
                    *shift += n;
                }

                else 
                {
                    SyntaxError ("big variable name");
                }
            }

            else
            {
                if (token_data.type == CONST)
                {
                    TOKEN_INIT_ (token_data.type, constant, token_data.content.constant);
                }
                else if (token_data.type == FUNC)
                {
                    TOKEN_INIT_ (token_data.type, function, token_data.content.function);
                }

                *shift += n;
            }
        }

        else if (buffer[*shift] == '(' || buffer[*shift] == ')')
        {
            TOKEN_INIT_ (SP_SYMB, special_symb, (special_symb_t) buffer[*shift]);
            *shift += 1;
        }

        else if (buffer[*shift] == '+' || buffer[*shift] == '-' || buffer[*shift] == '/' || 
                 buffer[*shift] == '*' || buffer[*shift] == '^')
        {
            TOKEN_INIT_ (OP, operation, (operation_t) buffer[*shift]);
            *shift += 1;
        }

        else if (buffer[*shift] == ' ' || buffer[*shift] == '\r')
        {
            *shift += 1;
        }

        else 
        {
            SyntaxError ("strange symbol");
        }
    }

    // токен конца выражения
    TOKEN_INIT_ (SP_SYMB, special_symb, EXPRESSION_END);

    token_array = (tree_node_t**) MyRecalloc (token_array, (size_t) token_array_shift + 1, sizeof (tree_node_t*), TOKEN_ARRAY_SIZE, 0);
    return token_array;
}

//--------------------------------------------------------------------------

void TokenArrayDestroy (tree_node_t** token_array)
{
    int i = 0;
    while ((token_array[i]->data.type != SP_SYMB) || (token_array[i]->data.content.special_symb != EXPRESSION_END))
    {
        free (token_array[i]); token_array[i] = NULL;
        i++;
    }
    free (token_array[i]); token_array[i] = NULL;
    free (token_array); token_array = NULL;
}

//--------------------------------------------------------------------------

void SyntaxError (const char* message)
{
    printf ("Syntax error: %s\n", message);
    assert(0);
}

//--------------------------------------------------------------------------
