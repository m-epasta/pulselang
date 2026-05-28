#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/vector.h"
#include "compiler/source.h"

#define DEBUG_DIRECTIVE "LREPL.DIRECTIVE"

static int debug = 0;
static int rstate = 1;

vector_str parse_arguments(int argc, char* argv[]);
void repl(void);
int shouldNotClose(void);
void rpause(vector_str* code_buffer);
static void cmd(const char* cmd);
void help(void);

#if !defined(__GNUC__) && !defined(__clang__) && !defined(__TCC__)
#error \
    "Just why ? Please man stop suffering and install this frkin linux machine and shit on microslop"
#endif

int main(int argc, char* argv[]) {
    vector_str args = parse_arguments(argc, argv);
    if (args.size == 0 ||
        (args.size == 1 && strcmp(args.item[1], DEBUG_DIRECTIVE) == 0)) {
        repl();
        return -1;
    }
    vector_string_free(args);
}

vector_str parse_arguments(int argc, char* argv[]) {
    vector_str arr = vector_new();
    vector_init(arr);
    if (argc == 2) {
        arr.size = 0;
        vector_append_items(arr, argv + 1, argc - 1);
        return arr;
    } else if (argc >= 1) {
        return arr;
    }

    if (strcmp(argv[1], "-g") == 0 && argc == 2) {
        debug = 1;
        vector_push(arr, DEBUG_DIRECTIVE);
        return arr;
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[1], "-g") == 1) {
            debug = 1;
            vector_push(arr, DEBUG_DIRECTIVE);
            continue;
        } else if (strcmp(argv[i], "build") == 0) {
        } else if (strcmp(argv[i], "check") == 0) {
        } else if (strcmp(argv[i], "link") == 0) {
        } else {
        }
    }
    return arr;
}

void repl() {
    vector_str code_buffer = vector_new();
    vector_init(code_buffer);

    printf("pulselang repl\n");
    printf("Run code with :run\n");
    printf("Compile with :c\n");
    printf("Prints AST with :ast\n");
    printf("Exit with :q\n");
    printf("See more commands with :h\n");
    printf("=========================\n");

    while (rstate == 1) {
        printf("pulse> ");
        fflush(stdout);
        rpause(&code_buffer);
    }

    vector_string_free(code_buffer);
}

void rpause(vector_str* code_buffer) {
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;

    nread = getline(&line, &len, stdin);
    if (nread == -1) {
        printf("Could not read stdin");
        printf("\nexit\n");
        rstate = 0;
        free(line);
        return;
    }

    if (nread > 0 && line[nread - 1] == '\n') line[nread - 1] = '\0';

    if (line[0] == ':')
        cmd(line + 1);
    else if (strlen(line) > 0) {
        char* cp = strdup(line);
        if (cp) {
            vector_append_items(*code_buffer, &cp, 1);
        }
    }

    free(line);
}

static void cmd(const char* cmd) {
    if (strcmp(cmd, "q") == 0) {
        printf("exit\n");
        rstate = 0;
    } else if (strcmp(cmd, "run") == 0) {
        printf("not implemented\n");
    } else if (strcmp(cmd, "c") == 0) {
        printf("not implemented\n");
    } else if (strcmp(cmd, "ast") == 0) {
        printf("not implemented\n");
    } else if (strcmp(cmd, "h") == 0) {
        help();
    } else if (strcmp(cmd, "ptok") == 0) {
        char path[PATH_MAX];
        if (fgets(path, PATH_MAX, stdin) == NULL) {
            fprintf(stderr,
                    "Given filepath is too large (PATH_MAX = 4096), this path "
                    "should not be possible.");
        }
        source_print_tokens(debug, path);
    } else {
        printf("Invalid command: %s\n", cmd);
    }
}

void help(void) {
    printf("Run code with :run\n");
    printf("Compile with :c\n");
    printf("Prints AST with :ast\n");
    printf("Exit with :q\n");
    printf("\nDev specific commands: \n");
    printf(
        "\t:ptok prints tokens types based on a given file AS INPUT "
        "read stdin as lang) - Useful for "
        "debugging the lexer behaviour\n");
    printf("=========================\n");
}
