#ifndef SOURCE_H
#define SOURCE_H

#include "lexer/lexer.h"
#define PATH_MAX 4096

char* take_source_code(char* source_path);

/* UTILS */
char* read_rel(char source_file[PATH_MAX]);

/* DEBUG */
int source_print_tokens(int debug_mode, char path[PATH_MAX]);
void tprinter(vector_token tokens);

#endif  // !SOURCE_H
