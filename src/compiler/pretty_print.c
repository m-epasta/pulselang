#include "pretty_print.h"

#include <stdio.h>
#include <string.h>

#include "lexer/lexer.h"

void print_tokens(Token** tokens, int size) {
    /* Does not store output, instead it prints it directly */
    int indent_level = 0;
    char curr_line[1024];

#define PRINT                                            \
    for (int t = 0; t < indent_level; ++i) printf("\t"); \
    printf("%s\n", curr_line);

    for (int i = 0; i < size; ++i) {
        Token* current = tokens[i];

        strcpy(curr_line, current->lexeme);
        if (current->typ == lbrace) {
            PRINT
            indent_level++;
        } else if (current->typ == rbrace) {
            indent_level--;
            if (indent_level < 0) indent_level = 0;
            PRINT
        } else {
            PRINT
        }

        curr_line[0] = '\0';
    }
}
