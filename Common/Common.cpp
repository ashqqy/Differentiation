#include <stdio.h>

#include "Common.h"
#include "CustomAssert.h"

//--------------------------------------------------------------------------

char* ReadFile (FILE* file_input, size_t* n_readen)
{
    size_t file_size = FileSizeFinder (file_input);
    CustomAssert (file_size != 0);

    char* buffer = (char*) calloc (file_size + 1, sizeof (char));
    CustomAssert (buffer != NULL);

    *n_readen = fread (buffer, sizeof (char), file_size, file_input);
    CustomAssert (*n_readen == file_size);

    return buffer;
}

//--------------------------------------------------------------------------

size_t FileSizeFinder (FILE* file_input)
{
    fseek (file_input, 0L, SEEK_END); //лучше fstat
    size_t size_file_input = (size_t) ftell(file_input);
    fseek(file_input, 0L, SEEK_SET);

    return size_file_input;
}

//--------------------------------------------------------------------------
