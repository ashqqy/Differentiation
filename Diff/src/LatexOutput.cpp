#include <stdio.h>

#include "Tokenization.h"
#include "CustomAssert.h"
#include "Tree.h"
#include "LatexOutput.h"

//--------------------------------------------------------------------------

void LatexOutput (FILE* output_file, tree_node_t* node)
{
    CustomAssert (output_file != NULL);
    if (node == NULL)
        return;


    switch (node->data.type)
    {
        case NUM:
        {
            fprintf (output_file, "%lg", node->data.content.number);
            break;
        }
        
        case CONST:
        {
            fprintf (output_file, "%s", FindReservedName (&node->data));
            break;
        }

        case VAR:
        {
            fprintf (output_file, "%c", node->data.content.variable);
            break;
        }

        case OP:
        {
            // if (node->data.content.operation == DIV)
            // {
            //     fprintf (output_file, "/frac{");
            //     LatexOutput (output_file, node->left);
            //     fprintf (output_file, "}{");
            //     LatexOutput (output_file, node->right);
            //     fprintf (output_file, "}");
            //     break;
            // }

            if ((node->left->data.type == OP) && (node->data.content.operation == DEG ||
                ((node->data.content.operation == MUL) && ((node->left->data.content.operation == ADD) 
                || (node->left->data.content.operation == SUB)))))
            {
                fprintf (output_file, "(");
                LatexOutput (output_file, node->left);
                fprintf (output_file, ")");
            }
            else 
                LatexOutput (output_file, node->left);

            fprintf (output_file, "%c", node->data.content.operation);

            if ((node->right->data.type == OP) && (node->data.content.operation == DEG ||
                ((node->data.content.operation == MUL) && ((node->right->data.content.operation == ADD) 
                || (node->right->data.content.operation == SUB)))))
            {
                fprintf (output_file, "(");
                LatexOutput (output_file, node->right);
                fprintf (output_file, ")");
            }
            else 
                LatexOutput (output_file, node->right);

            break;
        }
            
        case FUNC:
        {
            fprintf (output_file, "%s(", FindReservedName (&node->data));
            LatexOutput (output_file, node->left);
            fprintf (output_file, ")");
            break;
        }

        case SP_SYMB:
        {
            fprintf (output_file, "%c", node->data.content.special_symb);
            break;
        }

        default:
            break;
    }
}

//--------------------------------------------------------------------------
