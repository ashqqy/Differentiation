#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Tree.h"
#include "Tokenization.h"
#include "Parce.h"
#include "Diff.h"
#include "Common.h"

//--------------------------------------------------------------------------

int main ()
{
    FILE* input_file  = fopen ("./Examples/Input",  "r");
    // FILE* output_file = fopen ("./Examples/Output", "w");

    size_t input_size = 0;
    char* input_buffer = ReadFile (input_file, &input_size);
    
    int shift = 0;
    tree_node_t** token_array = Tokenization (input_buffer, input_size, &shift);

    printf ("node 0:\n");
    printf ("type = %d\n", token_array[0]->data.type);
    printf ("data = %lf\n\n", token_array[0]->data.content.number);

    printf ("node 1:\n");
    printf ("type = %d\n", token_array[1]->data.type);
    printf ("data = %c\n\n", token_array[1]->data.content.operation);

    printf ("node 2:\n");
    printf ("type = %d\n", token_array[2]->data.type);
    printf ("data = %d\n\n", token_array[2]->data.content.function);

    free (input_buffer); input_buffer = NULL;

    int i = 0;
    while (token_array[i]->data.content.variable != '$')
    {
        free (token_array[i]); token_array[i] = NULL;
        i++;
    }
    free (token_array[i]); token_array[i] = NULL;
    free (token_array); token_array = NULL;

    // tree_node_t* root_node = GetG ();
    // tree_node_t* diff_root_node = Diff (root_node);
    // TreeDump (diff_root_node);
}

//--------------------------------------------------------------------------
