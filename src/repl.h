#ifndef REPL_H
#define REPL_H

#include "vector.h"

void repl(void);
void rpause(vector_str* code_buffer);
void repl_cmd(const char* cmd);
void help(void);

#endif  // !REPL_H
