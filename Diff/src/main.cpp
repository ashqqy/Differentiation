#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Tree.h"
#include "Tokenization.h"
#include "Parce.h"
#include "Diff.h"
#include "Common.h"
#include "LatexOutput.h"
#include "Simplifier.h"

//--------------------------------------------------------------------------

int main ()
{
    FILE* input_file  = fopen ("./Examples/Input",  "r");
    FILE* output_file = fopen ("./Examples/Output", "w");

    size_t input_size = 0;
    char* input_buffer = ReadFile (input_file, &input_size);

    fclose (input_file);
    
    int shift = 0;
    tree_node_t** token_array = Tokenization (input_buffer, input_size, &shift);

    free (input_buffer); input_buffer = NULL;

    shift = 0;
    tree_node_t* root_node = GetG (token_array, &shift);

    tree_node_t* diff_root_node = Diff (root_node);
    Simplify (diff_root_node);
    TreeDump (diff_root_node);

    LatexOutput (output_file, diff_root_node);

    free (token_array); token_array = NULL;
    TreeDestroy (root_node);
    TreeDestroy (diff_root_node);
}

//--------------------------------------------------------------------------
