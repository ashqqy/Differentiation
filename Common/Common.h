#ifndef COMMON
#define COMMON

char* ReadFile (FILE* file_input, size_t* n_readen);
size_t FileSizeFinder (FILE* file_input);

const char* const RED_COLOR     = "\033[1;31m";
const char* const MAGENTA_COLOR = "\033[1;35m";
const char* const DEFAULT_COLOR = "\033[1;0m";

#endif //COMMON