#include "source.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lexer/lexer.h"
#include "vector.h"

#define READ_CHUNK 4096

// TODO: refactor errors so the users knows where it failed.
// Maybe annotate it with source.c ?
char* take_source_code(char* source_path) {
    char abs_path[PATH_MAX];
    if (realpath(source_path, abs_path) == NULL) {
        // TODO: Check if realpath also fails on already canonical path
        fprintf(stderr, "Could not expand %s to absolutz path", source_path);
        exit(1);
    }
    FILE* file = fopen(abs_path, "rb");
    if (!file) {
        fprintf(stderr, "Could not open %s in read-binary mode", abs_path);
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    if (size < 0) {
        fprintf(stderr, "ftell: Failed to determine size of %s (file object)",
                abs_path);
        exit(1);
    }
    char* buffer = malloc(size + 1);
    if (!buffer) {
        fprintf(stderr, "malloc failed");
        exit(1);
    }
    size_t read_bytes = fread(buffer, 1, size, file);
    buffer[read_bytes] = '\0';
    fclose(file);
    return buffer;
}

/* UTILS */

char* read_rel(char source_path[PATH_MAX]) {
    int fd = open(source_path, O_RDONLY);
    if (fd == -1) return NULL;

    char* buf = NULL;
    char* tmp;
    size_t total = 0;
    size_t capacity = 0;
    ssize_t n;

    while (1) {
        if (total + READ_CHUNK + 1 > capacity) {
            capacity = total + READ_CHUNK + 1;
            tmp = realloc(buf, capacity);
            if (!tmp) {
                free(buf);
                close(fd);
                return NULL;
            }
            buf = tmp;
        }

        n = read(fd, buf + total, READ_CHUNK);
        if (n == -1) {
            free(buf);
            close(fd);
            return NULL;
        }
        if (n == 0) break;
        total += n;
    }

    if (buf) {
        buf[total] = '\0';
    } else {
        buf = malloc(1);
        if (buf) buf[0] = '\0';
    }

    close(fd);
    return buf;
}

/* DEBUG */

int source_print_tokens(int debug_mode, char path[PATH_MAX]) {
    if (debug_mode == 1) {
        return -1;
    }

    char* content = read_rel(path);
    if (content == NULL) {
        fprintf(stderr, "read_rel: failed to read file %s", path);
        return -1;
    } else if (content[0] == '\0') {
        fprintf(stderr, "read_rel: %s is empty", path);
        return -1;
    }

    vector_token tokens = lex(content);

    tprinter(tokens);
    return 0;
}

static const char* token_type_str(TokenType typ) {
    switch (typ) {
        case ampersand:
            return "&";
        case attr:
            return "@";
        case backslash:
            return "\\\\";
        case bang:
            return "!";
        case caret:
            return "^";
        case colon:
            return ":";
        case comma:
            return ",";
        case dollar:
            return "$";
        case double_quote:
            return "\"";
        case dot:
            return ".";
        case equal:
            return "=";
        case greater:
            return ">";
        case lbrace:
            return "{";
        case lbracket:
            return "[";
        case less:
            return "<";
        case lparen:
            return "(";
        case minus:
            return "-";
        case percent:
            return "%";
        case pip:
            return "|";
        case plus:
            return "+";
        case question:
            return "?";
        case rbrace:
            return "}";
        case rbracket:
            return "]";
        case rparen:
            return ")";
        case sharp:
            return "#";
        case semicolon:
            return ";";
        case single_quote:
            return "'";
        case slash:
            return "/";
        case star:
            return "*";
        case tilde:
            return "~";
        case underscore:
            return "_";
        case arrow:
            return "->";
        case equal_equal:
            return "==";
        case slash_slash:
            return "//";
        case fat_arrow:
            return "=>";
        case greater_equal:
            return ">=";
        case less_equal:
            return "<=";
        case CONST:
            return "const";
        case FOR:
            return "for";
        case FN:
            return "fn";
        case IF:
            return "if";
        case INCLUDE:
            return "include";
        case RETURN:
            return "return";
        case VAR:
            return "var";
        case WHILE:
            return "while";
        case eof:
            return "eof";
        case nl:
            return "nl";
        case ident:
            return "ident";
        case Index:
            // Techincally an integer
            return "Int";
        case unknown:
            return "�";
    }
    return "�";
}

void tprinter(vector_token tokens) {
    for (size_t i = 0; i < tokens.size; ++i) {
        Token* tok = vector_item(tokens, i);

        if (tok->typ == eof) {
            printf("<eof>\n");
            break;
        }

        if (tok->typ == nl) {
            printf("\n");
            continue;
        }

        printf("%s", token_type_str(tok->typ));

        if (tok->lexeme && (tok->typ == ident || tok->typ == Index)) {
            printf(":%s", tok->lexeme);
        }

        printf(" ");
    }
}
