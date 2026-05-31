#include "args.h"

#include <stdio.h>
#include <string.h>

#include "compiler/source.h"

int debug = 0;

static int cmd_build(int argc, char* argv[], int i) {
    (void)argc;
    (void)argv;
    (void)i;
    printf("build: not implemented\n");
    return 0;
}

static int cmd_check(int argc, char* argv[], int i) {
    (void)argc;
    (void)argv;
    (void)i;
    printf("check: not implemented\n");
    return 0;
}

static int cmd_link(int argc, char* argv[], int i) {
    (void)argc;
    (void)argv;
    (void)i;
    printf("link: not implemented\n");
    return 0;
}

static int cmd_debug_on(int argc, char* argv[], int i) {
    (void)argc;
    (void)argv;
    (void)i;
    debug = 1;
    return 0;
}

static int cmd_ptok(int argc, char* argv[], int i) {
    if (i + 1 >= argc) {
        fprintf(stderr, "ptok: missing path\n");
        return 1;
    }
    char* path = argv[i + 1];
    return source_print_tokens(debug, path);
}

static Command compiler_cmds[] = {
    {"build", cmd_build},
    {"check", cmd_check},
    {"link", cmd_link},
};

static Command debug_cmds[] = {
    {"-g", cmd_debug_on},
    {"-debug", cmd_debug_on},
    {"-ptok", cmd_ptok},
};

int dispatch_command(int argc, char* argv[], int i) {
    const char* curr = argv[i];
    if (!curr) return -1;

    for (size_t j = 0; j < sizeof(compiler_cmds) / sizeof(compiler_cmds[0]);
         j++) {
        if (strcmp(curr, compiler_cmds[j].name) == 0)
            return compiler_cmds[j].handler(argc, argv, i);
    }
    for (size_t j = 0; j < sizeof(debug_cmds) / sizeof(debug_cmds[0]); j++) {
        if (strcmp(curr, debug_cmds[j].name) == 0)
            return debug_cmds[j].handler(argc, argv, i);
    }
    return -1;
}

int parse_arguments(int argc, char* argv[]) {
    if (argc == 1) {
        return -1;
    }

    if ((strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "-debug") == 0) &&
        argc == 2) {
        debug = 1;
    }

    for (int i = 1; i <= argc; ++i) {
        dispatch_command(argc, argv, i);
    }

    return 0;
}
