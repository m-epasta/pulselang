#include "pretty_print.h"

#include <stdio.h>

#include "lexer/lexer.h"

static const char* token_type_str(TokenType typ) {
    switch (typ) {
        case ampersand:     return "&";
        case attr:          return "@";
        case backslash:     return "\\\\";
        case bang:          return "!";
        case caret:         return "^";
        case colon:         return ":";
        case comma:         return ",";
        case dollar:        return "$";
        case double_quote:  return "\"";
        case dot:           return ".";
        case equal:         return "=";
        case greater:       return ">";
        case lbrace:        return "{";
        case lbracket:      return "[";
        case less:          return "<";
        case lparen:        return "(";
        case minus:         return "-";
        case percent:       return "%";
        case pip:           return "|";
        case plus:          return "+";
        case question:      return "?";
        case rbrace:        return "}";
        case rbracket:      return "]";
        case rparen:        return ")";
        case sharp:         return "#";
        case semicolon:     return ";";
        case single_quote:  return "'";
        case slash:         return "/";
        case star:          return "*";
        case tilde:         return "~";
        case underscore:    return "_";
        case arrow:         return "->";
        case equal_equal:   return "==";
        case slash_slash:   return "//";
        case fat_arrow:     return "=>";
        case greater_equal: return ">=";
        case less_equal:    return "<=";
        case CONST:         return "const";
        case FOR:           return "for";
        case FN:            return "fn";
        case IF:            return "if";
        case INCLUDE:       return "include";
        case RETURN:        return "return";
        case VAR:           return "var";
        case WHILE:         return "while";
        case eof:           return "eof";
        case nl:            return "nl";
        case ident:         return "ident";
        case Index:         return "Int";
        case unknown:       return "�";
    }
    return "�";
}

void print_tokens(Token** tokens, int size) {
    int indent = 0;
    int bol = 1;

    for (int i = 0; i < size; ++i) {
        Token* tok = tokens[i];
        TokenType typ = tok->typ;

        if (typ == eof) {
            printf("<eof>\n");
            break;
        }

        if (typ == nl) {
            printf("\n");
            bol = 1;
            continue;
        }

        if (typ == rbrace) {
            indent--;
            if (indent < 0) indent = 0;
            if (!bol) {
                printf("\n");
                bol = 1;
            }
        }

        if (bol) {
            for (int j = 0; j < indent; ++j) printf("\t");
            bol = 0;
        }

        printf("%s", token_type_str(typ));
        if (tok->lexeme && (typ == ident || typ == Index)) {
            printf(":%s", tok->lexeme);
        }

        if (typ == lbrace) {
            indent++;
        }

        printf(" ");
    }
}
