#include "args.h"
#include "repl.h"

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
