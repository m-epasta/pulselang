#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Scanner* scanner_new(Scanner* scanner, char* input) {
    scanner->input = input;
    scanner->pos = 0;
    scanner->line = 0;
    scanner->col = 0;
    return scanner;
}

char scanner_current(Scanner* scanner) { return scanner->input[scanner->pos]; }

void scanner_bump(Scanner* scanner) {
    char curr = scanner_current(scanner);
    if (curr == EOF) {
        return;
    }
    scanner->pos += 1;
    if (curr == '\n') {
        scanner->line += 1;
        scanner->col = 0;
        return;
    }
    scanner->col += 1;
}

void scanner_bumpnl(Scanner* scanner) {
    scanner->line += 1;
    scanner->col = 0;
    return;
}

void scanner_skip(Scanner* scanner) {
    char curr = scanner_current(scanner);

    if (curr == EOF)
        return;
    else if (curr == ' ' || curr == '\t' || curr == '\r')
        scanner_bump(scanner);
    else if (curr == '/') {
        char next_ch = scanner->input[scanner->pos + 1];
        if (next_ch == '/') {
            scanner_bump(scanner);
            scanner_bump(scanner);

            char ch = scanner_current(scanner);
            while (ch != EOF) {
                if (ch == '\n') {
                    return;
                }

                scanner_bump(scanner);
            }
        } else if (next_ch == '*') {
            scanner_bump(scanner);
            scanner_bump(scanner);

            char ch = scanner_current(scanner);
            while (ch != EOF) {
                if (ch == '*' && scanner->input[scanner->pos + 1] == '/') {
                    scanner_bump(scanner);
                    scanner_bump(scanner);
                    return;
                }
                scanner_bump(scanner);
                ch = scanner_current(scanner);
            }
        } else {
            return;
        }
    } else
        return;
}

char* scanner_slice(Scanner* scanner, size_t start, size_t end) {
    size_t len = end - start;
    char* slice = malloc(len + 1);
    memcpy(slice, scanner->input + start, len);
    slice[len] = '\0';
    return slice;
}

void tok_init(Token* tok) {
    tok->col = 0;
    tok->line = 0;
    tok->lexeme = 0;
    tok->typ = eof;
    return;
}

Token next(Scanner* scanner) {
    scanner_skip(scanner);
    size_t line = scanner->line;
    size_t col = scanner->col;
    size_t start = scanner->pos;

    char current = scanner_current(scanner);
    Token tok;
    tok_init(&tok);
    if (current == EOF) {
        tok.col = col;
        tok.line = line;
        tok.lexeme = strdup("");
        tok.typ = eof;
        return tok;
    }

    if (current == '\n') {
        size_t nline = scanner->line;
        size_t ncol = scanner->col;
        scanner_bumpnl(scanner);
        tok.col = ncol;
        tok.line = nline;
        tok.lexeme = strdup("\n");
        tok.typ = nl;
        return tok;
    }

    if (isdigit(current)) {
        while (scanner_current(scanner) != EOF) {
            if (isdigit(current) || current == '.' || current == '_')
                scanner_bump(scanner);
            else
                break;
        }

        tok.line = line;
        tok.col = col;
        tok.lexeme = scanner_slice(scanner, start, scanner->pos);
        tok.typ = Index;
        return tok;
    }

    if (isalpha(current) || current == '_') {
        while (scanner_current(scanner) != EOF) {
            if (isalnum(current) || current == '_')
                scanner_bump(scanner);
            else
                break;
        }

        char* lexeme = scanner_slice(scanner, start, scanner->pos);
        TokenType typ = match_lexeme(lexeme);

        tok.col = col;
        tok.line = line;
        tok.typ = typ;
        tok.lexeme = strdup(lexeme);
    }

    scanner_bump(scanner);
    TokenType typ = match_char(scanner_current(scanner), scanner);

    tok.col = col;
    tok.line = line;
    tok.lexeme = scanner_slice(scanner, start, scanner->pos);
    tok.typ = typ;
    return tok;
}

TokenType match_lexeme(char* lexeme) {
    if (strcmp(lexeme, "const"))
        return CONST;
    else if (strcmp(lexeme, "for"))
        return FOR;
    else if (strcmp(lexeme, "fn"))
        return FN;
    else if (strcmp(lexeme, "if"))
        return IF;
    else if (strcmp(lexeme, "include"))
        return INCLUDE;
    else if (strcmp(lexeme, "return"))
        return RETURN;
    else if (strcmp(lexeme, "var"))
        return VAR;
    else if (strcmp(lexeme, "while"))
        return WHILE;
    else {
        printf("%s: as ident", lexeme);
        return ident;
    }
}

TokenType match_char(char c, Scanner* scanner) {
    switch (c) {
        case '&':
            return ampersand;
        case '@':
            return attr;
        case '\\':
            return backslash;
        case '!':
            return bang;
        case '^':
            return caret;
        case ':':
            return colon;
        case ',':
            return comma;
        case '$':
            return dollar;
        case '"':
            return double_quote;
        case '.':
            return dot;

        case '=': {
            Token nextok = next(scanner);
            if (nextok.typ == equal)
                return equal_equal;
            else if (nextok.typ == greater)
                return fat_arrow;
            else
                return equal;
        }

        case '>': {
            if (next(scanner).typ == equal)
                return greater_equal;
            else
                return greater;
        }

        case '{':
            return lbrace;
        case '[':
            return lbracket;

        case '<': {
            if (next(scanner).typ == equal)
                return less_equal;
            else
                return less;
        }

        case '(':
            return lparen;

        case '-': {
            if (next(scanner).typ == greater)
                return arrow;
            else
                return minus;
        }

        case '%':
            return percent;
        case '|':
            return pip;
        case '+':
            return plus;
        case '?':
            return question;
        case '}':
            return rbrace;
        case ']':
            return rbracket;
        case ')':
            return rparen;
        case '#':
            return sharp;
        case ';':
            return semicolon;
        case '\'':
            return single_quote;

        case '/': {
            if (next(scanner).typ == slash)
                return slash_slash;
            else
                return slash;
        }

        case '*':
            return star;
        case '~':
            return tilde;
        case '_':
            return underscore;

        default:
            // TODO: Proper error handling
            // Should it errors ? We may be able to do some linking on that ?
            fprintf(stderr, "%c: unknown char (lexeme)", c);
            return unknown;
    }
}

#pragma endregion lexer

vector_token lex(char* source) {
    int idx = 0;
    vector_token tokens = vector_new();
    vector_init(tokens);
    Scanner* scanner = NULL;
    scanner_new(scanner, source);

    do {
        Token tok = next(scanner);
        vector_push(tokens, &tok);
        idx++;
    } while (source[idx] != EOF);

    return tokens;
}
