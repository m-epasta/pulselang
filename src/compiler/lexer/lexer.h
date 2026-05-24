#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef struct {
    char* input;
    size_t pos;
    size_t line;
    size_t col;
} Scanner;

char scanner_get_char(Scanner* scanner);

Scanner* scanner_new(Scanner* scanner);
char scanner_current(Scanner* scanner);
void scanner_bump(Scanner* scanner);
void scanner_bumpnl(Scanner* scanner);
void scanner_skip(Scanner* scanner);
char* scanner_slice(Scanner* scanner, size_t start, size_t end);

typedef enum {
    // single-char
    ampersand,     // &
    attr,          // @
    backslash,     // \ wth is this parser doing ?
    bang,          // !
    caret,         // ^
    colon,         // :
    comma,         // ,
    dollar,        // $
    double_quote,  // "
    dot,           // .
    equal,         // =
    greater,       // >
    lbrace,        // {
    lbracket,      // [
    less,          // <
    lparen,        // (
    minus,         // -
    percent,       // %
    pipe,          // |
    plus,          // +
    question,      // ?
    rbrace,        // }
    rbracket,      // ]
    rparen,        // )
    sharp,         // #
    semicolon,     // ;
    single_quote,  // '
    slash,         // /
    star,          // *
    tilde,         // ~
    underscore,    // _

    // double-char
    arrow,          // ->
    equal_equal,    // ==
    slash_slash,    // //
    fat_arrow,      // =>
    greater_equal,  // >=
    less_equal,     // <=

    // keywords
    CONST,
    FOR,
    FN,
    IF,
    INCLUDE,
    RETURN,
    VAR,
    WHILE,

    // Others char
    eof,    // EOF || <c-d>
    nl,     // \n
    ident,  // identifier | isalnum() == 0

    // Special char
    // All numeric values are stored as index during lexing, then it gets
    // resolved by expanding or using the value in its context at semantic pass
    Index,

    // NOTE: Used only for development
    unknown,
} TokenType;

typedef struct {
    TokenType typ;
    char* lexeme;
    size_t line;
    size_t col;
} Token;

void tok_init(Token* tok);
Token next(Scanner* scanner);
TokenType match_lexeme(char* lexeme);
TokenType match_char(char c, Scanner* scanner);

#endif  // !LEXER_H
