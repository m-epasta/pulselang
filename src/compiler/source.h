#ifndef SOURCE_H
#define SOURCE_H

#define PATH_MAX 4096

char* take_source_code(char* source_path);

/* UTILS */
char* read_rel(char source_file[PATH_MAX]);

/* DEBUG */
void source_print_tokens(int debug_mode, char path[PATH_MAX]);

#endif  // !SOURCE_H
