#include "repl.h"

#include <stdio.h>

#include "args.h"
#include "compiler/source.h"

static int rstate = 1;

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
        repl_cmd(line + 1);
    else if (strlen(line) > 0) {
        char* cp = strdup(line);
        if (cp) {
            vector_append_items(*code_buffer, &cp, 1);
        }
    }

    free(line);
}

void repl_cmd(const char* cmd) {
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
        // TODO: Fix this command
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
        "(read stdin as lang) - Useful for "
        "debugging the lexer behaviour\n");
    printf("=========================\n");
}
