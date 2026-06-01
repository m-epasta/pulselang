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

char scanner_current(Scanner* scanner) {
    char c = scanner->input[scanner->pos];
    return c == '\0' ? EOF : c;
}

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
    while (1) {
        char curr = scanner_current(scanner);

        if (curr == EOF)
            return;
        else if (curr == ' ' || curr == '\t' || curr == '\r') {
            scanner_bump(scanner);
            continue;
        } else if (curr == '/') {
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
                    ch = scanner_current(scanner);
                }
                return;
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
                return;
            } else {
                return;
            }
        } else
            return;
    }
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
        scanner_bump(scanner);
        tok.col = ncol;
        tok.line = nline;
        tok.lexeme = strdup("\n");
        tok.typ = nl;
        return tok;
    }

    if (isdigit(current)) {
        while (scanner_current(scanner) != EOF) {
            char c = scanner_current(scanner);
            if (isdigit(c) || c == '.' || c == '_')
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
            char c = scanner_current(scanner);
            if (isalnum(c) || c == '_')
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
        return tok;
    }

    scanner_bump(scanner);

    TokenType typ;
    switch (current) {
        case '(': typ = lparen; break;
        case ')': typ = rparen; break;
        case '{': typ = lbrace; break;
        case '}': typ = rbrace; break;
        case '[': typ = lbracket; break;
        case ']': typ = rbracket; break;
        case '+': typ = plus; break;
        case '-': {
            if (scanner_current(scanner) == '>') { scanner_bump(scanner); typ = arrow; }
            else typ = minus;
            break;
        }
        case '*': typ = star; break;
        case '/': {
            if (scanner_current(scanner) == '/') { scanner_bump(scanner); typ = slash_slash; }
            else typ = slash;
            break;
        }
        case '%': typ = percent; break;
        case '&': typ = ampersand; break;
        case '|': typ = pip; break;
        case '!': typ = bang; break;
        case '?': typ = question; break;
        case ':': typ = colon; break;
        case ';': typ = semicolon; break;
        case ',': typ = comma; break;
        case '.': typ = dot; break;
        case '=': {
            char n = scanner_current(scanner);
            if (n == '=') { scanner_bump(scanner); typ = equal_equal; }
            else if (n == '>') { scanner_bump(scanner); typ = fat_arrow; }
            else typ = equal;
            break;
        }
        case '>': {
            if (scanner_current(scanner) == '=') { scanner_bump(scanner); typ = greater_equal; }
            else typ = greater;
            break;
        }
        case '<': {
            if (scanner_current(scanner) == '=') { scanner_bump(scanner); typ = less_equal; }
            else typ = less;
            break;
        }
        case '@': typ = attr; break;
        case '#': typ = sharp; break;
        case '$': typ = dollar; break;
        case '^': typ = caret; break;
        case '~': typ = tilde; break;
        case '_': typ = underscore; break;
        case '\\': typ = backslash; break;
        case '\'': typ = single_quote; break;
        case '"': typ = double_quote; break;
        default:
            fprintf(stderr, "%c: unknown char (lexeme)", current);
            typ = unknown;
            break;
    }

    tok.col = col;
    tok.line = line;
    tok.lexeme = scanner_slice(scanner, start, scanner->pos);
    tok.typ = typ;
    return tok;
}

TokenType match_lexeme(char* lexeme) {
    if (strcmp(lexeme, "const") == 0)
        return CONST;
    else if (strcmp(lexeme, "for") == 0)
        return FOR;
    else if (strcmp(lexeme, "fn") == 0)
        return FN;
    else if (strcmp(lexeme, "if") == 0)
        return IF;
    else if (strcmp(lexeme, "include") == 0)
        return INCLUDE;
    else if (strcmp(lexeme, "return") == 0)
        return RETURN;
    else if (strcmp(lexeme, "var") == 0)
        return VAR;
    else if (strcmp(lexeme, "while") == 0)
        return WHILE;
    else
        return ident;
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
    vector_token tokens = vector_new();
    vector_init(tokens);
    Scanner scanner;
    scanner_new(&scanner, source);

    while (1) {
        Token* tok = malloc(sizeof(Token));
        *tok = next(&scanner);
        vector_push(tokens, tok);
        if (tok->typ == eof) break;
    }

    return tokens;
}
