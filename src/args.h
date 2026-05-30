#ifndef ARGS_H
#define ARGS_H

#include <stddef.h>
#include "vector.h"

#define DEBUG_DIRECTIVE "LREPL.DIRECTIVE"

typedef int (*cmd_handler_t)(int argc, char *argv[], int i);

typedef struct {
    const char *name;
    cmd_handler_t handler;
} Command;

vector_str parse_arguments(int argc, char *argv[]);
int dispatch_command(int argc, char *argv[], int i);

extern int debug;

#endif
