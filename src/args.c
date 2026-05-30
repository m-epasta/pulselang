#include "../include/args.h"

#include <stdio.h>
#include <string.h>

#include "../include/vector.h"

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
    source_print_tokens(debug, path);
    return 0;
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

    if ((strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "-debug") == 0) &&
        argc == 2) {
        debug = 1;
        vector_push(arr, DEBUG_DIRECTIVE);
        return arr;
    }

    for (int i = 2; i <= argc; ++i) {
        dispatch_command(argc, argv, i);
    }
    return arr;
}
