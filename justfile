default: ci

ci:
    echo "not there yet"

build-all:
    make
    make embed
    make debug
    make embed_debug

valgrind:
    ./scripts/val.sh

clean-build:
    rm -rf build/
    rm -rf integration_tests/.cache
    rm -f vgcore* core*
    ./scripts/nixclean.sh

gcc-compile-cmd:
    make clean
    bear -- make

# Recommend to use gcc instead (better diagnostics and debug hints)
# You also do not have include/*.h lsp support (fails to retrieve header files)
tcc-compile-cmd:
    make embed_clean
    bear -- make embed


# CODEGEN
gen-cli-codes ARGS="":
    ./scripts/gen_clicodes.sh {{ARGS}}


# TESTS

fuzz:
    # Not verified, simply tests some features without checking on it
    ./integration_tests/fuzz.sh
