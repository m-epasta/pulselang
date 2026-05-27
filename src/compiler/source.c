#include "source.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"

char* take_source_code(char* filepath) {
    char abs_path[PATH_MAX];
    if (realpath(filepath, abs_path) == NULL) {
        perror("realpath");
        exit(1);
    }
    FILE* file = fopen(abs_path, "rb");
    if (!file) {
        perror("fopen");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    if (size < 0) {
        perror("ftell");
        exit(1);
    }
    char* buffer = malloc(size + 1);
    if (!buffer) {
        perror("malloc");
        exit(1);
    }
    size_t read_bytes = fread(buffer, 1, size, file);
    buffer[read_bytes] = '\0';
    fclose(file);
    return buffer;
}

/* UTILS */

char* read_rel(char path[PATH_MAX]) {}

/* DEBUG */

void source_print_tokens(int debug_mode, char path[PATH_MAX]) {
    if (!(debug_mode == -1 || debug_mode == 1)) {
        return;
    }

    read_rel(path);
}
