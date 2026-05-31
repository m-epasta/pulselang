#include "args.h"
#include "repl.h"

#if !defined(__GNUC__) && !defined(__clang__) && !defined(__TCC__)
#error \
    "Just why ? Please man stop suffering and install this frkin linux machine and shit on microslop"
#endif

int main(int argc, char* argv[]) {
    int ret = parse_arguments(argc, argv);
    if (ret == -1) {
        repl();
        return -1;
    }

    return ret;
}
